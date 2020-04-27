#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "ball_chaser/DriveToTarget.h"
//TODO: Include the ball_chaser "DriveToTarget" header file

// ROS::Publisher motor commands;
ros::Publisher motor_command_publisher;

// Created a handle_drive_request callback function that executes whenever a drive_bot service is requested
// This function publishes the requested linear x and angular velocities to the robot wheel joints
// After publishing the requested velocities, a message feedback is returned with the requested wheel velocities
bool handle_drive_request(ball_chaser::DriveToTarget::Request& req, ball_chaser::DriveToTarget::Response& res){    
    // Create a motor_command object of type geometry_msgs::Twist
    geometry_msgs::Twist motor_command;

    ROS_INFO("DriveToTargetRequest received - linear_x:%1.2f, angular:%1.2f", (float)req.linear_x, (float)req.angular_z);
    //Motor command object set with requested values
    motor_command.linear.x = req.linear_x;
    motor_command.angular.z = req.angular_z;

    // Publishing requested linear x and angular velocities to the robot wheel joints
    motor_command_publisher.publish(motor_command);

    // Returning a message feedback
    res.msg_feedback = "Motor Command is Set - linear.x: "+ std::to_string(req.linear_x) + " angular.z: " + std::to_string(req.angular_z);
    ROS_INFO_STREAM(res.msg_feedback);

    return true;
};

int main(int argc, char** argv)
{
    // Initialize a ROS node
    ros::init(argc, argv, "drive_bot");

    // Create a ROS NodeHandle object
    ros::NodeHandle n;

    // Inform ROS master that we will be publishing a message of type geometry_msgs::Twist on the robot actuation topic with a publishing queue size of 10
    motor_command_publisher = n.advertise<geometry_msgs::Twist>("/cmd_vel", 10);

    // Defined a drive /ball_chaser/command_robot service with a handle_drive_request callback function
    ros::ServiceServer drive_bot_srv = n.advertiseService("/ball_chaser/command_robot", handle_drive_request);
    ROS_INFO("Ready to send motor commands");

    // TODO: Handle ROS communication events
    ros::spin();

    return 0;
}