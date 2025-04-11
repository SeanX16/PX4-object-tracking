// Initialization
target_altitude = 2.5        // meters
takeoff = false
// Setup publishers: position setpoints (20Hz) to /mavros/setpoint_position/local
// Setup subscribers: state from /mavros/state (mode, arming status)

// Wait for connection
while (not connected):
    process ROS callbacks

// Mode switching and arming loop
while (not takeoff):
    publish position setpoint (x=0, y=0, z=target_altitude) at 20Hz

    if (flight_mode != "OFFBOARD"):
        request mode switch via /mavros/set_mode
    if (not armed):
        request arming via /mavros/cmd/arming

    if (flight_mode == "OFFBOARD" and armed):
        // UAV takes off and hovers steadily using position control (PID ensures stability)
        takeoff = true

// Transition to autonomous tracking
while (tracking):
    publish velocity commands for target tracking;




if (findCar) {
    ROS_INFO("Target found at: x = %d, y = %d", x, y);

    // Normalize offset between target and image center
    double vx = abs(centX - x) / centX;
    double vy = abs(centY - y) / centY;

    // Set x and y velocities based on target position
    velocity.linear.x = (x < centX) ? vx : -vx;
    velocity.linear.y = (y < centY) ? vy : -vy;

    // Adjust z velocity to maintain altitude h
    if (curH < h - 0.5)
        velocity.linear.z = hv;
    else if (curH < h + 0.5)
        velocity.linear.z = 0;
    else
        velocity.linear.z = (curH - h) * -hv;

    ROS_INFO("Publishing velocity: v_x=%f, v_y=%f, v_z=%f",
             velocity.linear.x, velocity.linear.y, velocity.linear.z);
}    