/**
 * @file Param.cpp
 * @author Oriol Gorriz (origovi2000@gmail.com)
 * @brief Contains the Param class member functions implementation
 * @version 1.0
 * @date 2022-10-31
 * 
 * @copyright Copyright (c) 2022 BCN eMotorsport
 */

#include "utils/Params.hpp"

/* ----------------------------- Private Methods ---------------------------- */

/* ----------------------------- Public Methods ----------------------------- */

Params::Params(rclcpp::Node::SharedPtr const nh) {
  std::string ns = nh->get_name();
  // Main
  main.package_path = ament_index_cpp::get_package_share_directory("urinay");
  main.input_cones_topic = nh->declare_parameter<std::string>(ns + "/input_cones_topic","/AS/P/ccat/cones");
  main.input_pose_topic = nh->declare_parameter<std::string>(ns + "/input_pose_topic", "/AS/C/state");
  main.output_full_topic = nh->declare_parameter<std::string>(ns + "/output_full_topic", "/AS/P/tracklimits/full");
  main.output_partial_topic = nh->declare_parameter<std::string>(ns + "/output_partial_topic", "/AS/P/tracklimits/partial");
  main.shutdown_on_loop_closure = nh->declare_parameter<bool>(ns + "/shutdown_on_loop_closure", true);
  main.min_cone_confidence = nh->declare_parameter<float>(ns + "/min_cone_confidence", 0.0);
  main.verbose = nh->declare_parameter<bool>(ns + "/verbose", false);

  // WayComputer
  wayComputer.max_triangle_edge_len = nh->declare_parameter<double>(ns + "/max_triangle_edge_len", 9.0);
  wayComputer.min_triangle_angle = nh->declare_parameter<double>(ns + "/min_triangle_angle", 0.25);
  wayComputer.failsafe_max_way_horizon_size = nh->declare_parameter<int>(ns + "/failsafe_max_way_horizon_size", 6);
  wayComputer.general_failsafe = nh->declare_parameter<bool>(ns + "/general_failsafe", true);
  wayComputer.general_failsafe_safetyFactor = nh->declare_parameter<double>(ns + "/general_failsafe_safetyFactor", 1.4);
  // WayComputer::Search
  wayComputer.search.max_way_horizon_size = nh->declare_parameter<int>(ns + "/max_way_horizon_size", 0);
  wayComputer.search.tree_search_max_height = nh->declare_parameter<int>(ns + "/tree_search_max_height", 7);
  wayComputer.search.search_radius = nh->declare_parameter<double>(ns + "/search_radius", 5.0);
  wayComputer.search.max_angle_diff = nh->declare_parameter<double>(ns + "/max_angle_diff", 0.6);
  wayComputer.search.min_track_width = nh->declare_parameter<double>(ns + "/min_track_width", 2.8);
  wayComputer.search.max_search_options = nh->declare_parameter<int>(ns + "/max_search_options", 2);
  wayComputer.search.overnext_midpoint_angle = nh->declare_parameter<double>(ns + "/overnext_midpoint_angle", 0.2);
  wayComputer.search.max_next_heuristic = nh->declare_parameter<double>(ns + "/max_next_heuristic", 3.0);
  wayComputer.search.heur_dist_weight = nh->declare_parameter<float>(ns + "/heur_dist_weight", 0.6);
  wayComputer.search.heur_track_width_diff_weight = nh->declare_parameter<float>(ns + "/heur_track_width_diff_weight", 0.2);
  wayComputer.search.allow_intersection = nh->declare_parameter<bool>(ns + "/allow_intersection", false);
  wayComputer.search.min_dist_between_midpoints = nh->declare_parameter<double>(ns + "/min_dist_between_midpoints", 0.09);
  // WayComputer::Trace
  wayComputer.trace.max_dist_loop_closure = nh->declare_parameter<double>(ns + "/max_dist_loop_closure", 1.0);
  wayComputer.trace.max_angle_diff_loop_closure = nh->declare_parameter<double>(ns + "/max_angle_diff_loop_closure", 0.6);
  wayComputer.trace.vital_num_midpoints = nh->declare_parameter<int>(ns + "/vital_num_midpoints", 5);

  // Visualization
  visualization.publish_markers = nh->declare_parameter<bool>(ns + "/publish_markers", false);
  visualization.visualize_treeSearch = nh->declare_parameter<bool>(ns + "/visualize_treeSearch", false);
  visualization.triangulation_topic = nh->declare_parameter<std::string>(ns + "/marker_topics/triangulation", "/AS/P/urinay/markers/triangulation");
  visualization.way_topic = nh->declare_parameter<std::string>(ns + "/marker_topics/way", "/AS/P/urinay/markers/way");
  visualization.treeSearch_topic = nh->declare_parameter<std::string>(ns + "/marker_topics/treeSearch", "/AS/P/urinay/markers/treeSearch");
}