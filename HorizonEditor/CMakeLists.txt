# CMakeList.txt : CMake project for HznApplication, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.15)

set(HZNEDITOR_SOURCE_FILES
	"src/Main.cpp"
	"src/EditorLayer.cpp"
	"src/ContentBrowser/ContentBrowser.cpp"
	"src/Modals/Modals.cpp")

set(HZNEDITOR_HEADER_FILES
	"include/EditorLayer.h"
	"include/NodeEditor.h"
	"include/Modals.h"
	"include/ContentBrowser.h"
	"include/ComponentDisplays.h")

set(HZNEDITOR_ICON ${CMAKE_CURRENT_LIST_DIR}/icons/HorizonGameEngine.ico)
# 'HznApplication' target which would generate an executable.

# icon
message("Cmake System Name: " ${CMAKE_SYSTEM_NAME})
if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    enable_language("RC")
    set (WIN32_RESOURCES ${CMAKE_CURRENT_LIST_DIR}/icons/HorizonGameEngine.rc)
endif()

add_executable(HznEditor ${HZNEDITOR_SOURCE_FILES} ${HZNEDITOR_HEADER_FILES} ${WIN32_RESOURCES})

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
	$<TARGET_FILE_DIR:HorizonEngine>/mono-2.0-sgen.dll
	$<TARGET_FILE_DIR:HznEditor>/mono-2.0-sgen.dll
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

message("Icon file path: " ${CMAKE_CURRENT_LIST_DIR}/icons/HorizonGameEngine.ico)
set(CPACK_WIX_PRODUCT_ICON ${WIN32_RESOURCES})

include(InstallRequiredSystemLibraries)
install(TARGETS HznEditor DESTINATION bin)
install(DIRECTORY "mono/lib" DESTINATION .)
install(DIRECTORY "assets" DESTINATION bin)
install(DIRECTORY "icons" DESTINATION bin)
install(DIRECTORY "scripts" DESTINATION bin)
install(FILES "premake5.exe" "premake5.lua" DESTINATION bin)