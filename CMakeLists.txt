cmake_minimum_required(VERSION 2.3.1)
project( demoImage )
find_package( OpenCV )
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_custom_target(
   extern_lib
   COMMAND make alaska
   WORKING_DIRECTORY .
)
add_executable( demoImage experimentalExplore.cpp )
target_link_libraries( demoImage ${OpenCV_LIBS} )

