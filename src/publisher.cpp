//
// Created by qin on 5/2/22.
//

#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <iostream>
#include <sophus/se3.hpp>
#include <geometry_msgs/TransformStamped.h>
#include <cmath>

int main(int argc, char **argv) {
  ros::init(argc, argv, "publisher");
  ros::NodeHandle nh_boardcaster;
  tf2_ros::TransformBroadcaster pose_br;
  double t = 0.0;
  double x,y;
  while (nh_boardcaster.ok()) {
    geometry_msgs::TransformStamped transform_stamped;
    Eigen::Matrix3d R = Eigen::AngleAxisd(M_PI / 2, Eigen::Vector3d(0, 0, 1)).toRotationMatrix();
    x = sin(t);
    y = cos(t);
    t=t+0.01;
    Eigen::Vector3d t(x, y, 0);
    Sophus::SE3d current_position(R, t);
    std::cout << "current position: " << std::endl << current_position.matrix() << std::endl;
    transform_stamped.header.stamp = ros::Time::now();
    transform_stamped.header.frame_id = "map";
    transform_stamped.child_frame_id = "camera";
    transform_stamped.transform.translation.x = current_position.translation()[0];
    transform_stamped.transform.translation.y = current_position.translation()[1];
    transform_stamped.transform.translation.z = current_position.translation()[2];
    transform_stamped.transform.rotation.x = current_position.unit_quaternion().x();
    transform_stamped.transform.rotation.y = current_position.unit_quaternion().y();
    transform_stamped.transform.rotation.z = current_position.unit_quaternion().z();
    transform_stamped.transform.rotation.w = current_position.unit_quaternion().w();

    pose_br.sendTransform(transform_stamped);

    ros::Duration(0.01).sleep();
    ros::spinOnce();
  }

  return 0;
}