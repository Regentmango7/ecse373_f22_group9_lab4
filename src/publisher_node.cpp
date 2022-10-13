#include "ros/ros.h"
#include "std_msgs/String.h"
#include "geometry_msgs/Twist.h"
#include "sensor_msgs/LaserScan.h"
#include <sstream>

// Create a global to hold a pointer to the Publisher
ros::Publisher *p_cmd_pub;
std::string ns = ros::this_node::getName();

bool laser = false;
bool too_close = false;
bool moving_forward = true;
bool is_cur_msg = false;
geometry_msgs::Twist cur_msg;
bool stopped = false;
float previous_x = 0;

void stop_moving(){
 
    if(too_close && moving_forward){
        
        cur_msg.linear.x = 0.0;
        p_cmd_pub->publish(cur_msg);
        if (!stopped) {ROS_WARN("ROBOT HAS BEEN STOPPED FOR BEING TOO CLOSE TO THE WALL, BACK AWAY OR ROTATE TO CONTINUE MOVING");}
        stopped = true;
    }else{
        cur_msg.linear.x = previous_x;
        p_cmd_pub->publish(cur_msg);
        if (stopped){ROS_INFO("Robot is clear of any walls and can move forward.");}
        stopped = false;
    }
}

void des_vel_Callback(const geometry_msgs::Twist::ConstPtr& msg)
{
    if(msg->linear.x > 0){
        moving_forward = true;
    }else if(msg->linear.x <= 0){
        moving_forward = false;
    }
    
    previous_x = msg->linear.x;
    
    is_cur_msg = true;
    cur_msg.linear.x = msg->linear.x;
    cur_msg.linear.y = msg->linear.y;
    cur_msg.linear.z = msg->linear.z;
    cur_msg.angular.x = msg->angular.x;
    cur_msg.angular.y = msg->angular.y;
    cur_msg.angular.z = msg->angular.z;
    
    stop_moving();
}

void laserScanCallback(const sensor_msgs::LaserScan::ConstPtr& msg)
{
    laser = true;
    bool check = false;
    for(int i = 90; i < 180; i++){
        if(msg->ranges[i] <= 1){
            too_close = true;
            check = true;
        }
    }
    
    if(check == false){
        too_close = false;
    }
    
    stop_moving();
    
}

/**
 * This tutorial demonstrates simple sending of messages over the ROS system.
 */
int main(int argc, char **argv)
{
  /**
   * The ros::init() function needs to see argc and argv so that it can perform
   * any ROS arguments and name remapping that were provided at the command line.
   * For programmatic remappings you can use a different version of init() which takes
   * remappings directly, but for most command-line programs, passing argc and argv is
   * the easiest way to do it.  The third argument to init() is the name of the node.
   *
   * You must call one of the versions of ros::init() before using any other
   * part of the ROS system.
   */
  ros::init(argc, argv, "talker");

  /**
   * NodeHandle is the main access point to communications with the ROS system.
   * The first NodeHandle constructed will fully initialize this node, and the last
   * NodeHandle destructed will close down the node.
   */
  ros::NodeHandle n;

  /**
   * The advertise() function is how you tell ROS that you want to
   * publish on a given topic name. This invokes a call to the ROS
   * master node, which keeps a registry of who is publishing and who
   * is subscribing. After this advertise() call is made, the master
   * node will notify anyone who is trying to subscribe to this topic name,
   * and they will in turn negotiate a peer-to-peer connection with this
   * node.  advertise() returns a Publisher object which allows you to
   * publish messages on that topic through a call to publish().  Once
   * all copies of the returned Publisher object are destroyed, the topic
   * will be automatically unadvertised.
   *
   * The second parameter to advertise() is the size of the message queue
   * used for publishing messages.  If messages are published more quickly
   * than we can send them, the number here specifies how many messages to
   * buffer up before throwing some away.
   */
  ros::Publisher cmd_pub = n.advertise<geometry_msgs::Twist>("cmd_vel", 1000);
// After creating the Publisher handle, assign the pointer the address of the new Publisher
    p_cmd_pub = &cmd_pub;
// Inside the callback, it is now possible to publish to the topic.
// p_pub, however, is a pointer, so the syntax is slightly different

    ros::Subscriber sub = n.subscribe("des_vel", 1000, des_vel_Callback);
    ros::Subscriber laserSub = n.subscribe("laser_1", 1000, laserScanCallback);
  ros::Rate loop_rate(10);

  /**
   * A count of how many messages we have sent. This is used to create
   * a unique string for each message.
   */
  int count = 0;
  while (ros::ok())
  {
    /**
     * This is a message object. You stuff it with data, and then publish it.
     */
     if (count % 200 == 0 && is_cur_msg == false){
        ROS_INFO("No directions given to robot.");
     }
     if (count % 200 == 0 && laser == false){
        ROS_INFO("No LIDAR data received from robot.");
     }
    geometry_msgs::Twist msg;

    /* msg.linear = Vector3(0, 0, 0);*/
    /* msg.angular = Vector3(0, 0, 0);*/

    
                            
    /**
     * The publish() function is how you send messages. The parameter
     * is the message object. The type of this object must agree with the type
     * given as a template parameter to the advertise<>() call, as was done
     * in the constructor above.
     */
//    p_cmd_pub->publish(msg);

    ros::spinOnce();

    loop_rate.sleep();
    ++count;
  }


  return 0;
}

