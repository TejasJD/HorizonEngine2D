# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.15)

project ("HorizonGameEngine")

# c++ standard for our project
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

find_package(GTest CONFIG REQUIRED)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
enable_testing()

include(GoogleTest)

include(InstallRequiredSystemLibraries)

set(CPACK_PACKAGE_NAME "HorizonGameEngine")
set(CPACK_PACKAGE_VENDOR "Tejas Dusane")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Get Horizon Game Engine")
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "HorizonEngine")
set(CMAKE_INSTALL_DEFAULT_DIRECTORY_PERMISSIONS
	OWNER_READ
	OWNER_WRITE
	OWNER_EXECUTE
	GROUP_READ
	GROUP_WRITE
	GROUP_EXECUTE
	WORLD_READ
	WORLD_WRITE
	WORLD_EXECUTE
	SETUID
	SETGID
)

# Include sub-projects.
add_subdirectory ("HorizonEngine")
add_subdirectory ("HorizonEditor")

include(CPack)