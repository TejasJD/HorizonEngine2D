# CMakeList.txt : CMake project for HorizonGame, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.15)

set(HZGAME_SOURCE_FILES
	"src/Main.cpp"
	"src/GameLayer.cpp"
 )

set(HZGAME_HEADER_FILES
	"include/GameLayer.h"
)

# 'HznApplication' target which would generate an executable.
add_executable(HorizonGame ${HZGAME_SOURCE_FILES} ${HZGAME_HEADER_FILES})

# libraries linked to HorizonEngine.
target_link_libraries(HorizonGame PUBLIC HorizonEngine)

target_include_directories(HorizonGame PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)

# post build command for copying DLL files related to 'HorizonEngine'
# to 'HorizonGame'.
# add_custom_command(
# 	TARGET HorizonGame
# 	POST_BUILD
# 	COMMAND ${CMAKE_COMMAND}
# 	ARGS -E copy_directory
# 	${CMAKE_BINARY_DIR}/HorizonEngine/$<CONFIG>
# 	$<TARGET_FILE_DIR:HorizonGame>
# )

add_custom_command(
	TARGET HorizonGame
	POST_BUILD
	COMMAND ${CMAKE_COMMAND}
	ARGS -E copy
	$<TARGET_FILE_DIR:HorizonEngine>/mono-2.0-sgen.dll
	$<TARGET_FILE_DIR:HorizonGame>/mono-2.0-sgen.dll
)

add_custom_command(
	TARGET HorizonGame
	POST_BUILD
	COMMAND ${CMAKE_COMMAND}
	ARGS -E copy_directory
	${CMAKE_CURRENT_LIST_DIR}/mono/lib
	$<TARGET_FILE_DIR:HorizonGame>/../lib
)

add_custom_command(
	TARGET HorizonGame
	POST_BUILD
	COMMAND ${CMAKE_COMMAND}
	ARGS -E copy_directory
	${CMAKE_CURRENT_LIST_DIR}/Scripts
	$<TARGET_FILE_DIR:HorizonGame>/Scripts
)

add_custom_command(
	TARGET HorizonGame
	POST_BUILD
	COMMAND ${CMAKE_COMMAND}
	ARGS -E copy_directory
	${CMAKE_CURRENT_LIST_DIR}/assets
	$<TARGET_FILE_DIR:HorizonGame>/assets
)

add_custom_command(
	TARGET HorizonGame
	POST_BUILD
	COMMAND ${CMAKE_COMMAND}
	ARGS -E copy_directory
	$<TARGET_FILE_DIR:HorizonEngine>/api_assets
	$<TARGET_FILE_DIR:HorizonGame>/api_assets
)