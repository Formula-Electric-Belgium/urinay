cmake_minimum_required(VERSION 3.5)
project(urinay)
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(feb_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(tf2_eigen REQUIRED)
find_package(visualization_msgs REQUIRED)
find_package(ament_index_cpp REQUIRED)

find_package(Eigen3 REQUIRED)

add_compile_options(-std=c++17)

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Ofast")
set(CMAKE_BUILD_TYPE "Release")

include_directories(${EIGEN3_INCLUDE_DIR})
include_directories(
  include
)

add_executable(${PROJECT_NAME}_exec src/main.cpp)

add_library(${PROJECT_NAME}
  src/structures/Point.cpp
  src/structures/Node.cpp
  src/structures/Edge.cpp
  src/structures/Circle.cpp
  src/structures/Triangle.cpp
  src/structures/Trace.cpp
  src/structures/TraceBuffer.cpp
  src/structures/Vector.cpp
  src/modules/DelaunayTri.cpp
  src/modules/WayComputer.cpp
  src/modules/Visualization.cpp
  src/utils/Logger.cpp
  src/utils/Params.cpp
  src/utils/Failsafe.cpp
  src/utils/KDTree.cpp
)

target_include_directories(${PROJECT_NAME}
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

ament_target_dependencies(${PROJECT_NAME} "nav_msgs" "geometry_msgs" "tf2_ros" "rclcpp" "feb_msgs" "visualization_msgs" "Eigen3" "tf2_eigen" "ament_index_cpp")
ament_target_dependencies(${PROJECT_NAME}_exec "nav_msgs" "geometry_msgs" "tf2_ros" "rclcpp" "feb_msgs" "visualization_msgs" "Eigen3" "tf2_eigen" "ament_index_cpp")
target_link_libraries(${PROJECT_NAME}_exec ${PROJECT_NAME})
ament_export_include_directories(include)
ament_export_libraries(${PROJECT_NAME})


ament_package()

install(
  TARGETS ${PROJECT_NAME}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)
install(
  DIRECTORY include/
  DESTINATION include/
)

install(
  TARGETS ${PROJECT_NAME}_exec
  DESTINATION lib/${PROJECT_NAME}
)

# Install config and launch files
install(DIRECTORY config launch
  DESTINATION share/${PROJECT_NAME}/
)
