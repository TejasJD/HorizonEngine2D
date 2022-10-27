# CMakeList.txt : CMake project for HznApplication, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.15)

project(HznApplication)

set(HZAPP_SOURCE_FILES
	"HznApp.cpp"
	"Sandbox.cpp"
 )

set(HZAPP_HEADER_FILES
	"HznApp.h"
	"Sandbox.h"
)

# 'HznApplication' target which would generate an executable.
add_executable(HznApplication ${HZAPP_SOURCE_FILES} ${HZN_HEADER_FILES})

# libraries linked to HorizonEngine.
target_link_libraries(HznApplication PUBLIC HorizonEngine)

# testing
add_executable(AnotherTest "tests/AnotherTest.cpp")
add_executable(AnotherTest2 "tests/AnotherTest2.cpp")

target_link_libraries(AnotherTest PUBLIC GTest::gtest_main)
target_link_libraries(AnotherTest2 PUBLIC GTest::gtest_main)

gtest_discover_tests(AnotherTest)
gtest_discover_tests(AnotherTest2)

# post build command for copying DLL files related to 'HorizonEngine'
# to 'HznApplication'.
# add_custom_command(
# 	TARGET HznApplication
# 	POST_BUILD
# 	COMMAND ${CMAKE_COMMAND}
# 	ARGS -E copy_directory
# 	${CMAKE_BINARY_DIR}/HorizonEngine/$<CONFIG>
# 	$<TARGET_FILE_DIR:HznApplication>
# )

add_custom_command(
	TARGET HznApplication
	POST_BUILD
	COMMAND ${CMAKE_COMMAND}
	ARGS -E copy_directory
	$<TARGET_FILE_DIR:HorizonEngine>/api_assets
	$<TARGET_FILE_DIR:HznApplication>/api_assets
)

add_custom_command(
	TARGET HznApplication
	POST_BUILD
	COMMAND ${CMAKE_COMMAND}
	ARGS -E copy_directory
	${CMAKE_CURRENT_LIST_DIR}/assets
	$<TARGET_FILE_DIR:HznApplication>/assets
)