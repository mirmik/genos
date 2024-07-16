project(ircc)
cmake_minimum_required(VERSION 3.0)
set(CMAKE_BUILD_TYPE RelWithDebInfo)

set(SOURCES 
	main.cpp
    ircc_resources.gen.cpp)

add_executable(server ${SOURCES})

execute_process(COMMAND ircc resources.txt -o ircc_resources.gen.cpp --sources-cmake
                OUTPUT_VARIABLE RESOURCE_LIST)
add_custom_command(OUTPUT ircc_resources.gen.cpp
    COMMAND ircc resources.txt -o ircc_resources.gen.cpp
    DEPENDS ${RESOURCE_LIST}
)