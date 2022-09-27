# CMakeList.txt : CMake project for HorizonEngine, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project(HorizonEngine)

set(HZN_SRC_FILES
	"src/HorizonEngine/App.cpp"
)

# Add source to this project's executable.
add_library (HorizonEngine ${HZN_SRC_FILES})

# glm
add_subdirectory(extern/glm)

# spdlog logging library
add_subdirectory(extern/spdlog)

# imgui directory
add_subdirectory(extern/imgui)

# compile definitions for 'HorizonEngine' target
target_compile_definitions(HorizonEngine PUBLIC HZN_PLATFORM_WINDOWS HZN_BUILD)


# directories on include search path for 'HorizonEngine' target
target_include_directories(HorizonEngine
	PUBLIC . src
)

# precompiled headers to target 'HorizonEngine'
target_precompile_headers(HorizonEngine PUBLIC pch.h)

# external libraries linked to 'HorizonEngine' target
target_link_libraries(HorizonEngine
	PUBLIC
		spdlog::spdlog_header_only
		imgui 
		glm::glm 
		opengl32.lib
)

# Add the test executables here.
# For every new 'test file' add into the three sections below

# add a test executable using 'add_executable' command
add_executable (SampleTest
	"tests/SampleTest.cpp"
)
add_executable(SampleTest2
	"tests/SampleTest2.cpp"
)

# link the 'test targets' to the test library (gtest_main)
target_link_libraries(SampleTest gtest_main)
target_link_libraries(SampleTest2 gtest_main)

# add the new test executable fo
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
