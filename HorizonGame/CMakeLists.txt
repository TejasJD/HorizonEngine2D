# CMakeList.txt : CMake project for HorizonGame, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.15)

set(HZGAME_SOURCE_FILES
	"src/Main.cpp"
	"src/GameLayer.cpp"
 )

set(HZGAME_HEADER_FILES
	"src/GameLayer.h"
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