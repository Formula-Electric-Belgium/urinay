/**
 * @file main.cpp
 * @author Oriol Gorriz (origovi2000@gmail.com)
 * @brief Main file of Urinay, creates all modules, subcribers and publishers.
 * @version 1.0
 * @date 2022-10-31
 * 
 * @copyright Copyright (c) 2022 BCN eMotorsport
 */

#include <feb_msgs/msg/track.hpp>
#include "rclcpp/rclcpp.hpp"
#include <sys/stat.h>

#include <iostream>

#include "urinay/modules/DelaunayTri.hpp"
#include "urinay/modules/Visualization.hpp"
#include "urinay/modules/WayComputer.hpp"
#include "urinay/utils/Logger.hpp"

// Publishers are initialized here
// rclcpp::Publisher<custom_msgs::msg::PathLimits>::SharedPtr pubPartial;
// rclcpp::Publisher<custom_msgs::msg::PathLimits>::SharedPtr pubFull;

WayComputer *wayComputer;
Params *params;

// This is the map callback
void mapCallback(const feb_msgs::msg::Track::ConstSharedPtr & data) {
  if (not wayComputer->isLocalTfValid()) {
    Logger::logwarn("CarState not being received.");
    if (params->main.verbose)
      Logger::print_report();
    return;
  }
  if (data->track.empty()) {
    Logger::logwarn("Reading empty set of cones.");
    if (params->main.verbose)
      Logger::print_report();
    return;
  }

  Logger::tick("total");

  // Convert to Node vector
  std::vector<Node> nodes;
  nodes.reserve(data->track.size());
  uint32_t id = 0;
  for (const feb_msgs::msg::Cone &c : data->track) {
    nodes.emplace_back(c, id);
    id++;
  }

  // Update local coordinates of Nodes (makes original local coords unnecessary)
  for (const Node &n : nodes) {
    n.updateLocal(wayComputer->getLocalTf());
  }

  // Delaunay triangulation
  TriangleSet triangles = DelaunayTri::compute(nodes);

  // Update the way with the new triangulation
  wayComputer->update(triangles, data->header);

  // Publish loop and write tracklimits to a file
  if (wayComputer->isLoopClosed()) {
    // pubFull.publish(wayComputer->getPathLimits());
    Logger::loginfo("Loop closed!");
    std::string loopDir = params->main.package_path + "/loops";
    mkdir(loopDir.c_str(), 0777);
    wayComputer->writeWayToFile(loopDir + "/loop.unay");
    if (params->main.shutdown_on_loop_closure) {
      Logger::tock("total");
      Logger::loginfo("Have a good day :)");
      if (params->main.verbose)
        Logger::print_report();
      rclcpp::shutdown();
    }
  }
  // Publish partial
  else {
    // pubPartial.publish(wayComputer->getPathLimits());
  }

  Logger::tock("total");
  if (params->main.verbose)
    Logger::print_report();
}

// Main
int main(int argc, char **argv) {
  rclcpp::init(argc, argv);
  auto node = rclcpp::Node::make_shared("urinay");
  Logger::nh_ = node;

  params = new Params(node);
  wayComputer = new WayComputer(params->wayComputer);
  Logger::print_immediately = false;
  Visualization::getInstance().init(node, params->visualization);

  // Subscribers & Publishers
  auto subCones = node->create_subscription<feb_msgs::msg::Track>(params->main.input_cones_topic, 1, mapCallback);
  auto subPose = node->create_subscription<nav_msgs::msg::Odometry>(params->main.input_pose_topic, 1, std::bind(&WayComputer::stateCallback, wayComputer, std::placeholders::_1));

  // pubPartial = nh->advertise<custom_msgs::PathLimits>(params->main.output_partial_topic, 1);
  // pubFull = nh->advertise<custom_msgs::PathLimits>(params->main.output_full_topic, 1, true);  // Latch message

  rclcpp::spin(node);
  delete wayComputer;
  delete params;
  return 0;
}