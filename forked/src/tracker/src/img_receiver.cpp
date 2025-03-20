/**
 * @file offb_node.cpp
 * @brief Offboard control example node, written with MAVROS version 0.19.x, PX4 Pro Flight
 * Stack and tested in Gazebo SITL
 */

#include <ros/ros.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/Twist.h>
#include <mavros_msgs/CommandBool.h>
#include <mavros_msgs/SetMode.h>
#include <mavros_msgs/State.h>
#include <mavros_msgs/Altitude.h>


#include "sensor_msgs/Image.h"
#include "cv_bridge/cv_bridge.h"

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

// 悬空高度
const double h = 4;
// 控制高度的速度
const double hv = 0.1;

geometry_msgs::Twist velocity;

double curH;

bool start = false;

void doImg(const sensor_msgs::Image::ConstPtr &msg) {
    if(!start) return;
    cv_bridge::CvImagePtr cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
	cv::Mat img = cv_ptr -> image;
    cv::Mat gray, bin;
    cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);
    cv::threshold(gray, bin, 127, 255, cv::THRESH_BINARY);
    static int row = bin.rows, col = bin.cols;
    static double centX = row / 2, centY = col / 2;
    int x, y;
    bool findCar = false;
    for(int i = 0; i < row; i++) {
        for(int j = 0; j < col; j++) {
            uchar point = bin.at<uchar>(i, j);
            if(point == 0) {
                findCar = true;
                x = i, y = j;
                break;
            }
        }
    }
    static ros::Time last_find_time = ros::Time::now();
    if(findCar) {
        ROS_INFO("找到目标位置, x = %d, y = %d", x, y);
        double vx = abs(centX - x) / centX;
        double vy = abs(centY - y) / centY;
        // vx = 2 * vx * vx;
        // vy = 2 * vy * vy;
        if(x < centX) velocity.linear.x = vx;
        else velocity.linear.x = -vx;
        

        if(y < centY) velocity.linear.y = vy;
        else velocity.linear.y = -vy;

        if(curH < h - 0.5) velocity.linear.z = hv;
        else if(curH < h + 0.5) velocity.linear.z = 0;
        else velocity.linear.z = (curH - h) * -hv;
        ROS_INFO("发布速度 x : %f, y : %f, z : %f", velocity.linear.x, velocity.linear.y, velocity.linear.z);
        last_find_time = ros::Time::now();
    } else {
        ros::Time now = ros::Time::now();
        velocity.linear.x = 0;
        velocity.linear.y = 0;
        if(now - last_find_time < ros::Duration(5)) {
            ROS_INFO("没有找到目标...");
        } else {
            if(curH < 2 * h - 1) {
                ROS_INFO("上升高度寻找，当前高度为：%.2f", curH);
                velocity.linear.z = hv;
            } else {
                if(curH > 2 * h + 1) velocity.linear.z = -hv;
                else velocity.linear.z = 0;
                ROS_INFO("目标丢失。。。");
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
                    ROS_INFO("开始追踪...");
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
