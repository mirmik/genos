cmake_minimum_required(VERSION 3.5.1)
project(rfmeas)
set( CMAKE_VERBOSE_MAKEFILE on )

set(CMAKE_BUILD_TYPE Debug)
#set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CC_STANDARD 11)
set (PLATFORM_SOURCES )

set(CMAKE_CXX_FLAGS "-std=c++20")

file(GLOB SOURCES 
	src/*.cpp
	src/app/*.cpp
	src/cankollm/*.cpp
	src/comm/*.cpp
	src/ndmath/*.cpp
	src/nos-shell/*.cpp
	src/utilxx/*.cpp
	src/servos/*.cpp
	src/groups/*.cpp
	src/moveapi/*.cpp
	src/comm/*.cpp
	src/devices/*.cpp
	src/initialization/*.cpp
	lib/libscpi/src/*.c
	src/initialization/*.cpp
)

execute_process(COMMAND ircc resources.txt -o ircc_resources.gen.cpp --sources-cmake
                OUTPUT_VARIABLE RESOURCE_LIST)
add_custom_command(OUTPUT ircc_resources.gen.cpp
    COMMAND ircc resources.txt -o ircc_resources.gen.cpp
    DEPENDS ${RESOURCE_LIST}
)

add_executable(rfmeas 
	${SOURCES} 
	ircc_resources.gen.cpp
)
target_link_libraries(rfmeas PRIVATE pthread igris nos ralgo rabbit crow)

target_include_directories(rfmeas PRIVATE 
	src
	src/deprecated
	lib/libscpi/inc
	tests
)

add_definitions(
	-DRABBIT_REAL_TYPE=double 
	-DSCPI_USER_CONFIG 
	-DHAVE_CORRECTION_TABLE=0 
	-DHAVE_INTERPOLATION_V2=1 
	-DHAVE_NETWORK_ANALYZER_PROXY=0 
	-DHAVE_LAZER_PROXY=0 
	-DLAZER_EMULATION=0 
	-DHAVE_MITSUBISHI_BOARD=0 
	-DWITHOUT_PARAMETERS=0 
	-DMODBUS_ENABLED=0 
	-DDISABLE_CROW=0
)

enable_testing()
