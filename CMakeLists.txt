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

# Include sub-projects.
add_subdirectory ("HorizonEngine")
add_subdirectory ("HorizonEditor")