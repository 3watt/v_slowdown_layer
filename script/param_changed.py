#! /usr/bin/env python

import rospy
import dynamic_reconfigure.client
from std_msgs.msg import Byte, Bool
import time

def layer_callback(data):
    rate = rospy.Rate(50)
    if data.data == True :
        client.update_configuration({"inflation_radius": 0.13})
    elif data.data == False :
        client.update_configuration({"inflation_radius": 5})
    print(data.data)
    rate.sleep()

    
if __name__ == '__main__':
    rospy.init_node("dynamic_reconfigure_client")
    client = dynamic_reconfigure.client.Client("/move_base/global_costmap/inflater_layer/")
    layer_sub = rospy.Subscriber("/virtual_costmap_layer_node/robot_in_zone_pub", Bool, layer_callback)
        
    rospy.spin()