# ##############################################################################
# Setup version
file(STRINGS ${CMAKE_SOURCE_DIR}/VERSION version)
message(STATUS "Read version from file: ${version}")

cmake_minimum_required(VERSION 3.5.1)
project(virtual_costmap_layer LANGUAGES C CXX VERSION ${version})

set(CMAKE_CXX_STANDARD 14)
set(CXX_STANDARD_REQUIRED ON)

# ##############################################################################
# Load and setup packages

find_package(catkin
             REQUIRED
             roscpp
             costmap_2d
             dynamic_reconfigure
             custom_msgs
             std_msgs
             tf)
## System dependencies are found with CMake's conventions
find_package(Boost REQUIRED COMPONENTS system)

# ##############################################################################
# Generate messages and configurations
generate_dynamic_reconfigure_options(cfg/VirtualLayer.cfg)

# ##############################################################################
# Setup catkin packages
catkin_package(INCLUDE_DIRS
               include
               LIBRARIES
               virtual_layer
               CATKIN_DEPENDS
               roscpp
               costmap_2d
               dynamic_reconfigure
               custom_msgs
               tf
               DEPENDS)

# ##############################################################################
# Generate library
include_directories(include ${catkin_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})
add_library(virtual_layer src/virtual_layer.cpp)
target_link_libraries(virtual_layer ${catkin_LIBRARIES})
add_dependencies(virtual_layer ${PROJECT_NAME}_gencfg)


# include_directories(include ${catkin_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})
add_executable(virtual_layer_node src/main.cpp)
target_link_libraries(virtual_layer_node ${catkin_LIBRARIES} virtual_layer)
# add_dependencies(virtual_layer_node ${PROJECT_NAME}_gencfg)



# ############################################################################
# add_library(compare src/compare.cpp)
# target_link_libraries(compare ${catkin_LIBRARIES} virtual_layer)
# add_dependencies(compare ${PROJECT_NAME}_gencfg)
# ###########################3#################################################
