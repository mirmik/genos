# cmake

project (dataproxy)

cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

# find all cpp files in src
file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(dataproxy ${SOURCES})


target_include_directories(dataproxy PRIVATE 
	src
)
target_link_libraries(dataproxy pthread nos igris)