cmake_minimum_required(VERSION 2.8)
project( Markless-AR-experiments )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# ### Build ###
###############
# default using 3.2.0
find_package(OpenCV REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)

include_directories(include ${OpenCV_INCLUDE_DIRS} )
include_directories(include ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )
include_directories(include ${GLEW_INCLUDE_DIRS})

# ### opencv_handler ###
########################
add_library( Detections src/Detections.cpp )

# ### opengl_handler ###
########################
add_library( Graphics src/Graphics.cpp )

# ### main ###
##############
add_executable( main src/main.cpp )
target_link_libraries( main ${OpenCV_LIBS}  ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${GLEW_LIBRARIES})

#add_executable(mesh src/Graphics.cpp)
#target_link_libraries(mesh  ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${GLEW_LIBRARIES})
