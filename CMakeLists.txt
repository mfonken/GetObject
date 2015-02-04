cmake_minimum_required(VERSION 2.8)
project( GetObject )

include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( GetObject driver.cpp )
target_link_libraries( GetObject ${OpenCV_LIBS} )