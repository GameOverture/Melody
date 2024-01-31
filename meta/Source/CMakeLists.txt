cmake_minimum_required(VERSION 3.10)
project(Melody)

set(REL_PROJ_PATH "../../")
set(REL_DATA_DIR "../../data")
set(REL_HARMONY_DIR "../../../../../LG/_GameFramework/HarmonyEngine")
set(EXE_OUTPUT_NAME "Melody")

set(GAME_SRCFILES
	"main.cpp"
	"pch.h"
	"Melody.cpp"
	"Melody.h"
	"Wheel/Wheel.cpp"
	"Wheel/Wheel.h"
	"Wheel/WheelInit.h"
	"Wheel/WeightTable.cpp"
	"Wheel/WeightTable.h"
	"Scenes/Intro.cpp"
	"Scenes/Intro.h"
	"Scenes/Outro.cpp"
	"Scenes/Outro.h"
	"Components/IComponent.cpp"
	"Components/IComponent.h"
	"Scenes/LowerThird.cpp"
	"Scenes/LowerThird.h"
	"Scenes/Break.cpp"
	"Scenes/Break.h"
	"Components/Crt/Crt.cpp"
	"Components/Crt/Crt.h"
	"Components/MessageCycle.cpp"
	"Components/MessageCycle.h"
	"Scenes/CtrlPanel.cpp"
	"Scenes/CtrlPanel.h"
	"Components/Crt/Channels/Channel.cpp"
	"Components/Crt/Channels/Channel.h"
	"Components/Crt/Channels/Music.cpp"
	"Components/Crt/Channels/Music.h"
	"Components/FightStick.cpp"
	"Components/FightStick.h"
	"Components/VgMusic.cpp"
	"Components/VgMusic.h"
	"_hy_/NewEntity.h"
	"_hy_/NewEntity.cpp"
	"Components/Brb.cpp"
	"Components/Brb.h"
	"Components/HeartBeat.cpp"
	"Components/HeartBeat.h")

