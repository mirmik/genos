cmake_minimum_required(VERSION 3.5.1)
project(crow)

file(GLOB HEADERS
	crow/*.h
)

file(GLOB SOURCES
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
	crow/gates/chardev/*.cpp
	crow/brocker/*.cpp
)

if(!WIN32)
	file(GLOB SOURCES
		${SOURCES}
		crow/gates/serial_gstuff.cpp
	)
endif()

file(GLOB SOURCES_TEST
	${SOURCES}
	tests/*.cpp
)

if(${CMAKE_PROJECT_NAME} STREQUAL ${PROJECT_NAME})
	set(CMAKE_VERBOSE_MAKEFILE on)
	set(CMAKE_CXX_STANDARD 20)
	set(CMAKE_CC_STANDARD 11)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCROW_USE_ASYNCIO=1 -g")

	if(${STATIC_LIBS})
		set(CMAKE_SHARED_LINKER_FLAGS "-static-libstdc++")
	endif()

	set(PLATFORM_SOURCES)

	if(WIN32 OR WIN64)
		set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_CRT_SECURE_NO_WARNINGS")
	else()
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCROW_REALTIME_THREADS=1")
	endif()

	add_library(crow SHARED ${SOURCES} ${PLATFORM_SOURCES})
	target_include_directories(crow PRIVATE . ${ADDITIONAL_INCLUDES_IGRIS} ${ADDITIONAL_INCLUDES_NOS})

	target_link_directories(crow PUBLIC
		${IGRIS_LIBRARY_PATH}/build-android
		${NOS_LIBRARY_PATH}/build-android
	)
	target_link_libraries(crow igris nos)

	if(NOT DEFINED WITHOUT_TESTS)
		add_executable(crow_test ${SOURCES_TEST})
		target_include_directories(crow_test PRIVATE . tests)
		target_link_libraries(crow_test igris nos pthread)
	endif()

	if(WIN32)
		target_include_directories(crow PRIVATE . "C:/Program Files (x86)/igris/include")
		target_include_directories(crow PRIVATE . "C:/Program Files (x86)/nos/include")
		target_link_directories(crow PRIVATE . "C:/Program Files (x86)/igris/bin")
		target_link_directories(crow PRIVATE . "C:/Program Files (x86)/nos/bin")
		target_link_libraries(crow ws2_32)

		target_include_directories(crow_test PRIVATE . "C:/Program Files (x86)/igris/include")
		target_include_directories(crow_test PRIVATE . "C:/Program Files (x86)/nos/include")
		target_link_directories(crow_test PRIVATE . "C:/Program Files (x86)/igris/bin")
		target_link_directories(crow_test PRIVATE . "C:/Program Files (x86)/nos/bin")
		target_link_libraries(crow_test ws2_32)
	endif()

	if(UNIX AND NOT APPLE)
		install(TARGETS ${PROJECT_NAME}
			LIBRARY DESTINATION /usr/lib
		)

		install(DIRECTORY ${PROJECT_NAME}
			DESTINATION /usr/local/include
			PATTERN "*HIDE*" EXCLUDE
		)
	endif()

	if(APPLE)
		install(TARGETS ${PROJECT_NAME}
			LIBRARY DESTINATION /usr/local/lib
		)

		install(DIRECTORY ${PROJECT_NAME}
			DESTINATION /usr/local/include
			PATTERN "*HIDE*" EXCLUDE
		)
	endif()

	enable_testing()
	add_test(NAME crow_test COMMAND crow_test)
else()
	set(CROW_SOURCES ${SOURCES} PARENT_SCOPE)
endif()
