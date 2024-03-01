#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "nimble_interfaces/msg/therapy_requirements.hpp"
#include "nimble_interfaces/msg/cartesian_trajectory.hpp"
#include "nimble_interfaces/msg/joints_trajectory.hpp"

using namespace std::chrono_literals;

//Estructura compartida para almacenar datos al recibir cada topic
struct SharedData {

    nimble_interfaces::msg::JointsTrajectory joints_trajectory;
    nimble_interfaces::msg::CartesianTrajectory cartesian_trajectory;
    nimble_interfaces::msg::TherapyRequirements therapy_requirements;
    nimble_interfaces::msg::TherapyRequirements step_target;

};

//Nodo
class GaitControlNode : public rclcpp::Node
{
public:
    GaitControlNode() : Node("gait_control")
    {	
    	//Possible internal parameters
        this->declare_parameter("param1", 1);
    	this->declare_parameter("param2", 2);
    	
        // Create a subscribers 
        subscriber_joints_trajectory = create_subscription<nimble_interfaces::msg::JointsTrajectory>(
            "joints_trajectory", 10,
            [this](const nimble_interfaces::msg::JointsTrajectory msg) {
                // Callback function that publishes the received Int64 message
                call_back_joints_trajectory(msg);
            });
            
        subscriber_cartesian_trajectory = create_subscription<nimble_interfaces::msg::CartesianTrajectory>(
            "cartesian_trajectory", 10,
            [this](const nimble_interfaces::msg::CartesianTrajectory msg) {
                // Callback function that publishes the received Int64 message
                call_back_cartesian_trajectory(msg);
            });    
            
        subscriber_step_target = create_subscription<nimble_interfaces::msg::TherapyRequirements>(
            "step_target", 10,
            [this](const nimble_interfaces::msg::TherapyRequirements msg) {
                // Callback function that publishes the received Int64 message
                call_back_step_target(msg);
            }); 
            
        subscriber_therapy_requirements = create_subscription<nimble_interfaces::msg::TherapyRequirements>(
            "therapy_requirements", 10,
            [this](const nimble_interfaces::msg::TherapyRequirements msg) {
                
                call_back_therapy_requirements(msg);
            });
                           
        
        // Create a publishers
        publisher_cartTarget = create_publisher<nimble_interfaces::msg::CartesianTrajectory>("cartesian_target", 10);
        publisher_jointsTarget = create_publisher<nimble_interfaces::msg::JointsTrajectory>("joints_trajectory", 10);
        //Create wall timer to publish periodically (eliminar si no se usa)
        timer_ = this->create_wall_timer(1000ms, std::bind(&GaitControlNode::timer_callback, this)); 
    }

private:
    //Instancias	
    SharedData	shared_data_;    //estructura de datos
    //Subscribers	
    rclcpp::Subscription<nimble_interfaces::msg::JointsTrajectory>::SharedPtr subscriber_joints_trajectory;
    rclcpp::Subscription<nimble_interfaces::msg::CartesianTrajectory>::SharedPtr subscriber_cartesian_trajectory;
    rclcpp::Subscription<nimble_interfaces::msg::TherapyRequirements>::SharedPtr subscriber_therapy_requirements;
    rclcpp::Subscription<nimble_interfaces::msg::TherapyRequirements>::SharedPtr subscriber_step_target;
    
    //Publishers
    rclcpp::Publisher<nimble_interfaces::msg::CartesianTrajectory>::SharedPtr publisher_cartTarget;
    rclcpp::Publisher<nimble_interfaces::msg::JointsTrajectory>::SharedPtr publisher_jointsTarget;   
    rclcpp::TimerBase::SharedPtr timer_; //timer (eliminar si no se usa)
    
    
    //Callbacks, funciones asociadas a la recepcion de cada topic
    	        
    void call_back_joints_trajectory(const nimble_interfaces::msg::JointsTrajectory & joint_trajectory_msg) 
    {
        shared_data_.joints_trajectory = joint_trajectory_msg;  //almacenamiento del mensaje en la estructura de datos
        processData();  //llamada a la funcion de procesamiento
        
    }
    
    void call_back_step_target(const nimble_interfaces::msg::TherapyRequirements & step_target_msg) 
    {
        shared_data_.step_target = step_target_msg;  //almacenamiento del mensaje en la estructura de datos
        processData();  //llamada a la funcion de procesamiento
        
    }
    
    
    void call_back_cartesian_trajectory(const nimble_interfaces::msg::CartesianTrajectory & cartesian_trajectory_msg) 
    {
        shared_data_.cartesian_trajectory = cartesian_trajectory_msg;  //almacenamiento del mensaje en la estructura de datos
        processData();  //llamada a la funcion de procesamiento
        
    }
    
    void call_back_therapy_requirements(const nimble_interfaces::msg::TherapyRequirements & therapy_requirements_msg) 
    {
        shared_data_.therapy_requirements = therapy_requirements_msg;  //almacenamiento del mensaje en la estructura de datos
        processData();  //llamada a la funcion de procesamiento
        
    }
    
    void processData() {
        // Perform your logic using the shared_data_
 	    	 
    }
    
        // Callback ejecutado cada cierto tiempo (eliminar si no se usa)
      void timer_callback()
  {
 
    
  }    
    
    
};

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<GaitControlNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}

