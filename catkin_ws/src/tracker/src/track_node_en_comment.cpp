#include <ros/ros.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/Twist.h>
#include <mavros_msgs/CommandBool.h>
#include <mavros_msgs/SetMode.h>
#include <mavros_msgs/State.h>
#include <mavros_msgs/Altitude.h>


#include "sensor_msgs/Image.h"
#include "cv_bridge/cv_bridge.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
// Some global variables

// Hovering altitude (the height at which the drone tracks the car)
const double h = 4;
// Speed of adjusting altitude (ascending or descending)
const double hv = 0.1;

// Drone velocity control (published to /mavros/setpoint_velocity/cmd_vel_unstamped)
geometry_msgs::Twist velocity;

// Current drone altitude (subscribed from /mavros/altitude)
double curH;

// Flag indicating whether the drone is stably in the air
bool start = false;

// Image callback function: processes camera images and calculates velocity commands for tracking
void doImg(const sensor_msgs::Image::ConstPtr &msg) {

    if(!start) return;

    // Convert the image published by the drone to BGR8 format, then to grayscale, then binarize.
    // This gives a black and white image. If the car appears, there will be black pixels in the image.
    // Otherwise, the image is all white. That's why the car was changed to black — other colors are harder to separate.
    cv_bridge::CvImagePtr cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
	cv::Mat img = cv_ptr -> image;
    cv::Mat gray, bin;
    cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);
    cv::threshold(gray, bin, 127, 255, cv::THRESH_BINARY);

    // Get image width and height
    static int row = bin.rows, col = bin.cols;
    // Image center position. We assume the center is the drone position,
    // so we can easily publish velocity commands to control the drone
    static double centX = row / 2, centY = col / 2;

    // x, y record the detected position of the car in the current frame
    int x, y;
    // Flag indicating whether the car is found
    bool findCar = false;

    // Traverse the image. If there are black pixels, it means the car is detected.
    // Record the x, y position.
    for(int i = 0; i < row; i++) {
        for(int j = 0; j < col; j++) {
            uchar point = bin.at<uchar>(i, j);
            if(point == 0) {
                findCar = true;
                x = i, y = j;
                break;
            }
        }
        if(findCar) break;
    }

    // Record the last time the car was detected
    static ros::Time last_find_time = ros::Time::now();
    if(findCar) {
        ROS_INFO("Target found at: x = %d, y = %d", x, y);
        // Normalize the car's position relative to the drone (image center) to a 0 ~ 1 range
        // and use it to control drone speed. The farther the car, the faster the drone moves.
        double vx = abs(centX - x) / centX;
        double vy = abs(centY - y) / centY;

        // After testing, the image vertical direction corresponds to the drone's x-axis,
        // and the image horizontal direction to the drone's y-axis.
        // So if the car is above the drone (in pixel position), send a positive x velocity, otherwise negative.
        if(x < centX) velocity.linear.x = vx;
        else velocity.linear.x = -vx;

	// Same for y direction
        if(y < centY) velocity.linear.y = vy;
        else velocity.linear.y = -vy;

        // Without sending z-axis velocity, the drone will go up and down randomly.
        // Use the code below to control altitude: if lower than threshold, send positive z velocity;
        // if higher, send negative z velocity to descend.
        if(curH < h - 0.5) velocity.linear.z = hv;
        else if(curH < h + 0.5) velocity.linear.z = 0;
        else velocity.linear.z = (curH - h) * -hv;
        ROS_INFO("Publishing velocity: v_x = %f, v_y = %f, v_z = %f", velocity.linear.x, velocity.linear.y, velocity.linear.z);
        // Record the last time the drone detected the car, useful later
        last_find_time = ros::Time::now();
    } else {
        ros::Time now = ros::Time::now();
        velocity.linear.x = 0;
        velocity.linear.y = 0;
        // Do nothing within 5 seconds after losing the target
        if(now - last_find_time < ros::Duration(5)) {
            ROS_INFO("Target not found...");
        } else {
            // If the target is lost for more than 5 seconds, start ascending (to widen view) to search for the car
            // The maximum altitude is twice the tracking altitude
            // If the car is found while ascending, the drone will immediately descend to follow it
            if(curH < 2 * h - 1) {
                ROS_INFO("Ascending to search, current altitude: %.2f", curH);
                velocity.linear.z = hv;
            } else {
                if(curH > 2 * h + 1) velocity.linear.z = -hv;
                else velocity.linear.z = 0;
                ROS_INFO("Target lost...");
            }
        }
    }
}


