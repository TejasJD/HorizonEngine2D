# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("HorizonGameEngine")

# c++ standard for our project
set(CMAKE_CXX_STANDARD 14)

# adding google test
include(FetchContent)

FetchContent_Declare(
	googletest
	GIT_REPOSITORY https://github.com/google/googletest.git
	GIT_TAG release-1.12.1
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# include google test module
include(GoogleTest)

# enable testing for all sub-projects
enable_testing()
# Include sub-projects.
add_subdirectory ("HorizonEngine")
add_subdirectory ("HznApplication")