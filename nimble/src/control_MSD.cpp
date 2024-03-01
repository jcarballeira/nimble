#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/int64.hpp"
#include "std_msgs/msg/float32_multi_array.hpp"
#include "std_msgs/msg/byte_multi_array.hpp"
#include "nimble_interfaces/msg/joints_trajectory.hpp"
#include "sensor_msgs/msg/joint_state.hpp"


using namespace std::chrono_literals;

//Estructura compartida para almacenar datos al recibir cada topic
struct SharedData {

    nimble_interfaces::msg::JointsTrajectory joints_target;
    sensor_msgs::msg::JointState joints_state;
    std_msgs::msg::Int64 assist_level;
    std_msgs::msg::Float32MultiArray interaction_torque;
    std_msgs::msg::ByteMultiArray FSR;
    // Add more fields as needed
    std::mutex mutex;  // Mutex for thread-safe access
};

//Nodo
class MSDControlNode : public rclcpp::Node
{
public:
    MSDControlNode() : Node("control_MSD")
    {	
    	//Possible internal parameters
        this->declare_parameter("contact_point", 1);
    	this->declare_parameter("param2", 2);
    	
        // Create a subscribers 
        subscriber_jointsTarget = create_subscription<nimble_interfaces::msg::JointsTrajectory>(
            "joints_target", 10,
            [this](const nimble_interfaces::msg::JointsTrajectory msg) {
                call_back_jointsTarget(msg);
            });
            
        subscriber_jointsState = create_subscription<sensor_msgs::msg::JointState>(
            "joints_state", 10,
            [this](const sensor_msgs::msg::JointState msg) {
                call_back_jointsState(msg);
            }); 
        subscriber_assist_level = create_subscription<std_msgs::msg::Int64>(
            "assistLevel", 10,
            [this](const std_msgs::msg::Int64 msg) {
                
                call_back_assist_level(msg);
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
            
                
        // Create a publisher
        publisher_jointsSP = create_publisher<sensor_msgs::msg::JointState>("joints_SP", 10);
        
        //Create wall timer to publish periodically (eliminar si no se usa)
        timer_ = this->create_wall_timer(1000ms, std::bind(&MSDControlNode::timer_callback, this)); 
        
    }

private:

    //Instancias
    SharedData	shared_data_; //esteuctura de datos
    rclcpp::Subscription<nimble_interfaces::msg::JointsTrajectory>::SharedPtr subscriber_jointsTarget; //Subscriptores
    rclcpp::Subscription<sensor_msgs::msg::JointState>::SharedPtr subscriber_jointsState;
    rclcpp::Subscription<std_msgs::msg::Int64>::SharedPtr subscriber_assist_level;
    rclcpp::Subscription<std_msgs::msg::Float32MultiArray>::SharedPtr suscriber_interaction_torque;
    rclcpp::Subscription<std_msgs::msg::ByteMultiArray>::SharedPtr suscriber_FSR;
        
    rclcpp::Publisher<sensor_msgs::msg::JointState>::SharedPtr publisher_jointsSP;//publishers
    rclcpp::TimerBase::SharedPtr timer_; //timer (eliminar si no se usa)
   
    //Callbacks, funciones asociadas a la recepcion de cada topic   

    void call_back_jointsTarget(const nimble_interfaces::msg::JointsTrajectory & joints_target_msg) 
    {
        shared_data_.joints_target = joints_target_msg; //almacenamiento del mensaje en la estructira de datos
        processData(); //llamada a la funcion de procesameinto
        
    }
    
    void call_back_jointsState(const sensor_msgs::msg::JointState & joints_state_msg) 
    {
        shared_data_.joints_state = joints_state_msg;
        processData();
    }
    
    void call_back_assist_level(const std_msgs::msg::Int64 & assist_level_msg) 
    {
        shared_data_.assist_level = assist_level_msg;
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
                
        //Publicacion
        auto jointsSP_msg = sensor_msgs::msg::JointState();  //declarar mensaje con el tipo necesario
      	 //jointsSP_msg=.......;   //Rellenar con la información correspondiente
         //jointsSP_msg.header.stamp=now();   //header con el momento de publicacion
    	 //publisher_jointsSP->publish(jointsSP_msg);  //publicar
    
      
    }
    
    // Callback ejecutado cada cierto tiempo (eliminar si no se usa)
      void timer_callback()
  {
     	 
  } 
    
};

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<MSDControlNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}

