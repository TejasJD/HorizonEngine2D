# CMakeList.txt : CMake project for HorizonEngine, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add dependencies related to Horizon Engine

# glfw
add_subdirectory(vendor/GLFW)

# spdlog logging library
add_subdirectory(vendor/spdlog)

# Add source to this project's executable.
add_executable (HorizonEngine "HorizonEngine.cpp" "HorizonEngine.h")

# linking libraries
target_link_libraries(HorizonEngine 
	PRIVATE 
		glfw
		spdlog::spdlog
)

# TODO: Add tests and install targets if needed.
