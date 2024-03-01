from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
        Node(
            package="nimble",
            executable="hmi",
            name="hmi",
            output="screen",
            emulate_tty=True,
            parameters=[
                {"height": 1.8},
                {"femur": 0.42},
                {"tibia": 0.35},
                {"distance_to_heel": 0.10},
                {"distance_to_toe": 0.2},
                {"height_ankle": 0.1},
                {"depth_pelvis": 0.15},
                {"width_pelvis": 0.4},
                {"speed": 0.3},
                {"min_assist_level": 10},
                {"max_assist_level": 50},
                {"step_length": 0.6},
                {"step_height": 0.12},
                
            ]
        ),
        Node(
            package="nimble",
            executable="traj_generator_server",
            name="traj_generator_server",
            output="screen",
            emulate_tty=True,
            
        ),
        Node(
            package="nimble",
            executable="states_machine",
            name="states_machine",
            output="screen",
            emulate_tty=True,
            
        ),
        Node(
            package="nimble",
            executable="kinematic_model",
            name="kinematic_model",
            output="screen",
            emulate_tty=True,
            
        ),
        Node(
            package="nimble",
            executable="gait_control",
            name="gait_control",
            output="screen",
            emulate_tty=True,
            
        ),        
        Node(
            package="nimble",
            executable="control_MSD",
            name="control_MSD",
            output="screen",
            emulate_tty=True,
            
        ),
        Node(
            package="nimble",
            executable="control_ZMP",
            name="control_ZMP",
            output="screen",
            emulate_tty=True,
            
        )
    ])
