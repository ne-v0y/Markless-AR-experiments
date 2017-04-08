cmake_minimum_required(VERSION 2.8)
project( Markless-AR-experiments )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# ### Build ###
###############
find_package( OpenCV REQUIRED)

include_directories(include ${OpenCV_INCLUDE_DIRS} )

# ### opencv_handler ###
########################
add_library( Detections src/Detections.cpp )

# ### main ###
##############
add_executable( main src/main.cpp )
target_link_libraries( main ${OpenCV_LIBS})
