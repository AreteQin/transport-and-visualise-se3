//
// Created by qin on 5/2/22.
//

#include <ros/ros.h>
#include <sensor_msgs/image_encodings.h>
#include <iostream>
#include <tf/transform_listener.h>
#include <sophus/se3.hpp>
#include <geometry_msgs/TransformStamped.h>

int main(int argc, char **argv) {
  ros::init(argc, argv, "receiver");

  ros::NodeHandle nh_listener;
  tf2_ros::Buffer tfBuffer;
  tf2_ros::TransformListener pose_listener(tfBuffer);
  ros::Rate rate(30.0);
  while (nh_listener.ok()) {
    geometry_msgs::TransformStamped pose_ts;
    try { pose_ts = tfBuffer.lookupTransform("map", "camera", ros::Time(0)); }
    catch (tf2::TransformException ex) {
      ROS_ERROR("%s", ex.what());
      ros::Duration(1.0).sleep();
      continue;
    }

    Sophus::SE3f pose_se3;
    Eigen::Quaternion<float> q;
    Eigen::Matrix<float, 3, 1> t;
    t.x() = pose_ts.transform.translation.x;
    t.y() = pose_ts.transform.translation.y;
    t.z() = pose_ts.transform.translation.z;
    q.w() = pose_ts.transform.rotation.w;
    q.x() = pose_ts.transform.rotation.x;
    q.y() = pose_ts.transform.rotation.y;
    q.z() = pose_ts.transform.rotation.z;
    pose_se3 = Sophus::SE3f(q, t);

    std::cout << "current position: " << std::endl << pose_se3.matrix() << std::endl;
    rate.sleep();
  }

  return 0;
}

