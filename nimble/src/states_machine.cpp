#include <chrono>
#include <functional>
#include <memory>
#include <string>
#include <cstdlib>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/int16.hpp"
#include "std_msgs/msg/int32_multi_array.hpp"
#include "std_msgs/msg/float32_multi_array.hpp"
#include "std_msgs/msg/byte_multi_array.hpp"
#include "nimble_interfaces/msg/measurements.hpp"
#include "nimble_interfaces/msg/therapy_requirements.hpp"
#include "nimble_interfaces/msg/frame_state.hpp"
#include "nimble_interfaces/msg/cartesian_trajectory.hpp"
#include "nimble_interfaces/msg/joints_trajectory.hpp"
#include "trajectory_msgs/msg/joint_trajectory.hpp"
#include "sensor_msgs/msg/joint_state.hpp"

#include "nimble_interfaces/srv/traj_generator_service.hpp"


using namespace std::chrono_literals;

//Estructura compartida para almacenar datos al recibir cada topic
struct SharedData {
    nimble_interfaces::msg::JointsTrajectory joints_trajectory;
    nimble_interfaces::msg::CartesianTrajectory cartesian_trajectory;
    nimble_interfaces::msg::CartesianTrajectory cartesian_state;
    nimble_interfaces::msg::JointsTrajectory joints_acc_state;
    sensor_msgs::msg::JointState cables_state;
    nimble_interfaces::msg::TherapyRequirements step_target;
    nimble_interfaces::msg::TherapyRequirements therapy_requirements;
    nimble_interfaces::msg::Measurements measurements;
    std_msgs::msg::Float32MultiArray interaction_torque;
    std_msgs::msg::ByteMultiArray FSR;
    
 //Duplicar datos de measurements y th_requirements para solo solicitar el servicio de calculo de trayectorias si algo se modifica
    nimble_interfaces::msg::TherapyRequirements therapy_requirements_previous;
    nimble_interfaces::msg::Measurements measurements_previous;
    
//Iniciar a 0 todos los valores, para que se solicite el servicio al primer mensaje (chapuza)  
    SharedData(){

	    measurements_previous.femur=0;
	    measurements_previous.tibia=0;
	    measurements_previous.height=0;
	    measurements_previous.height_ankle=0;
	    measurements_previous.distance_to_heel=0;
	    measurements_previous.distance_to_toe=0;
	    measurements_previous.depth_pelvis=0;
	    measurements_previous.width_pelvis=0;
	    therapy_requirements_previous.step_length=0;
	    therapy_requirements_previous.step_height=0;
    
    }
    
};

//NODO

