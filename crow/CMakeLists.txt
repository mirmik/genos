cmake_minimum_required(VERSION 3.12)

set(CROW_TOP_LEVEL OFF)
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    set(CROW_TOP_LEVEL ON)
endif()

project(crow LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

if(CROW_TOP_LEVEL)
    set(CMAKE_VERBOSE_MAKEFILE ON)
endif()

include(GNUInstallDirs)
# Явные пути установки: библиотека в /usr/lib, заголовки в /usr/local/include
set(CMAKE_INSTALL_LIBDIR "/usr/lib")
set(CMAKE_INSTALL_INCLUDEDIR "/usr/local/include")
find_package(Threads REQUIRED)

option(CROW_BUILD_APPS "Build crow command line utilities" ${CROW_TOP_LEVEL})
option(CROW_BUILD_TESTS "Build crow tests" ${CROW_TOP_LEVEL})

if(STATIC_LIBS)
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -static-libstdc++")
endif()

add_compile_definitions(CROW_USE_ASYNCIO=1)
if(WIN32)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
else()
    add_compile_definitions(CROW_REALTIME_THREADS=1)
endif()

file(GLOB CROW_HEADERS
    crow/*.h
)

file(GLOB CROW_SOURCES CONFIGURE_DEPENDS
    crow/src/*.cpp
    crow/src/*.c
	crow/src/variants/allocation_malloc.cpp
	crow/src/variants/crow_print.cpp
	crow/src/variants/warn.cpp
	crow/src/variants/stdtime.cpp
    crow/pubsub/*.cpp
    crow/nodes/*.cpp
    crow/proto/*.cpp
    crow/gates/udpgate.cpp
    crow/gates/tcpgate.cpp
    crow/gates/chardev/*.cpp
    crow/brocker/*.cpp
)

if(NOT WIN32)
    list(APPEND CROW_SOURCES crow/gates/serial_gstuff.cpp)
endif()

add_library(crow SHARED ${CROW_SOURCES})

target_include_directories(crow
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_FULL_INCLUDEDIR}>
)

if(ADDITIONAL_INCLUDES_IGRIS)
    target_include_directories(crow PRIVATE ${ADDITIONAL_INCLUDES_IGRIS})
endif()

if(ADDITIONAL_INCLUDES_NOS)
    target_include_directories(crow PRIVATE ${ADDITIONAL_INCLUDES_NOS})
endif()

if(IGRIS_LIBRARY_PATH)
    target_link_directories(crow PUBLIC ${IGRIS_LIBRARY_PATH}/build-android)
endif()

if(NOS_LIBRARY_PATH)
    target_link_directories(crow PUBLIC ${NOS_LIBRARY_PATH}/build-android)
endif()

target_link_libraries(crow
    PUBLIC
        igris
        nos
        Threads::Threads
)

if(WIN32)
    target_link_libraries(crow PRIVATE ws2_32)
endif()

if(CROW_BUILD_APPS)
    add_executable(ctrans apps/ctrans/main.cpp)
    target_link_libraries(ctrans PRIVATE crow)
    target_include_directories(ctrans PRIVATE apps/ctrans)

    add_executable(crowker
        apps/crowker/main.cpp
        apps/crowker/control_node.cpp
    )
    target_link_libraries(crowker PRIVATE crow)
    target_include_directories(crowker PRIVATE apps/crowker)
endif()

if(CROW_BUILD_TESTS)
    enable_testing()
    file(GLOB CROW_TEST_SOURCES CONFIGURE_DEPENDS tests/*.cpp )
    add_executable(crow_test ${CROW_TEST_SOURCES})
    target_link_libraries(crow_test PRIVATE crow)
    target_include_directories(crow_test PRIVATE tests)
    add_test(NAME crow_test COMMAND crow_test)
endif()

install(TARGETS crow
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(DIRECTORY crow
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    PATTERN "*HIDE*" EXCLUDE
)

if(CROW_BUILD_APPS)
    install(TARGETS ctrans crowker
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
endif()

if(NOT CROW_TOP_LEVEL)
    set(CROW_SOURCES ${CROW_SOURCES} PARENT_SCOPE)
    set(CROW_HEADERS ${CROW_HEADERS} PARENT_SCOPE)
endif()
