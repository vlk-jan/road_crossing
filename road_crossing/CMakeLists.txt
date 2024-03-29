cmake_minimum_required(VERSION 3.0.2)
project(road_crossing)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  compass_msgs
  geometry_msgs
  std_msgs
  road_crossing_msgs
  behaviortree_cpp_v3
  message_generation
)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};/usr/share/cmake/geographiclib")
find_package(GeographicLib REQUIRED)

catkin_python_setup()

add_service_files(
  FILES
  get_road_segment.srv
  get_suitability.srv
  start_algorithm.srv
  get_road_info.srv
  get_finish.srv
)

generate_messages(
  DEPENDENCIES 
  std_msgs
)

catkin_package(
  CATKIN_DEPENDS message_runtime rospy
)

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
  ${GeographicLib_INCLUDE_DIRS}
)

catkin_install_python(
  PROGRAMS
  nodes/test_injector
  nodes/road_cost_run
  nodes/get_segment_srv
  nodes/get_suitability_srv
  nodes/get_finish_srv
  nodes/get_road_info_srv
  nodes/start
  scripts/save_test_road.py
  scripts/process_logs.py

  DESTINATION
  ${CATKIN_PACKAGE_BIN_DESTINATION}
)

set(SOURCES src/misc.cpp include/road_crossing/misc.h)
add_library(misc_lib ${catkin_LIBRARIES} ${SOURCES})
target_link_libraries(misc_lib ${catkin_LIBRARIES} ${GeographicLib_LIBRARIES})

set(SOURCES src/get_azimuth.cpp include/road_crossing/get_azimuth.h)
add_library(azimuth_lib ${catkin_LIBRARIES} ${SOURCES})
target_link_libraries(azimuth_lib ${catkin_LIBRARIES} misc_lib)
add_dependencies(misc_lib ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

set(SOURCES src/road_cross_tree.cpp include/road_crossing/road_cross_tree.h)
add_library(road_cross_tree_lib ${SOURCES} ${catkin_LIBRARIES})
target_link_libraries(road_cross_tree_lib ${catkin_LIBRARIES} azimuth_lib)

set(SOURCES src/get_gps.cpp include/road_crossing/get_gps.h)
add_library(gps_lib ${catkin_LIBRARIES} ${SOURCES})
target_link_libraries(gps_lib ${catkin_LIBRARIES})
add_dependencies(gps_lib ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

set(SOURCES src/vehicles.cpp include/road_crossing/vehicles.h)
add_library(vehicles_lib ${catkin_LIBRARIES} ${SOURCES})
target_link_libraries(vehicles_lib ${catkin_LIBRARIES} misc_lib gps_lib)
add_dependencies(vehicles_lib ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

set(SOURCES src/movement.cpp include/road_crossing/movement.h)
add_library(movement_lib ${catkin_LIBRARIES} ${SOURCES})
target_link_libraries(movement_lib ${catkin_LIBRARIES} misc_lib gps_lib vehicles_lib)
add_dependencies(movement_lib ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

set(SOURCES src/start_srv.cpp include/road_crossing/start_srv.h)
add_library(start_lib ${catkin_LIBRARIES} ${SOURCES})
target_link_libraries(start_lib ${catkin_LIBRARIES} misc_lib movement_lib gps_lib vehicles_lib)
add_dependencies(start_lib ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

add_executable(run_tree src/main.cpp)
target_link_libraries(run_tree ${catkin_LIBRARIES} road_cross_tree_lib azimuth_lib movement_lib gps_lib vehicles_lib start_lib)

add_executable(start_srv src/start_srv.cpp include/road_crossing/start_srv.h)
target_link_libraries(start_srv ${catkin_LIBRARIES})
add_dependencies(start_srv ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