class StatesMachineNode : public rclcpp::Node
{
public:
    StatesMachineNode() : Node("states_machine")
    {	
    	//Possible internal parameters
        this->declare_parameter("contact_point", 1);
    	this->declare_parameter("param2", 2);
    	
        // Create a subscribers 
                    
        subscriber_joints_acc_state = create_subscription<nimble_interfaces::msg::JointsTrajectory>(
            "joints_acc_state", 10,
            [this](const nimble_interfaces::msg::JointsTrajectory msg) {
                // Callback function that publishes the received Int64 message
                call_back_joints_acc_state(msg);
            }); 
                    
       suscriber_cartesian_trajectory = create_subscription<nimble_interfaces::msg::CartesianTrajectory>(
            "cartesian_trajectory", 10,
            [this](const nimble_interfaces::msg::CartesianTrajectory msg) {
                
                call_back_cartesian_trajectory(msg);
            }); 
       suscriber_cartesian_state = create_subscription<nimble_interfaces::msg::CartesianTrajectory>(
            "cartesian_state", 10,
            [this](const nimble_interfaces::msg::CartesianTrajectory msg) {
                
                call_back_cartesian_state(msg);
            }); 
            
       subscriber_state_cables = create_subscription<sensor_msgs::msg::JointState>(
            "cables_state", 10,
            [this](const sensor_msgs::msg::JointState msg) {
                
                call_back_state_cables(msg);
            });      
        suscriber_step_target = create_subscription<nimble_interfaces::msg::TherapyRequirements>(
            "step_target", 10,
            [this](const nimble_interfaces::msg::TherapyRequirements msg) {
                
                call_back_step_target(msg);
            });
        suscriber_therapy_requirements = create_subscription<nimble_interfaces::msg::TherapyRequirements>(
            "therapy_requirements", 10,
            [this](const nimble_interfaces::msg::TherapyRequirements msg) {
                
                call_back_therapy_requirements(msg);
            });
                
        suscriber_measurements = create_subscription<nimble_interfaces::msg::Measurements>(
            "measurements", 10,
            [this](const nimble_interfaces::msg::Measurements msg) {
                
                call_back_measurements(msg);
            }); 
         
         suscriber_interaction_torque = create_subscription<std_msgs::msg::Float32MultiArray>(
            "interactionTorque", 10,
            [this](const std_msgs::msg::Float32MultiArray msg) {
                
                call_back_interaction_torque(msg);
            });
            
         suscriber_FSR = create_subscription<std_msgs::msg::ByteMultiArray>(
            "FSR", 10,
            [this](const std_msgs::msg::ByteMultiArray msg) {
                
                call_back_FSR(msg);
            });      
              
                   
        
        // Create publisher for topics
        
        publisher_joints_trajectory = create_publisher<nimble_interfaces::msg::JointsTrajectory>("joints_trajectory", 10);
        publisher_assistLevel = create_publisher<std_msgs::msg::Int32MultiArray>("assistLevel", 10);
        publisher_executionMode = create_publisher<std_msgs::msg::Int32MultiArray>("executionMode", 10);
        publisher_controlMode = create_publisher<std_msgs::msg::Int16>("controlMode", 10);
        
        //Create callback groups to avoid multi-executor issues
        client_cb_group_ = this->create_callback_group(rclcpp::CallbackGroupType::MutuallyExclusive);
        
        
        // Create a client for your service
        client_ = this->create_client<nimble_interfaces::srv::TrajGeneratorService>("traj_generator_service", rmw_qos_profile_services_default, client_cb_group_);
        
        //Create wall timer to publish periodically
        timer_ = this->create_wall_timer(1000ms, std::bind(&StatesMachineNode::timer_callback, this));                
    }

private:
    //Instancias	
    rclcpp::CallbackGroup::SharedPtr client_cb_group_; //callback group para poder simultanear servicio y nodo
    rclcpp::Client<nimble_interfaces::srv::TrajGeneratorService>::SharedPtr client_; //cliente pata el servicio de trayectoria
    rclcpp::TimerBase::SharedPtr timer_;	 //timer
    SharedData	shared_data_; //estructura de datos
    //Subscribers
    rclcpp::Subscription<nimble_interfaces::msg::JointsTrajectory>::SharedPtr subscriber_joints_acc_state;
    rclcpp::Subscription<nimble_interfaces::msg::CartesianTrajectory>::SharedPtr suscriber_cartesian_trajectory;
    rclcpp::Subscription<nimble_interfaces::msg::CartesianTrajectory>::SharedPtr suscriber_cartesian_state;
    rclcpp::Subscription<sensor_msgs::msg::JointState>::SharedPtr subscriber_state_cables;
    rclcpp::Subscription<nimble_interfaces::msg::TherapyRequirements>::SharedPtr suscriber_step_target;
    rclcpp::Subscription<nimble_interfaces::msg::TherapyRequirements>::SharedPtr suscriber_therapy_requirements;
    rclcpp::Subscription<nimble_interfaces::msg::Measurements>::SharedPtr suscriber_measurements;
    rclcpp::Subscription<std_msgs::msg::Float32MultiArray>::SharedPtr suscriber_interaction_torque;
    rclcpp::Subscription<std_msgs::msg::ByteMultiArray>::SharedPtr suscriber_FSR;
    //Publishers
    rclcpp::Publisher<nimble_interfaces::msg::JointsTrajectory>::SharedPtr publisher_joints_trajectory;
    rclcpp::Publisher<std_msgs::msg::Int32MultiArray>::SharedPtr publisher_assistLevel;
    rclcpp::Publisher<std_msgs::msg::Int32MultiArray>::SharedPtr publisher_executionMode;
    rclcpp::Publisher<std_msgs::msg::Int16>::SharedPtr publisher_controlMode;
    
        
    // Funcion para convertir JointTrajectory a string (solo para mostrarla por el terminal)
    std::string jointTrajectoryToString(const nimble_interfaces::msg::JointsTrajectory::SharedPtr& joints_trajectory) {
    
	    std::stringstream ss;

	    ss << "Joint Names: ";
	    for (const auto& joint_name : joints_trajectory->joint_trajectory.joint_names) {
		ss << joint_name << " ";
	    }
	    ss << std::endl;

	    for (const auto& point : joints_trajectory->joint_trajectory.points) {
		ss << "Point:" << std::endl;
		ss << "  Positions: ";
		for (const auto& position : point.positions) {
		    ss << position << " ";
		}
		ss << std::endl;
		ss << "  Time from start: " << point.time_from_start.sec << " seconds" << std::endl;
	    }

	    return ss.str();
 }
 
 
//Funcion para comprobar si hay modificaciones en las medidas del paciente o los requisitos del fisio y poder solicitar trayectoria ideal (tambien se ejecuta con el primer mensaje)
    bool check_variations_intherapy(const nimble_interfaces::msg::Measurements & measurements, const nimble_interfaces::msg::TherapyRequirements & therapy_requirements){
    
    	bool therapy_variation=false;
    	if (measurements.height_ankle !=shared_data_.measurements_previous.height_ankle){
    		therapy_variation=true;
    		shared_data_.measurements_previous.height_ankle=measurements.height_ankle;
    	}else if (measurements.distance_to_heel !=shared_data_.measurements_previous.distance_to_heel){
    		therapy_variation=true;
    		shared_data_.measurements_previous.distance_to_heel=measurements.distance_to_heel;
    	}else if (measurements.distance_to_toe !=shared_data_.measurements_previous.distance_to_toe){
    		therapy_variation=true;
    		shared_data_.measurements_previous.distance_to_toe=measurements.distance_to_toe;
    	}else if (measurements.depth_pelvis !=shared_data_.measurements_previous.depth_pelvis){
    		therapy_variation=true;
    		shared_data_.measurements_previous.depth_pelvis=measurements.depth_pelvis;
    	} else if (measurements.width_pelvis !=shared_data_.measurements_previous.width_pelvis){	
    		therapy_variation=true;
    		shared_data_.measurements_previous.width_pelvis=measurements.width_pelvis;	
    		
    	} else if (measurements.height !=shared_data_.measurements_previous.height){	
    		therapy_variation=true;
    		shared_data_.measurements_previous.height=measurements.height;
    	} else if (measurements.tibia !=shared_data_.measurements_previous.tibia){	
    		therapy_variation=true;
    		shared_data_.measurements_previous.tibia=measurements.tibia;
    	} else if (measurements.femur !=shared_data_.measurements_previous.femur){
    		therapy_variation=true;
    		shared_data_.measurements_previous.femur=measurements.femur;
    	} else if (therapy_requirements.step_length !=shared_data_.therapy_requirements_previous.step_length){
    		therapy_variation=true;
    		shared_data_.therapy_requirements_previous.step_length=therapy_requirements.step_length;
    	} else if (therapy_requirements.step_height !=shared_data_.therapy_requirements_previous.step_height){
    		therapy_variation=true;
    		shared_data_.therapy_requirements_previous.step_height=therapy_requirements.step_height;
    	}
    	
    return therapy_variation;
	
    }
    
     
    //Funcion para solicitar el servicio de cálculo de trayectorias
    
