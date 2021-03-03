#include <costmap_2d/costmap_2d.h>
#include <costmap_2d/layer.h>
#include <geometry_msgs/Point.h>
#include <ros/ros.h>
#include "std_msgs/String.h"
#include "std_msgs/Bool.h"
#include <geometry_msgs/Pose.h>
#include <pluginlib/class_list_macros.h>
#include <tf/transform_listener.h>

#include <tf/transform_broadcaster.h>
#include <nav_msgs/Odometry.h>

#include <XmlRpcException.h>

#include <iostream>
#include <sstream>
#include "virtual_costmap_layer/virtual_layer.hpp"

geometry_msgs::Point robotpoint();
using Polygon = std::vector<geometry_msgs::Point>;


int main(int argc, char **argv)
{
    virtual_costmap_layer::VirtualLayer vl;

    ros::init(argc, argv, "virtual_layer");
    ros::NodeHandle nh("~");
    ros::Publisher robot_in_zone_pub = nh.advertise<std_msgs::Bool>("robot_in_zone_pub", 1000);
    ros::Rate loop_rate(10);
    std::string param {"/move_base/local_costmap/virtual_layer/forms"};
    std_msgs::Bool robot_in_zone_pub_bool;
    tf::TransformListener tfListener;
    geometry_msgs::PoseStamped current_robot_pose, current_robot_pose_base;
    geometry_msgs::Point robot_point;
    geometry_msgs::TransformStamped current_transform_msg;
    tf::StampedTransform current_transform_tf;


    double _costmap_resolution;            
    double _min_x, _min_y, _max_x, _max_y; 

    int count = 0;

    tf::TransformListener listener;

    
    while (ros::ok())
    {
        tf::StampedTransform transform;
        try {
            listener.lookupTransform("map", "base_link", ros::Time(0), transform);
        } catch (tf::TransformException &ex) {
            ROS_DEBUG_STREAM("Can't get robot pose: " << ex.what());
            continue;
        }

        robot_point.x = transform.getOrigin().x();
        robot_point.y = transform.getOrigin().y();
        robot_point.z = 0.0;

        vl.parseFormListFromYaml(nh, param);
        vl.computeMapBounds();
        
        robot_in_zone_pub_bool.data = false;
    
        for(int i = 0 ; i < vl._form_polygons.size(); i++ ){
            if (vl.robotInZone(vl._form_polygons[i],robot_point) == true) {
                robot_in_zone_pub_bool.data = true;
                ROS_ERROR_STREAM("Robot point is in the polygon I made");
                // system("rosrun dynamic_reconfigure dynparam set /move_base/global_costmap/inflater_layer inflation_radius 10");
            }
            else {
                // system("rosrun dynamic_reconfigure dynparam set /move_base/global_costmap/inflater_layer inflation_radius 0");
            }
            robot_in_zone_pub.publish(robot_in_zone_pub_bool);
            
        }
        


        ros::spinOnce();
        loop_rate.sleep();
        ++count;
    
    }

    return 0;
}

