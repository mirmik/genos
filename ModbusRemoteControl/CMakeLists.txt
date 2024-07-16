cmake_minimum_required(VERSION 2.8.9)

project (ModbusRemoteControl)

set(CMAKE_CXX_FLAGS "-Wall -Weffc++")
set(CMAKE_CXX_STANDARD 17)

set(SOURCES
	main.cpp
	ModbusHMI.cpp
	scpi.cpp
)

add_executable(ModbusRemoteControl ${SOURCES})
target_include_directories(ModbusRemoteControl PRIVATE .)

target_link_libraries(ModbusRemoteControl modbus pthread igris nos)
