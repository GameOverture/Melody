cmake_minimum_required(VERSION 3.10)
project(NESController)

set(NESCONTROLLER_SRCFILES
	"NESController.cpp"
	"pch.h"
	"pch.cpp"
	"SerialPort.hpp"
	"SerialPort.cpp")

set(NESCONTROLLER_INCLUDE_DIRS "")
list(APPEND NESCONTROLLER_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}")

#################################################################################################
# ^ ^ ^ USER CONFIG ABOVE ^ ^ ^

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_library(${PROJECT_NAME} STATIC ${NESCONTROLLER_SRCFILES})
set_target_properties(${PROJECT_NAME} PROPERTIES
	CXX_STANDARD 17
	CXX_STANDARD_REQUIRED YES
	CXX_EXTENSIONS NO)

target_include_directories(${PROJECT_NAME} PRIVATE "$<BUILD_INTERFACE:${NESCONTROLLER_INCLUDE_DIRS}>")

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "Source Files" FILES ${NESCONTROLLER_SRCFILES})