set(GAME_INCLUDE_DIRS "")
list(APPEND GAME_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}")
list(APPEND GAME_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/Components/Crt")
list(APPEND GAME_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/Wheel")
list(APPEND GAME_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/_hy_")
list(APPEND GAME_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/Components")
list(APPEND GAME_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/Components/Crt/Channels")
list(APPEND GAME_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/Scenes")

#################################################################################################
# ^ ^ ^ USER CONFIG ABOVE ^ ^ ^

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

# Build Options
set(HYBUILD_GLFW ON)
set(HYBUILD_SDL2 OFF)
set(HYBUILD_SDL_AUDIO OFF)
set(HYBUILD_SDL_NET OFF)
option(HYBUILD_DebugEmscripten "Use a Debug build configuration when targeting web browsers" OFF)
option(HYBUILD_STATIC "Statically link all dependencies into the executable" OFF)

# Built Extras - Requires installed libraries
set(HYBUILD_SPINE OFF)
set(HYBUILD_ICU OFF)

if(HYBUILD_GLFW)
	add_compile_definitions(HY_USE_GLFW)
endif()
if(HYBUILD_SDL2)
	add_compile_definitions(HY_USE_SDL2)
endif()
if(HYBUILD_SDL_AUDIO)
	add_compile_definitions(HY_USE_SDL2_AUDIO)
endif()
if(HYBUILD_SPINE)
	add_compile_definitions(HY_USE_SPINE)
endif()
if(HYBUILD_RAPIDJSON)
	add_compile_definitions(HY_USE_RAPIDJSON)
endif()
if(HYBUILD_ICU)
	add_compile_definitions(HY_USE_ICU)
endif()

message(STATUS "Melody building for target: ${CMAKE_SYSTEM_NAME}")
if("${CMAKE_SYSTEM_NAME}" STREQUAL "Emscripten")
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${REL_PROJ_PATH}/bin/web")
	set(HYEM_COMPILE_FLAGS "")
	set(HYEM_LINK_FLAGS "")
	list(APPEND HYEM_LINK_FLAGS "-s TOTAL_STACK=4MB")
	list(APPEND HYEM_LINK_FLAGS "-s ALLOW_MEMORY_GROWTH=1")
	list(APPEND HYEM_LINK_FLAGS "-s USE_WEBGL2=1")
	list(APPEND HYEM_LINK_FLAGS "-s FETCH")
	
	
	if(HYBUILD_GLFW)
		list(APPEND HYEM_LINK_FLAGS "-s USE_GLFW=3")
	endif()
	
	if(HYBUILD_SDL2)
		list(APPEND HYEM_COMPILE_FLAGS "-s USE_SDL=2")
		list(APPEND HYEM_LINK_FLAGS "-s USE_SDL=2")
	endif()
	
	if(HYBUILD_SDL_NET)
		list(APPEND HYEM_COMPILE_FLAGS "-s USE_SDL_NET=2")
		list(APPEND HYEM_LINK_FLAGS "-s USE_SDL_NET=2")
	endif()
	
	if(HYBUILD_ICU)
		list(APPEND HYEM_COMPILE_FLAGS "-s USE_ICU=1")
		list(APPEND HYEM_LINK_FLAGS "-s USE_ICU=1")
	endif()
	
	list(APPEND HYEM_LINK_FLAGS "--no-heap-copy")
	list(APPEND HYEM_LINK_FLAGS "--preload-file ${CMAKE_CURRENT_SOURCE_DIR}/${REL_DATA_DIR}@./data")
	list(APPEND HYEM_LINK_FLAGS "--preload-file ${CMAKE_CURRENT_SOURCE_DIR}/${REL_PROJ_PATH}/${PROJECT_NAME}.hyproj@${PROJECT_NAME}.hyproj")
	
	if(HYBUILD_DebugEmscripten)
		set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${REL_PROJ_PATH}/bin/web/debug")
		message(STATUS "!!! EMSCRIPTEN DEBUG FLAGS ENABLED !!!")
		add_compile_definitions(DEBUG)
		
		list(APPEND HYEM_COMPILE_FLAGS "-g")
		#list(APPEND HYEM_COMPILE_FLAGS "-s ASSERTIONS=1")
		#list(APPEND HYEM_COMPILE_FLAGS "-s STACK_OVERFLOW_CHECK=2")
		#list(APPEND HYEM_COMPILE_FLAGS "-s LLD_REPORT_UNDEFINED")
		#list(APPEND HYEM_COMPILE_FLAGS "-s SAFE_HEAP=1")
		#list(APPEND HYEM_COMPILE_FLAGS "-fsanitize=undefined")
		#list(APPEND HYEM_COMPILE_FLAGS "-fsanitize=address")
		#list(APPEND HYEM_COMPILE_FLAGS "-s TOTAL_MEMORY=2147418112")
		#list(APPEND HYEM_COMPILE_FLAGS "-gsource-map http://localhost:8000/")
		
		list(APPEND HYEM_LINK_FLAGS "-g")
		#list(APPEND HYEM_LINK_FLAGS "-s ASSERTIONS=1")
		#list(APPEND HYEM_LINK_FLAGS "-s STACK_OVERFLOW_CHECK=2")
		#list(APPEND HYEM_LINK_FLAGS "-s LLD_REPORT_UNDEFINED")
		#list(APPEND HYEM_LINK_FLAGS "-s SAFE_HEAP=1")
		#list(APPEND HYEM_LINK_FLAGS "-fsanitize=undefined")
		#list(APPEND HYEM_LINK_FLAGS "-fsanitize=address")
		#list(APPEND HYEM_LINK_FLAGS "-s TOTAL_MEMORY=2147418112")
		#list(APPEND HYEM_LINK_FLAGS "-gsource-map http://localhost:8000/")
	else()
		list(APPEND HYEM_COMPILE_FLAGS "-O2")
	endif()
	
	string(REPLACE ";" " " HYEM_COMPILE_FLAGS "${HYEM_COMPILE_FLAGS}")
	add_compile_options("SHELL:${HYEM_COMPILE_FLAGS}")
	
	string(REPLACE ";" " " HYEM_LINK_FLAGS "${HYEM_LINK_FLAGS}")
	add_link_options("SHELL:${HYEM_LINK_FLAGS}")
	
	set(CMAKE_EXECUTABLE_SUFFIX ".html")
else()
	if(MSVC)
		if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Win32")
			set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${REL_PROJ_PATH}/bin/Win32")
		else()
			set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${REL_PROJ_PATH}/bin/x64")
		endif()
	else()
		set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${REL_PROJ_PATH}/bin/")
	endif()
endif()

add_subdirectory("${REL_HARMONY_DIR}/Engine" "HyEngine")
add_subdirectory("../../../SimpleBLE/simpleble" "simpleble")

add_executable(${PROJECT_NAME} WIN32 ${GAME_SRCFILES})
set_target_properties(${PROJECT_NAME} PROPERTIES
	CXX_STANDARD 17
	CXX_STANDARD_REQUIRED YES
	CXX_EXTENSIONS NO
	OUTPUT_NAME ${EXE_OUTPUT_NAME})

target_include_directories(${PROJECT_NAME} PRIVATE "$<BUILD_INTERFACE:${GAME_INCLUDE_DIRS}>")
if(NOT "${CMAKE_SYSTEM_NAME}" STREQUAL "Emscripten")
	target_precompile_headers(${PROJECT_NAME} PRIVATE "pch.h")
endif()
target_link_libraries(${PROJECT_NAME} PUBLIC HyEngine "simpleble" )


if(TARGET uninstall)
	set_target_properties(uninstall PROPERTIES FOLDER "CMakePredefinedTargets")
endif()

if (MSVC)
	set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/${REL_PROJ_PATH}")
	set_property(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
	
	if(HYBUILD_ICU)
		set(ICU_RUNTIME_LIBS "")
		file(GLOB_RECURSE ICU_RUNTIME_LIBS "${REL_HARMONY_DIR}/Engine/extras/ICU/*.dll")
		foreach(CUR_RUNTIME_FILE IN LISTS ICU_RUNTIME_LIBS)
			add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
				COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CUR_RUNTIME_FILE} $(OutDir)
				COMMENT "Copying ICU runtime lib: ${CUR_RUNTIME_FILE}\n")
		endforeach()
	endif()
endif()

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "Source Files" FILES ${GAME_SRCFILES})
