# CMakeList.txt : CMake project for HorizonEngine, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.15)

project(HorizonEngine)

set(HZN_SOURCE_FILES
	"src/HorizonEngine/App.cpp"
	"src/HorizonEngine/GameObject.cpp"
	"src/HorizonEngine/Utils/Time.cpp"
	"src/HorizonEngine/Utils/Math.cpp"
	"src/HorizonEngine/Components/Transform.cpp"
	"Platform/Windows/MSWindow.cpp"
	"src/HorizonEngine/Logging/Logging.cpp"
	"src/HorizonEngine/LayerStack.cpp"
	"src/HorizonEngine/Layer.cpp"
	"src/HorizonEngine/Imgui/ImguiLayer.cpp"
	"src/HorizonEngine/FileManagement/ProjectFile.cpp"
	"Platform/Windows/MSInput.cpp"
	"src/HorizonEngine/Audio/SoundDevice.cpp"
	"src/HorizonEngine/Audio/SoundBuffer.cpp"
	"src/HorizonEngine/Audio/AudioSource.cpp"
	"src/HorizonEngine/SceneManagement/Scene.cpp"
	"src/HorizonEngine/Physics2D/BoxCollider2D.cpp"
	"src/HorizonEngine/Physics2D/Rigidbody2D.cpp"
	"src/HorizonEngine/Physics2D/PhysicsWorld2D.cpp"
	"platform/windows/MSWindow.cpp"
	"src/HorizonEngine/Logging/Logging.cpp")

set(HZN_HEADER_FILES
	"HorizonEngine.h"
	"pch.h"
	"src/HorizonEngine/Core/Core.h"
	"src/HorizonEngine/App.h"
	"src/HorizonEngine/EntryPoint.h"
	"src/HorizonEngine/Window.h"
	"src/HorizonEngine/GameObject.h"
	"src/HorizonEngine/Utils/Math.h"
	"src/HorizonEngine/Utils/Time.h"
	"src/HorizonEngine/SceneManagement/Scene.h"
	"src/HorizonEngine/Physics2D/RigidBody2D.h"
	"src/HorizonEngine/Components/Component.h"
	"src/HorizonEngine/Components/Transform.h"
	"src/HorizonEngine/Components/ComponentFactory.h"
	"src/HorizonEngine/Physics2D/BodyType.h"
	"src/HorizonEngine/Physics2D/ColliderType.h"
	"src/HorizonEngine/Physics2D/PhysicsMaterial.h"
	"src/HorizonEngine/Physics2D/Collider2D.h"
	"src/HorizonEngine/Physics2D/BoxCollider2D.h"
	"src/HorizonEngine/Physics2D/PhysicsWorld2D.h"
	"platform/windows/MSWindow.h"
	"src/HorizonEngine/Logging/Logging.h"
	"src/HorizonEngine/Events/Event.h"
	"src/HorizonEngine/Events/MouseEvent.h"
	"src/HorizonEngine/Events/KeyEvent.h"
	"src/HorizonEngine/Events/ApplicationEvent.h"
	"src/HorizonEngine/Codes/MouseCodes.h"
	"src/HorizonEngine/Codes/KeyboardCodes.h"
	"src/HorizonEngine/LayerStack.h"
	"src/HorizonEngine/Layer.h"
	"src/HorizonEngine/Core/Core.h"
	"src/HorizonEngine/Imgui/ImguiLayer.h"
	"src/HorizonEngine/FileManagement/ProjectFile.h"
	"src/HorizonEngine/Audio/SoundDevice.h"
	"src/HorizonEngine/Audio/SoundBuffer.h"
	"src/HorizonEngine/Audio/AudioSource.h"
	"src/HorizonEngine/Imgui/ImguiLayer.h"
	"src/HorizonEngine/Input.h"
	"Platform/Windows/MSInput.h")

# Add source to this project's executable.
add_library (HorizonEngine ${HZN_SOURCE_FILES} ${HZN_HEADER_FILES})

# compile definitions for 'HorizonEngine' target
target_compile_definitions(HorizonEngine
	PUBLIC
	HZN_PLATFORM_WINDOWS
	HZN_ASSERTS_ON
	PRIVATE
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
find_package(openal CONFIG REQUIRED)
find_package(Sndfile CONFIG REQUIRED)

target_link_libraries(HorizonEngine 
	PUBLIC
	glfw
	glm::glm
	glad::glad
	imgui::imgui
	spdlog::spdlog_header_only
	box2d::box2d
	unofficial::sajson::sajson
	OpenAL::OpenAL
	SndFile::sndfile
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
