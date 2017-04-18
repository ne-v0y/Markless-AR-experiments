cmake_minimum_required(VERSION 2.8)
project( Markless-AR-experiments )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# ### Build ###
###############
# default using 3.2.0
#SET(OpenCV_INCLUDE_DIRS "/usr/local/include/opencv;/usr/local/include/opencv2")
#SET(OpenCV_LIB_DIR "/usr/local/lib")
find_package( OpenCV REQUIRED)

include_directories(include ${OpenCV_INCLUDE_DIRS} )

# ### opencv_handler ###
########################
add_library( Detections src/Detections.cpp )

# ### opengl_handler ###
########################
add_library( Graphics src/Graphics.cpp )

# ### main ###
##############
add_executable( main src/main.cpp )
target_link_libraries( main ${OpenCV_LIBS})

#add_executable(mesh src/Graphics.cpp)
