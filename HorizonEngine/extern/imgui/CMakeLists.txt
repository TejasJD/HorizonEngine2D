cmake_minimum_required(VERSION 3.8)

# ---------- SUBPROJECTS ----------
# add subprojects in this section
add_subdirectory(GLFW)
# ---------------------------------

# source files variable
set(IMGUI_SRC_FILES
    src/imgui_demo.cpp
    src/imgui_draw.cpp
    src/imgui_tables.cpp
    src/imgui_widgets.cpp
    src/imgui.cpp
    src/imgui_impl_glfw.cpp
    src/imgui_impl_opengl3.cpp
)

# target
add_library(imgui STATIC ${IMGUI_SRC_FILES})

# ---------- INCLUDE DIRECTORIES ----------
target_include_directories(imgui
    PUBLIC
        include
    PRIVATE
        include/imgui
)
# ---------------------------------

# ---------- DEPENDENCIES ----------
target_link_libraries(imgui
    PUBLIC
        glfw
)
# ---------------------------------
