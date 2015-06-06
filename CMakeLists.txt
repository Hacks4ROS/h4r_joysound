cmake_minimum_required(VERSION 2.8.3)
project(h4r_joysound)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  roscpp
  sound_play
  sensor_msgs
)

catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES joysound
  CATKIN_DEPENDS sound_play sensor_msgs
#  DEPENDS system_lib
)

###########
## Build ##
###########

include_directories(
  ${catkin_INCLUDE_DIRS}
)
add_executable(joysound_node src/joysound_node.cpp)

target_link_libraries(joysound_node
   ${catkin_LIBRARIES}
)

#############
## Install ##
#############


## Mark executables and/or libraries for installation
 install(TARGETS joysound_node
   ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
   LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
   RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
 )

## Mark other files for installation (e.g. launch and bag files, etc.)
install(DIRECTORY launch sound
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)

