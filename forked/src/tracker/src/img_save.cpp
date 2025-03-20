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


void doImg(const sensor_msgs::Image::ConstPtr &msg) {
    cv_bridge::CvImagePtr cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
	cv::Mat img = cv_ptr -> image;
    int cnt = 0;
    cv::imwrite(std::to_string(cnt) + ".jpg", img);
    ROS_INFO("%d", cnt);
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "img_save");
    ros::NodeHandle nh;
    setlocale(LC_ALL, "");

    ros::Subscriber img_sub = nh.subscribe<sensor_msgs::Image>("/camera/rgb/image_raw", 10, doImg);



    while(ros::ok()){
        ros::spinOnce();
    }

    return 0;
}