void do_H(const mavros_msgs::Altitude::ConstPtr& msg) {
    curH = msg->local;
}

mavros_msgs::State current_state;
void state_cb(const mavros_msgs::State::ConstPtr& msg){
    current_state = *msg;
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "offb_node");
    ros::NodeHandle nh;
    setlocale(LC_ALL, "");

    ros::Subscriber state_sub = nh.subscribe<mavros_msgs::State>
            ("mavros/state", 10, state_cb);
    ros::Publisher local_pos_pub = nh.advertise<geometry_msgs::PoseStamped>
            ("mavros/setpoint_position/local", 10);
    ros::Publisher local_vec_pub = nh.advertise<geometry_msgs::Twist>
            ("/mavros/setpoint_velocity/cmd_vel_unstamped", 10);
    ros::ServiceClient arming_client = nh.serviceClient<mavros_msgs::CommandBool>
            ("mavros/cmd/arming");
    ros::ServiceClient set_mode_client = nh.serviceClient<mavros_msgs::SetMode>
            ("mavros/set_mode");
    ros::Subscriber img_sub = nh.subscribe<sensor_msgs::Image>("/camera/rgb/image_raw", 10, doImg);

    ros::Subscriber height_sub = nh.subscribe<mavros_msgs::Altitude>
            ("/mavros/altitude", 10, do_H);

    //the setpoint publishing rate MUST be faster than 2Hz
    ros::Rate rate(20.0);

    // wait for FCU connection
    while(ros::ok() && !current_state.connected){
        ros::spinOnce();
        rate.sleep();
    }

    geometry_msgs::PoseStamped pose;
    pose.pose.position.x = 0;
    pose.pose.position.y = 0;
    pose.pose.position.z = h;

    velocity.linear.x = 0;
    velocity.linear.y = 0;
    velocity.linear.z = 0;

    mavros_msgs::SetMode offb_set_mode;
    offb_set_mode.request.custom_mode = "OFFBOARD";

    mavros_msgs::CommandBool arm_cmd;
    arm_cmd.request.value = true;

    ros::Time last_request = ros::Time::now();

    bool takeoff = false;

    while(ros::ok()){
        if(!takeoff) {
            if( current_state.mode != "OFFBOARD" &&
                (ros::Time::now() - last_request > ros::Duration(2.0))){
                if( set_mode_client.call(offb_set_mode) &&
                    offb_set_mode.response.mode_sent){
                    ROS_INFO("Offboard enabled");
                }
                last_request = ros::Time::now();
            }

            if( !current_state.armed &&
                (ros::Time::now() - last_request > ros::Duration(2.0))){
                if( arming_client.call(arm_cmd) &&
                    arm_cmd.response.success){
                    ROS_INFO("Vehicle armed");
                }
                last_request = ros::Time::now();
            }

            if( current_state.armed && 
                (ros::Time::now() - last_request > ros::Duration(5.0))) {
                    takeoff = true;
                    ROS_INFO("Vehicle stabled");
                    start = true;
                    ROS_INFO("Start tracking...");
                    last_request = ros::Time::now();
                }

            local_pos_pub.publish(pose);

        } else {
            local_vec_pub.publish(velocity);
        }

        ros::spinOnce();
        rate.sleep();
    }

    return 0;
}
