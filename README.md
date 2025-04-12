# Vision-Based UAV Target Tracking (FYP Project)

This repository contains the full implementation of my final-year project: an autonomous UAV simulation system capable of tracking a moving ground target using only onboard vision. The project was developed and tested in a ROS-Gazebo-PX4 simulation environment.

## Features

- Full UAV simulation using PX4 SITL + Gazebo
- Real-time target detection using OpenCV (grayscale + binary + contour)
- Velocity-based control using MAVROS `/cmd_vel_unstamped`
- OFFBOARD mode takeoff and mode switching
- Simple recovery logic when target is lost (hover + ascend)
- Fully integrated ROS node for closed-loop visual tracking

## Demo Video

> Watch a full demo of the system here:  
> [**YouTube / Drive Link**](https://your-link-here.com)

## System Architecture

![System Diagram](your-diagram.png)

## Requirements

- Ubuntu 18.04 (inside VMware recommended)
- ROS Melodic
- PX4 v1.11.0-beta1
- Gazebo 9
- MAVROS
- OpenCV (via `cv_bridge`)

## Installation & Usage

1. Clone this repo into your ROS workspace:
   ```bash
   cd ~/catkin_ws/src
   git clone https://github.com/your-username/your-repo-name.git
   cd ..
   catkin_make

	2.	Launch PX4 with Gazebo:

source devel/setup.bash
roslaunch px4 mavros_posix_sitl.launch


	3.	Launch TurtleBot3:

roslaunch turtlebot3_gazebo turtlebot3_empty_world.launch


	4.	Run the tracking node:

rosrun your_package_name tracking_node


	5.	Use teleop_twist_keyboard to control the target:

rosrun teleop_twist_keyboard teleop_twist_keyboard.py



Repository Structure

.
├── src/
│   └── tracking_node.cpp         # Main ROS node for image processing and velocity control
├── launch/
│   └── tracking.launch           # Launch file (optional)
├── include/                      # Header files (if any)
├── video/                        # Demonstration video (optional)
└── README.md

Acknowledgements

This project was developed as part of my undergraduate Final Year Project at [Your University Name]. Special thanks to [Supervisor’s Name] for guidance and support, and to the PX4 and ROS communities for their amazing open-source tools.

---

## ✅ 小提醒

- 有图的话记得放进 `README.md` 同级目录，用 `![描述](相对路径)` 插入；
- GitHub 默认渲染 `README.md` 作为首页，不需要特别命名；
- 如果你的视频是 Drive 链接，记得设置成“仅限持链接者可看”。

---

你要是把你 GitHub 仓库地址发我，我可以帮你填完整、改成你的专属定制版，还能加你图里的结构图、视频封面图等内容。要不要我来搞一版完整版 README 给你？😎






# Object Tracking Using UAV

## Description

My final year project repository.

## Environment

- VMware Workstation 16 Player

- Ubuntu 18.04

- ROS Melodic

-------------------

### Reference

[PX4](https://github.com/PX4) , [GAAS](https://github.com/generalized-intelligence/GAAS) , @Anlper
