# CMakeList.txt : CMake project for HznApplication, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.15)

set(HZNEDITOR_SOURCE_FILES
	"src/Main.cpp"
  "src/EditorLayer.cpp"
  "src/NodeEditor/NodeEditor.cpp"
  "src/ContentBrowser/ContentBrowser.cpp"
  "src/Modals/Modals.cpp")

set(HZNEDITOR_HEADER_FILES
 "include/EditorLayer.h"
 "include/NodeEditor.h"
 "include/ContentBrowser.h"
 "include/Modals.h")

# 'HznApplication' target which would generate an executable.
add_executable(HznEditor ${HZNEDITOR_SOURCE_FILES} ${HZNEDITOR_HEADER_FILES})

# libraries linked to HorizonEngine.
target_link_libraries(HznEditor PUBLIC HorizonEngine)

target_include_directories(HznEditor PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)

add_custom_command(
	TARGET HznEditor
	POST_BUILD
	COMMAND ${CMAKE_COMMAND}
	ARGS -E copy_directory
	${CMAKE_CURRENT_LIST_DIR}/assets
	$<TARGET_FILE_DIR:HznEditor>/assets
)

add_custom_command(
	TARGET HznEditor
	POST_BUILD
	COMMAND ${CMAKE_COMMAND}
	ARGS -E copy_directory
	${CMAKE_CURRENT_LIST_DIR}/scenes
	$<TARGET_FILE_DIR:HznEditor>/scenes
)

add_custom_command(
	TARGET HznEditor
	POST_BUILD
	COMMAND ${CMAKE_COMMAND}
	ARGS -E copy_directory
	${CMAKE_CURRENT_LIST_DIR}/icons
	$<TARGET_FILE_DIR:HznEditor>/icons
)

# post build commands to copy resources from source to build directories for the HorizonEngine target
add_custom_command(
	TARGET HznEditor
	POST_BUILD
	COMMAND ${CMAKE_COMMAND}
	ARGS -E copy_directory
	$<TARGET_FILE_DIR:HorizonEngine>/api_assets
	$<TARGET_FILE_DIR:HznEditor>/api_assets
)