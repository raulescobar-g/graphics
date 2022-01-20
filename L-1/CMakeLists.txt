CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

# Name of the project
PROJECT(L-1)

# Use glob to get the list of all source files.
FILE(GLOB_RECURSE SOURCES "src/*.cpp")

# Set the executable.
ADD_EXECUTABLE(${CMAKE_PROJECT_NAME} ${SOURCES})

# OS specific options and libraries
IF(WIN32)
	# c++11 is enabled by default.
	# -Wall produces way too many warnings.
	# -pedantic is not supported.
	# Disable warning 4996.
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4996")
	SET_PROPERTY(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${CMAKE_PROJECT_NAME})
ELSE()
	# Enable all pedantic warnings.
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -pedantic")
	IF(APPLE)
		# Add required frameworks for GLFW.
	ELSE()
		#Link the Linux OpenGL library
	ENDIF()
ENDIF()
