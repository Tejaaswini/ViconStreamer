cmake_minimum_required(VERSION 3.14)
project(vicon_streamer)

# Find ZeroMQ
find_package(ZeroMQ REQUIRED)

# Include headers
include_directories(include)

# Library
add_library(vicon_streamer SHARED src/vicon_streamer.cpp)
target_link_libraries(vicon_streamer PRIVATE ZeroMQ::ZeroMQ)

# Server example
add_executable(vicon_server src/server.cpp)
target_link_libraries(vicon_server PRIVATE vicon_streamer)

# Client example
add_executable(vicon_client client/client.cpp)
target_link_libraries(vicon_client PRIVATE ZeroMQ::ZeroMQ)