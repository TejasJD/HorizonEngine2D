# CMakeList.txt : CMake project for HorizonEngine, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (HorizonEngine "HorizonEngine.cpp" "HorizonEngine.h" "HorizonEngine/Window.cpp" "HorizonEngine/Window.h")

# find packages that might exist on the system
find_package(OpenGL REQUIRED)

# ---------- SUBPROJECTS ----------
# add subprojects in this section

# glm
add_subdirectory(extern/glm)

# spdlog logging library
add_subdirectory(extern/spdlog)

# imgui directory
add_subdirectory(extern/imgui)
# ----------------------------------

# ---------- DEPENDENCIES ----------
target_link_libraries(HorizonEngine
	PRIVATE 
		spdlog::spdlog
		imgui 
		glm::glm 
		${OPENGL_gl_LIBRARY}
)
# ----------------------------------

# TODO: Add tests and install targets if needed.
