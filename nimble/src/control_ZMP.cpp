#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "nimble_interfaces/msg/therapy_requirements.hpp"
#include "nimble_interfaces/msg/cartesian_trajectory.hpp"
#include "trajectory_msgs/msg/joint_trajectory.hpp"
#include "nimble_interfaces/msg/frame_state.hpp"
#include "sensor_msgs/msg/joint_state.hpp"
#include "std_msgs/msg/byte_multi_array.hpp"


using namespace std::chrono_literals;

//Estructura compartida para almacenar datos al recibir cada topic
struct SharedData {

    nimble_interfaces::msg::CartesianTrajectory cart_target;
    nimble_interfaces::msg::CartesianTrajectory cart_state;
    nimble_interfaces::msg::TherapyRequirements step_target;
    nimble_interfaces::msg::FrameState frame_state;
    sensor_msgs::msg::JointState cables_state;
    std_msgs::msg::ByteMultiArray FSR;

};

//Nodo
class ZMPControlNode : public rclcpp::Node
{
public:
    ZMPControlNode() : Node("control_ZMP")
    {	
    	//Possible internal parameters
        this->declare_parameter("param", 1);
    	this->declare_parameter("param2", 2);
    	
        // Create a subscribers 
        subscriber_cartTarget = create_subscription<nimble_interfaces::msg::CartesianTrajectory>(
            "cartesian_target", 10,
            [this](const nimble_interfaces::msg::CartesianTrajectory msg) {
                // Callback function
                call_back_carTarget(msg);
            });
            
        subscriber_cartState = create_subscription<nimble_interfaces::msg::CartesianTrajectory>(
            "cartesian_target", 10,
            [this](const nimble_interfaces::msg::CartesianTrajectory msg) {
                // Callback function
                call_back_carState(msg);
            });    
            
        subscriber_stepTarget = create_subscription<nimble_interfaces::msg::TherapyRequirements>(
            "step_target", 10,
            [this](const nimble_interfaces::msg::TherapyRequirements msg) {
                // Callback function
                call_back_stepTarget(msg);
            });    
               
        subscriber_framestate = create_subscription<nimble_interfaces::msg::FrameState>(
            "frame_state", 10,
            [this](const nimble_interfaces::msg::FrameState msg) {
                // Callback function
                call_back_framestate(msg);
            });   
            
        subscriber_cablestate = create_subscription<sensor_msgs::msg::JointState>(
            "cables_state", 10,
            [this](const sensor_msgs::msg::JointState msg) {
                // Callback function
                call_back_cablestate(msg);
            });   
            
         suscriber_FSR = create_subscription<std_msgs::msg::ByteMultiArray>(
            "FSR", 10,
            [this](const std_msgs::msg::ByteMultiArray msg) {
                
                call_back_FSR(msg);
            });  
                       
        
        // Create a publishers
        publisher_pelvisSP = create_publisher<sensor_msgs::msg::JointState>("pelvis_SP", 10);
        
        //Create wall timer to publish periodically (eliminar si no se usa)
        timer_ = this->create_wall_timer(1000ms, std::bind(&ZMPControlNode::timer_callback, this));
        
    }

private:
    //Instancias
    SharedData	shared_data_;    //estructura de datos
    rclcpp::Subscription<nimble_interfaces::msg::CartesianTrajectory>::SharedPtr subscriber_cartTarget; //susbcriptores
    rclcpp::Subscription<nimble_interfaces::msg::CartesianTrajectory>::SharedPtr subscriber_cartState;
    rclcpp::Subscription<nimble_interfaces::msg::TherapyRequirements>::SharedPtr subscriber_stepTarget;
    rclcpp::Subscription<nimble_interfaces::msg::FrameState>::SharedPtr subscriber_framestate;
    rclcpp::Subscription<sensor_msgs::msg::JointState>::SharedPtr subscriber_cablestate;
    rclcpp::Subscription<std_msgs::msg::ByteMultiArray>::SharedPtr suscriber_FSR;
    
    rclcpp::Publisher<sensor_msgs::msg::JointState>::SharedPtr publisher_pelvisSP; //publishers
    rclcpp::TimerBase::SharedPtr timer_; //timer (eliminar si no se usa)
    
    
    //Callbacks, funciones asociadas a la recepcion de cada topic        
    void call_back_carTarget(const nimble_interfaces::msg::CartesianTrajectory & cart_target_msg)
    {
	shared_data_.cart_target = cart_target_msg;  //almacenamiento del mensaje en la estructura de datos
        processData();  //llamada a la funcion de procesamiento
        
    }
    
    void call_back_carState(const nimble_interfaces::msg::CartesianTrajectory & cart_state_msg)
    {
	shared_data_.cart_state = cart_state_msg;  //almacenamiento del mensaje en la estructura de datos
        processData();  //llamada a la funcion de procesamiento
        
    }
    
    void call_back_stepTarget(const nimble_interfaces::msg::TherapyRequirements & step_target_msg)
    {
	shared_data_.step_target = step_target_msg;
        processData();
        
    }
    
    void call_back_framestate(const nimble_interfaces::msg::FrameState & frame_state_msg)
    {
	shared_data_.frame_state = frame_state_msg;
        processData();
        
    }
    
    void call_back_cablestate(const sensor_msgs::msg::JointState & cables_state_msg)
    {
	shared_data_.cables_state = cables_state_msg;
        processData();
        
    }
    
    void call_back_FSR(const std_msgs::msg::ByteMultiArray & FSR_msg) 
    {
        shared_data_.FSR = FSR_msg;
        processData();
    }

    //Funcion para procesamiento y publicacion    
    void processData() {
        // Perform your logic using the shared_data_
        
        //Publicacion
        auto pelvisSP_msg = sensor_msgs::msg::JointState();  //declarar mensaje con el tipo necesario
      	 //pelvisSP_msg=.......;     //Rellenar con la información correspondiente
         //pelvisSP_msg.header.stamp=now();  //header con el momento de publicacion
    	 //publisher_pelvisSP->publish(pelvisSP_msg);  //publicar
    	 
    }
    
    // Callback ejecutado cada cierto tiempo (eliminar si no se usa)
      void timer_callback()
  {
     	 
    
  } 
    
};

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<ZMPControlNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}

