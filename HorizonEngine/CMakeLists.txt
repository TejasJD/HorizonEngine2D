# CMakeList.txt : CMake project for HorizonEngine, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project(HorizonEngine)

set(HZN_SOURCE_FILES
	"src/HorizonEngine/App.cpp"
	"src/HorizonEngine/Window.cpp"
	"src/HorizonEngine/GameObject.cpp"
	"src/HorizonEngine/Utils/time.cpp"
	"src/HorizonEngine/Utils/Math.cpp"
	"src/HorizonEngine/Components/Transform.cpp"
	"platform/windows/MSWindow.cpp"
 "src/HorizonEngine/Logging/Logging.cpp")

set(HZN_HEADER_FILES
	"HorizonEngine.h"
	"pch.h"
	"src/HorizonEngine/APIMacros.h"
	"src/HorizonEngine/App.h"
	"src/HorizonEngine/EntryPoint.h"
	"src/HorizonEngine/Window.h"
	"src/HorizonEngine/GameObject.h"
	"src/HorizonEngine/Utils/Math.h"
	"src/HorizonEngine/Utils/Time.h"
	"src/HorizonEngine/Physics2D/RigidBody2D.h"
	"src/HorizonEngine/Components/Component.h"
	"src/HorizonEngine/Components/Transform.h"
	"platform/windows/MSWindow.h"
	"src/HorizonEngine/Logging/Logging.h"
 "src/HorizonEngine/Events/Event.h" "src/HorizonEngine/Events/MouseEvent.h" "src/HorizonEngine/Events/KeyEvent.h" "src/HorizonEngine/Events/ApplicationEvent.h" "src/HorizonEngine/Codes/MouseCodes.h" "src/HorizonEngine/Codes/KeyboardCodes.h")

# Add source to this project's executable.
add_library (HorizonEngine ${HZN_SOURCE_FILES} ${HZN_HEADER_FILES})

# compile definitions for 'HorizonEngine' target
target_compile_definitions(HorizonEngine
	PUBLIC
		HZN_PLATFORM_WINDOWS
		HZN_BUILD
)


# directories on include search path for 'HorizonEngine' target
target_include_directories(HorizonEngine
	PUBLIC
	.
	src
	src/HorizonEngine
	platform
)

# precompiled headers to target 'HorizonEngine'
target_precompile_headers(HorizonEngine PUBLIC pch.h)

find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(box2d CONFIG REQUIRED)
find_package(unofficial-sajson CONFIG REQUIRED)

target_link_libraries(HorizonEngine 
	PUBLIC 
	glfw
	glm::glm
	glad::glad
	imgui::imgui
	spdlog::spdlog_header_only
	box2d::box2d
	unofficial::sajson::sajson
)

#testing
add_executable(SampleTest "tests/SampleTest.cpp")
target_include_directories(SampleTest
	PUBLIC
	.
	src
	src/HorizonEngine
	platform
)

add_executable(SampleTest2 "tests/SampleTest2.cpp")
target_include_directories(SampleTest2
	PUBLIC
	.
	src
	src/HorizonEngine
	platform
)

target_link_libraries(SampleTest PUBLIC GTest::gtest_main)
target_link_libraries(SampleTest2 PUBLIC GTest::gtest_main)

gtest_discover_tests(SampleTest)
gtest_discover_tests(SampleTest2)

# post build commands to copy resources from source to build directories for the HorizonEngine target
add_custom_command(
	TARGET HorizonEngine
	POST_BUILD
	COMMAND ${CMAKE_COMMAND}
	ARGS -E copy_directory
	${CMAKE_CURRENT_LIST_DIR}/resources
	$<TARGET_FILE_DIR:HorizonEngine>/resources
)

message(STATUS ${CMAKE_BINARY_DIR})
message(STATUS ${CMAKE_CURRENT_BINARY_DIR})
# TODO: Add tests and install targets if needed.
