# CMakeList.txt : CMake project for HznApplication, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project(HznApplication)

set(HZAPP_SRC_FILES
	"HznApp.cpp"
 "HznApp.h")

# 'HznApplication' target which would generate an executable
add_executable(HznApplication ${HZAPP_SRC_FILES} "HznApp.h")

# libraries linked to HorizonEngine
target_link_libraries(HznApplication PUBLIC HorizonEngine)

# Add the test executables here.
# For every new 'test file' add into the three sections below

# add a test executable using 'add_executable' command
add_executable (AnotherTest
	"tests/AnotherTest.cpp"
)

# link the 'test targets' to the test library (gtest_main)
target_link_libraries(AnotherTest gtest_main)

# add the new test executable fo
gtest_discover_tests(AnotherTest)

# post build command for copying DLL files related to 'HorizonEngine'
# to 'HznApplication'
add_custom_command(
	TARGET HznApplication
	POST_BUILD
	COMMAND ${CMAKE_COMMAND}
	ARGS -E copy_directory
	${CMAKE_BINARY_DIR}/HorizonEngine/$<CONFIG>
	$<TARGET_FILE_DIR:HznApplication>
)

# add_custom_command(
# 	TARGET HznApplication
# 	POST_BUILD
# 	COMMAND ${CMAKE_COMMAND}
# 	ARGS -E copy_directory
# 	${CMAKE_CURRENT_LIST_DIR}/resources
# 	$<TARGET_FILE_DIR:HznApplication>/resources
# )