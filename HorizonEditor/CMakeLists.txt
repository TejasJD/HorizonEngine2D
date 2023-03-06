# CMakeList.txt : CMake project for HznApplication, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.15)

set(HZNEDITOR_SOURCE_FILES
	
	"src/Main.cpp"
	"src/EditorLayer.cpp"
	"src/ContentBrowser/ContentBrowser.cpp"
	"src/Modals/Modals.cpp"
)

set(HZNEDITOR_HEADER_FILES
	"src/pch.h"
	"include/EditorLayer.h"
	"include/NodeEditor.h"
	"include/Modals.h"
	"include/ContentBrowser.h"
	"include/ComponentDisplays.h"
)

add_executable(HznEditor ${HZNEDITOR_SOURCE_FILES} ${HZNEDITOR_HEADER_FILES} ${WIN32_RESOURCES})

# libraries linked to HorizonEngine.
target_link_libraries(
	HznEditor
	PRIVATE
	project_options
	project_warnings
)

target_link_system_libraries(
	HznEditor
	PRIVATE
	HorizonEngine
)

target_precompile_headers(HznEditor PRIVATE src/pch.h)

target_include_directories(
	HznEditor 
	PRIVATE
	"${CMAKE_CURRENT_LIST_DIR}/include"
	"${CMAKE_CURRENT_LIST_DIR}/src"
)

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
	${CMAKE_CURRENT_LIST_DIR}/icons
	$<TARGET_FILE_DIR:HznEditor>/icons
)

add_custom_command(
	TARGET HznEditor
	POST_BUILD
	COMMAND ${CMAKE_COMMAND}
	ARGS -E copy_directory
	${CMAKE_CURRENT_LIST_DIR}/mono/lib
	$<TARGET_FILE_DIR:HznEditor>/../lib
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

add_custom_command(
	TARGET HznEditor
	POST_BUILD
	COMMAND ${CMAKE_COMMAND}
	ARGS -E copy
	${CMAKE_CURRENT_LIST_DIR}/Scripts/ScriptCoreLib.dll
	$<TARGET_FILE_DIR:HznEditor>/Scripts/ScriptCoreLib.dll
)

add_custom_command(
	TARGET HznEditor
	POST_BUILD
	COMMAND ${CMAKE_COMMAND}
	ARGS -E copy
	${CMAKE_CURRENT_LIST_DIR}/premake5.exe
	$<TARGET_FILE_DIR:HznEditor>/premake5.exe
)

add_custom_command(
	TARGET HznEditor
	POST_BUILD
	COMMAND ${CMAKE_COMMAND}
	ARGS -E copy
	${CMAKE_CURRENT_LIST_DIR}/utils/premake5.lua
	$<TARGET_FILE_DIR:HznEditor>/premake5.lua
)

target_compile_definitions(
	HznEditor
	PRIVATE
	_CRT_SECURE_NO_WARNINGS
)