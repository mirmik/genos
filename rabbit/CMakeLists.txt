cmake_minimum_required(VERSION 3.5.1)
project(rabbit)

file(GLOB HEADERS 
	rabbit/*.h
	rabbit/**/*.h	
)

file(GLOB RABBIT_SOURCES
	rabbit/intersect.cpp
	rabbit/space/screw.cpp
)

file(GLOB SOURCES_TEST 
	tests/*.cpp
)

if(${CMAKE_PROJECT_NAME} STREQUAL ${PROJECT_NAME})	
	set( CMAKE_VERBOSE_MAKEFILE on )
	set(CMAKE_CXX_STANDARD 20)
	set(CMAKE_CC_STANDARD 11)
	set (PLATFORM_SOURCES )
	if (WIN32 OR WIN64)
		set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_CRT_SECURE_NO_WARNINGS")
	endif()

	add_executable(rabbit_test ${SOURCES_TEST} ${RABBIT_SOURCES})
	add_library(rabbit SHARED ${RABBIT_SOURCES})
	
	target_include_directories(rabbit_test PRIVATE . tests)
	target_include_directories(rabbit PRIVATE .)
	target_link_libraries(rabbit_test ralgo nos igris)
	
	if (UNIX AND NOT APPLE)
		install(TARGETS ${PROJECT_NAME}
			LIBRARY DESTINATION /usr/lib
		)
	
		install(DIRECTORY ${PROJECT_NAME}
	    	    DESTINATION /usr/local/include
	    	    PATTERN "*HIDE*" EXCLUDE
	   	)
	endif()
	
	if (APPLE)
		install(TARGETS ${PROJECT_NAME}
			LIBRARY DESTINATION /usr/local/lib
		)
	
		install(DIRECTORY ${PROJECT_NAME}
	    	    DESTINATION /usr/local/include
	    	    PATTERN "*HIDE*" EXCLUDE
	   	)
	endif()
	
	enable_testing()
	add_test(NAME rabbit_test COMMAND rabbit_test)
else()
	set(RABBIT_SOURCES ${RABBIT_SOURCES} PARENT_SCOPE)
endif()