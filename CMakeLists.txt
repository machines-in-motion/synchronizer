cmake_minimum_required(VERSION 2.8.3)
project(synchronizer)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wl,--no-as-needed")

find_package(catkin REQUIRED COMPONENTS
  mpi_cmake_modules
  pybind11_catkin
  shared_memory
  real_time_tools)

include_directories(include ${catkin_INCLUDE_DIRS} )

catkin_package (
INCLUDE_DIRS include
CATKIN_DEPENDS mpi_cmake_modules shared_memory real_time_tools
LIBRARIES synchronizers
)

DEFINE_OS()

###############
# add library #
###############

add_library(synchronizers 
  src/slave_sync.cpp
  src/master_sync.cpp
  src/slave.cpp
  src/master.cpp
  )
target_link_libraries(synchronizers
  ${catkin_LIBRARIES}
)

#########
# demos #
#########

add_executable(demo_slave
  demos/demo_slave.cpp)
target_link_libraries(demo_slave synchronizers)

add_executable(demo_master
  demos/demo_master.cpp)
target_link_libraries(demo_master synchronizers)

##############
# unit tests #
##############
  
catkin_add_gtest(synchronizer_unit_tests
  tests/main.cpp
  tests/unit_tests_synchronizer.cpp
)
target_link_libraries(synchronizer_unit_tests
  synchronizers)

###################
# python bindings #
###################

pybind11_add_module(synchronizer srcpy/wrappers.cpp)
target_link_libraries(synchronizer PRIVATE synchronizers ${catkin_LIBRARIES})
set_target_properties(synchronizer PROPERTIES
  LIBRARY_OUTPUT_DIRECTORY ${CATKIN_DEVEL_PREFIX}/${CATKIN_GLOBAL_PYTHON_DESTINATION})
install(TARGETS synchronizer DESTINATION ${CATKIN_GLOBAL_PYTHON_DESTINATION})

##########################
# building documentation #
##########################

build_doxygen_documentation()
