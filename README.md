# ECSE373 Group 9 Lab 4

This package prevents the driver from colliding with a wall when driving the robot in the x direction.

This package depends on the following packages:
- stdr_simulation
- std_msgs
- geometry_msgs
- sensor_msgs

To launch this package starting the stdr environment and launching the first robot node, call the following line:

    roslaunch lab4 add_env.launch nr:=<number_of_desired_nodes>

You can add as many robots as you want by doing this.



NOTE: This will start the nodes in the following namespace: robot#. # is the number of robots desired.
Example: If you were to call `roslaunch lab4 add_env.launch nr:=3` it would start the following nodes: robot0, robot1, robot2 in their respective namespaces: robot0, robot1, robot2.