    void call_TrajGenerationService(const nimble_interfaces::msg::Measurements & measurements, const 			nimble_interfaces::msg::TherapyRequirements & requirements) {
    
       	RCLCPP_INFO(this->get_logger(), "Sending trajectory request");    
        // Create a request to send to the service
        auto request = std::make_shared<nimble_interfaces::srv::TrajGeneratorService::Request>();
        // Populate the request fields as needed
	request->measurements=measurements;
	request->therapy_requirements=requirements;
	
	while (!client_->wait_for_service(1s)) {
	    if (!rclcpp::ok()) {
	      RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "Interrupted while waiting for the service. Exiting.");
	      //return 0;
	    }
	    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "service not available, waiting again...");
	  }
	
        // Call the service using the client
        auto result = client_->async_send_request(request);
        
        
        // Wait for the result
        // Do this instead of rclcpp::spin_until_future_complete()
        std::future_status status = result.wait_for(10s);  // timeout to guarantee a graceful finish
        if (status == std::future_status::ready) {
            RCLCPP_INFO(this->get_logger(), "Received response");
            shared_data_.joints_trajectory = result.get()->joints_trajectory;
        }
    }
    
    //Callbacks, funciones asociadas a la recepcion de cada topic 
     
    void call_back_joints_acc_state(const nimble_interfaces::msg::JointsTrajectory & joints_acc_state_msg) 
    {
        shared_data_.joints_acc_state = joints_acc_state_msg;  //almacenamiento del mensaje en la estructura de datos
        processData();   //llamada a la funcion de procesamiento
    }
    
    void call_back_cartesian_trajectory(const nimble_interfaces::msg::CartesianTrajectory & cartesian_trajectory_msg) 
    {
        shared_data_.cartesian_trajectory = cartesian_trajectory_msg;
        processData();
    }
    void call_back_cartesian_state(const nimble_interfaces::msg::CartesianTrajectory & cartesian_state_msg) 
    {
        shared_data_.cartesian_state = cartesian_state_msg;
        processData();
    }
    
    void call_back_state_cables(const sensor_msgs::msg::JointState & joint_state_cables_msg) 
    {
        shared_data_.cables_state = joint_state_cables_msg;
        processData();
    }
    
    void call_back_step_target(const nimble_interfaces::msg::TherapyRequirements & step_target_msg) 
    {
        shared_data_.step_target = step_target_msg;
        processData();
    }
    
    void call_back_therapy_requirements(const nimble_interfaces::msg::TherapyRequirements & therapy_requirements_msg) 
    {
        shared_data_.therapy_requirements = therapy_requirements_msg;
        processData();
    }
    
    
    void call_back_measurements(const nimble_interfaces::msg::Measurements & measurements_msg) 
    {
        shared_data_.measurements = measurements_msg;
        processData();
    }
    
    void call_back_interaction_torque(const std_msgs::msg::Float32MultiArray & interaction_torque_msg) 
    {
        shared_data_.interaction_torque = interaction_torque_msg;
        processData();
    }
    
    void call_back_FSR(const std_msgs::msg::ByteMultiArray & FSR_msg) 
    {
        shared_data_.FSR = FSR_msg;
        processData();
    }
    
    
    
    //Funcion para procesamiento  
    void processData() {
        // Perform your logic using the shared_data_
        // ...aquí quizás habría que hacer distintas funciones de procesamiento y comprobar que existen datos de todo lo necesario con un if
        
        //Comprobar si hay modificaciones en los parametros de la terapia o el paciente
        bool therapy_variation= check_variations_intherapy(shared_data_.measurements, shared_data_.therapy_requirements);
        if (therapy_variation==true) {
        	//Si los hay solicitar cálculo de trayectoria ideal
                call_TrajGenerationService(shared_data_.measurements, shared_data_.therapy_requirements); 
        }
   }
   
    // Callback ejecutado cada cierto tiempo (eliminar si no se usa)
    void timer_callback()
  {
     	 auto joints_trajectory_msg = nimble_interfaces::msg::JointsTrajectory();   //declarar mensaje con el tipo necesario
      	 joints_trajectory_msg=shared_data_.joints_trajectory;   //Rellenar con la información correspondiente
         joints_trajectory_msg.header.stamp=now();   //header con el momento de publicacion
    	 publisher_joints_trajectory->publish(joints_trajectory_msg);  //publicar
    
  } 
    
    
};

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<StatesMachineNode>();
    rclcpp::executors::MultiThreadedExecutor executor;
    executor.add_node(node);

    //RCLCPP_INFO(node->get_logger(), "Starting states machine node, shut down with CTRL-C");
    executor.spin();
    //RCLCPP_INFO(node->get_logger(), "Keyboard interrupt, shutting down.\n");

    rclcpp::shutdown();
    return 0;
}

