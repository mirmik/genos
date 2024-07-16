cmake_minimum_required(VERSION 3.5.1)
project(nos)

file(GLOB HEADERS
	nos/*.h
	nos/**/*.h
)

file(GLOB NOS_SOURCES
	nos/*.cpp
	nos/print/*.cpp
	nos/fprint/*.cpp
	nos/check/*.cpp
	nos/util/osutil.cpp
	nos/util/trace.cpp
	nos/util/numconvert.cpp
	nos/util/nos_numconvert.cpp
	nos/util/string.cpp
	nos/meta/*.cpp
	nos/input/*.cpp
	nos/io/impls/current_ostream_stdout.cpp
	nos/convert/*.cpp
	nos/io/*.cpp
	nos/trent/*.cpp
	nos/log/*.cpp
)

if(UNIX)
	file(GLOB NOS_SOURCES ${NOS_SOURCES}
		nos/util/osutil_unix.cpp
		nos/inet/*.cpp
		nos/log/*.cpp
	)
endif()

if(WIN32)
	file(GLOB NOS_SOURCES ${NOS_SOURCES}
		nos/util/osutil_windows.cpp
		nos/inet/*.cpp
	)
endif()

file(GLOB NOS_SOURCES_PLUS)

file(GLOB SOURCES_TEST
	tests/*.cpp
)

if(UNIX)
	file(GLOB SOURCES_TEST ${SOURCES_TEST}
		tests/unix/*.cpp
	)
endif()

if(${CMAKE_PROJECT_NAME} STREQUAL ${PROJECT_NAME})
	set(CMAKE_VERBOSE_MAKEFILE on)
	set(CMAKE_CXX_STANDARD 20)
	set(CMAKE_CC_STANDARD 11)
	set(PLATFORM_SOURCES)

	if(${STATIC_LIBS})
		set(CMAKE_SHARED_LINKER_FLAGS "-static-libstdc++")
	endif()

	if(WIN32 OR WIN64)
		set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_CRT_SECURE_NO_WARNINGS")
	endif()

	add_library(nos SHARED ${NOS_SOURCES})
	target_include_directories(nos PRIVATE . ${ADDITIONAL_INCLUDES})

	if(NOT DEFINED WITHOUT_TESTS)
		add_executable(nos_test ${SOURCES_TEST} ${NOS_SOURCES})
		target_include_directories(nos_test PRIVATE . tests ${ADDITIONAL_INCLUDES})
	endif()

	if(WIN32)
		target_link_libraries(nos ws2_32)
		target_link_libraries(nos_test ws2_32)

		install(TARGETS ${PROJECT_NAME}
			EXPORT ${PROJECT_NAME}
			LIBRARY DESTINATION /)

		install(EXPORT ${PROJECT_NAME}
			NAMESPACE ${PROJECT_NAME}::
			FILE ${PROJECT_NAME}-config.cmake
			DESTINATION lib/cmake/${PROJECT_NAME})

		install(DIRECTORY ${PROJECT_NAME}
			DESTINATION "C:/Program Files (x86)/${PROJECT_NAME}/include"
			PATTERN "*HIDE*" EXCLUDE
		)
	endif()

	if(UNIX AND NOT APPLE)
		target_link_libraries(nos_test pthread)

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
	add_test(NAME nos_test COMMAND nos_test)

else()
	set(NOS_SOURCES ${NOS_SOURCES} ${NOS_SOURCES_PLUS} PARENT_SCOPE)
endif()
