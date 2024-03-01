#include <chrono>
#include <functional>
#include <memory>
#include <string>


#include "rclcpp/rclcpp.hpp"
#include "nimble_interfaces/msg/measurements.hpp"
#include "nimble_interfaces/msg/therapy_requirements.hpp"
#include "nimble_interfaces/srv/traj_generator_service.hpp"
#include "nimble_interfaces/msg/joints_trajectory.hpp"
#include <memory>

// Function to convert a JointTrajectory message to a string
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

void generate_trajectory(const std::shared_ptr<nimble_interfaces::srv::TrajGeneratorService::Request> request,
          std::shared_ptr<nimble_interfaces::srv::TrajGeneratorService::Response>      response)
{   float femur=request->measurements.femur;
    float tibia=request->measurements.tibia;	
    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Incoming request\nfemur: %f" " tibia: %f",
                femur, tibia);
   
  // Create a JointTrajectory message
    //std::shared_ptr<trajectory_msgs::msg::JointTrajectory> joint_trajectory;  
    auto joints_trajectory = std::make_shared<nimble_interfaces::msg::JointsTrajectory>();
    
    
    // Set the joint names
    joints_trajectory->joint_trajectory.joint_names = {"hipR", "kneeR", "ankleR","hipL", "kneeL", "ankleL"};

    // Create two trajectory points
    trajectory_msgs::msg::JointTrajectoryPoint point1;
    trajectory_msgs::msg::JointTrajectoryPoint point2;

    // Set positions for each joint in the first point
    point1.positions = {femur, femur, femur,tibia,tibia,tibia};

    // Set time for the first point (e.g., 0.0 seconds)
    point1.time_from_start = rclcpp::Duration(0, 0);

    // Set positions for each joint in the second point
    point2.positions = {1.0,2.0,3.0,4.0, 5.0, 6.0};

    // Set time for the second point (e.g., 1.0 second)
    point2.time_from_start = rclcpp::Duration(1, 0);

    // Add the two points to the trajectory
    joints_trajectory->joint_trajectory.points.push_back(point1);
    joints_trajectory->joint_trajectory.points.push_back(point2);
    
    
    RCLCPP_INFO(rclcpp::get_logger("joint_trajectory_publisher"), "JointTrajectory message:\n%s",
        jointTrajectoryToString(joints_trajectory).c_str());

    response->joints_trajectory = *joints_trajectory;                 
  
}

int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);

  std::shared_ptr<rclcpp::Node> node = rclcpp::Node::make_shared("traj_generator_service");

  rclcpp::Service<nimble_interfaces::srv::TrajGeneratorService>::SharedPtr service =
    node->create_service<nimble_interfaces::srv::TrajGeneratorService>("traj_generator_service", &generate_trajectory);

  RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Trajectory Generator Service: Ready.");

  rclcpp::spin(node);
  rclcpp::shutdown();
}

