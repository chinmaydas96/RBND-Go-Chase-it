#include "ros/ros.h"
#include "ball_chaser/DriveToTarget.h"
#include <sensor_msgs/Image.h>

// Define a global client that can request services
ros::ServiceClient client;

// This function calls the command_robot service to drive the robot in the specified direction
void drive_robot(float lin_x, float ang_z)
{
    // Requests a service and pass the velocities to it to drive the robot
    ball_chaser::DriveToTarget srv;
    srv.request.linear_x = lin_x;
    srv.request.angular_z = ang_z;
    if (!client.call(srv)) {
    ROS_ERROR("Failed to call service drive_bot!");
    }
}

// This callback function continuously executes and reads the image data
void process_image_callback(const sensor_msgs::Image img)
{
  int white_pixel = 255;
  // Loops through each pixel in the image and checks if there's a bright white one
    
  int step = -1; 
  int ball_detected = 0; 
  for (int i = 0; i < img.height * img.step; i++) {
    if (img.data[i] == white_pixel && img.data[i+1] == white_pixel  && img.data[i+2] == white_pixel  ) {
      step  = i % img.step;
      ball_detected = 1;
      break;
    }
   else{
       ball_detected = 0;
    }
    
  }
   // Identifies if this pixel falls in the left, mid, or right side of the image
  // Depending on the white ball position, calls the drive_bot function and pass velocities to it
  if (ball_detected==1){
    if (step  <= img.step * 0.3 && step  >= 0) { // Left area
    drive_robot(0,0.5);
  } else if (step  > img.step *0.7  && step  <= img.step) { // Right area
    drive_robot(0,-0.5);
  } else if (step  != -1) { // Forwared area
    drive_robot(0.5,0);
  }

  }
  // Requests a stop when there's no white ball seen by the camera
  else {
    drive_robot(0,0);
  }

             
}

int main(int argc, char** argv)
{
    // Initialize the process_image node and create a handle to it
    ros::init(argc, argv, "process_image");
    ros::NodeHandle n;

    // Define a client service capable of requesting services from command_robot
    client = n.serviceClient<ball_chaser::DriveToTarget>("/ball_chaser/command_robot");

    // Subscribe to /camera/rgb/image_raw topic to read the image data inside the process_image_callback function
    ros::Subscriber sub1 = n.subscribe("/camera/rgb/image_raw", 10, process_image_callback);

    // Handle ROS communication events
    ros::spin();

    return 0;
}