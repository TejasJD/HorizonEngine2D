# CMakeList.txt : CMake project for HorizonEngine, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.15)

set(HZN_SOURCE_FILES
	"src/pch.cpp"
	"src/HorizonEngine/AssetManagement/AssetManager.cpp"
	"src/HorizonEngine/Audio/AudioSource.cpp"
	"src/HorizonEngine/Audio/SoundDevice.cpp"
	"src/HorizonEngine/Audio/SoundBuffer.cpp"
	"src/HorizonEngine/Camera/CameraController.cpp"
	"src/HorizonEngine/Core/App.cpp"
	"src/HorizonEngine/Core/LayerStack.cpp"
	"src/HorizonEngine/Core/Window.cpp"
	"src/HorizonEngine/ImGui/ImguiLayer.cpp"
	"src/HorizonEngine/Logging/Logging.cpp"
	"src/HorizonEngine/Physics2D/ContactListener.cpp"
	"src/HorizonEngine/Project/Project.cpp"
	"src/HorizonEngine/Project/ProjectManager.cpp"
	"src/HorizonEngine/Renderer/Buffer.cpp"
	"src/HorizonEngine/Renderer/Framebuffer.cpp"
	"src/HorizonEngine/Renderer/RenderCall.cpp"
	"src/HorizonEngine/Renderer/RenderContext.cpp"
	"src/HorizonEngine/Renderer/Renderer.cpp"
	"src/HorizonEngine/Renderer/Renderer2D.cpp"
	"src/HorizonEngine/Renderer/RendererAPI.cpp"
	"src/HorizonEngine/Renderer/Shader.cpp"
	"src/HorizonEngine/Renderer/Sprite.cpp"
	"src/HorizonEngine/Renderer/Texture.cpp"
	"src/HorizonEngine/Renderer/VertexArray.cpp"
	"src/HorizonEngine/Scene/Component.cpp"
	"src/HorizonEngine/Scene/FunctionRegistry.cpp"
	"src/HorizonEngine/Scene/GameObject.cpp"
	"src/HorizonEngine/Scene/Scene.cpp"
	"src/HorizonEngine/Scene/SceneManager.cpp"
	"src/HorizonEngine/Scripting/ScriptEngine.cpp"
	"src/HorizonEngine/Scripting/ScriptRegistry.cpp"
	"Platform/OpenGL/FDWindows.cpp"
	"Platform/OpenGL/GLBuffer.cpp"
	"Platform/OpenGL/GLContext.cpp"
	"Platform/OpenGL/GLFramebuffer.cpp"
	"Platform/OpenGL/GLInput.cpp"
	"Platform/OpenGL/GLRendererAPI.cpp"
	"Platform/OpenGL/GLShader.cpp"
	"Platform/OpenGL/GLTexture.cpp"
	"Platform/OpenGL/GLVertexArray.cpp"
	"Platform/OpenGL/GLWindow.cpp"
	"vendor/stb/stbimage.cpp"
)

set(HZN_HEADER_FILES
	"include/HorizonEngine/HorizonEngine.h"
	"src/pch.h"
	"include/HorizonEngine/Core/Core.h"
	"include/HorizonEngine/Core/App.h"
	"include/HorizonEngine/Core/Window.h"
	"include/HorizonEngine/Scene/Scene.h"
	"src/HorizonEngine/Physics2D/ContactListener.h"
	"Platform/OpenGL/GLWindow.h"
	"include/HorizonEngine/Logging/Logging.h"
	"include/HorizonEngine/Events/Event.h"
	"include/HorizonEngine/Events/MouseEvent.h"
	"include/HorizonEngine/Events/KeyEvent.h"
	"include/HorizonEngine/Events/ApplicationEvent.h"
	"include/HorizonEngine/Codes/MouseCodes.h"
	"include/HorizonEngine/Codes/KeyboardCodes.h"
	"include/HorizonEngine/Core/LayerStack.h"
	"include/HorizonEngine/Core/Layer.h"
	"include/HorizonEngine/Core/Core.h"
	"include/HorizonEngine/ImGui/ImguiLayer.h"
	"src/HorizonEngine/Audio/SoundDevice.h"
	"src/HorizonEngine/Audio/SoundBuffer.h"
	"src/HorizonEngine/Audio/AudioSource.h"
	"include/HorizonEngine/Core/Input.h"
	"include/HorizonEngine/Renderer/RenderContext.h"
	"Platform/OpenGL/GLContext.h"
	"include/HorizonEngine/Renderer/Shader.h"
	"Platform/OpenGL/GLShader.h"
	"include/HorizonEngine/Renderer/Buffer.h"
	"Platform/OpenGL/GLBuffer.h"
	"include/HorizonEngine/Renderer/Renderer.h"
	"Platform/OpenGL/GLRenderer.h"
	"include/HorizonEngine/Renderer/VertexArray.h"
	"Platform/OpenGL/GLVertexArray.h"
	"include/HorizonEngine/HznEntryPoint.h"
	"include/HorizonEngine/Renderer/RendererAPI.h"
	"include/HorizonEngine/Renderer/RenderCall.h"
	"Platform/OpenGL/GLRendererAPI.h"
	"include/HorizonEngine/Platform/FileDialogs.h"
	"include/HorizonEngine/Core/TimeStep.h"
	"Platform/OpenGL/GLTexture.h"
	"include/HorizonEngine/Renderer/Texture.h"
	"include/HorizonEngine/Camera/CameraController.h"
	"include/HorizonEngine/Renderer/Renderer2D.h"
	"include/HorizonEngine/Renderer/Renderer3D.h"
	"include/HorizonEngine/Renderer/Sprite.h"
	"include/HorizonEngine/Renderer/Framebuffer.h"
	"Platform/OpenGL/GLFramebuffer.h"
	"include/HorizonEngine/Scene/GameObject.h"
	"include/HorizonEngine/Scene/SceneManager.h"
	"include/HorizonEngine/AssetManagement/AssetManager.h"
	"include/HorizonEngine/Scene/GameObject.h"
	"include/HorizonEngine/Project/ProjectManager.h"
	"include/HorizonEngine/Project/Project.h"
	"include/HorizonEngine/Scripting/ScriptEngine.h"
	"include/HorizonEngine/Scripting/ScriptReflection.h"
	"src/HorizonEngine/Scripting/ScriptRegistry.h"
	"src/HorizonEngine/Scene/FunctionRegistry.h"
)

set(IMGUIZMO_FILES
	"vendor/ImGuizmo/GraphEditor.cpp"
	"vendor/ImGuizmo/GraphEditor.h"
	"vendor/ImGuizmo/ImCurveEdit.cpp"
	"vendor/ImGuizmo/ImCurveEdit.h"
	"vendor/ImGuizmo/ImGradient.cpp"
	"vendor/ImGuizmo/ImGradient.h"
	"vendor/ImGuizmo/ImGuizmo.cpp"
	"vendor/ImGuizmo/ImGuizmo.h"
	"vendor/ImGuizmo/ImSequencer.cpp"
	"vendor/ImGuizmo/ImSequencer.h"
	"vendor/ImGuizmo/ImZoomSlider.h"
)

set(IMGUI_FILES
	"vendor/imgui/imconfig.h"
	"vendor/imgui/imgui.cpp"
	"vendor/imgui/imgui.h"
	"vendor/imgui/imgui_demo.cpp"
	"vendor/imgui/imgui_draw.cpp"
	"vendor/imgui/imgui_impl_glfw.cpp"
	"vendor/imgui/imgui_impl_glfw.h"
	"vendor/imgui/imgui_impl_opengl3.cpp"
	"vendor/imgui/imgui_impl_opengl3.h"
	"vendor/imgui/imgui_impl_opengl3_loader.h"
	"vendor/imgui/imgui_internal.h"
	"vendor/imgui/imgui_tables.cpp"
	"vendor/imgui/imgui_widgets.cpp"
	"vendor/imgui/imstb_rectpack.h"
	"vendor/imgui/imstb_textedit.h"
	"vendor/imgui/imstb_truetype.h"
)

set(IMNODES_FILES
	"vendor/imnodes/imnodes.cpp"
	"vendor/imnodes/imnodes.h"
	"vendor/imnodes/imnodes_internal.h"
)

set(FILEWATCH_FILES
	"vendor/filewatch/FileWatch.hpp"
)


# Add source to this project's executable.
add_library (HorizonEngine
	${HZN_SOURCE_FILES}
	${HZN_HEADER_FILES}
	${IMGUI_FILES}
	${IMGUIZMO_FILES}
	${IMNODES_FILES}
	${FILEWATCH_FILES}
)

# compile definitions for 'HorizonEngine' target
target_compile_definitions(HorizonEngine
	PUBLIC
	HZN_PLATFORM_WINDOWS
	$<$<CONFIG:Debug>:HZN_ASSERTS_ON>
)


# directories on include search path for 'HorizonEngine' target
target_include_directories(HorizonEngine
	PUBLIC
	"${CMAKE_CURRENT_LIST_DIR}/include"
	vendor/ImGuizmo
	vendor/imnodes
	vendor/mono/include
	vendor/filewatch
	vendor/imgui
	vendor/IconsFontAwesome
	PRIVATE
	"${CMAKE_CURRENT_LIST_DIR}/src"
	"${CMAKE_CURRENT_LIST_DIR}/platform"
)

# precompiled headers to target 'HorizonEngine'
target_precompile_headers(HorizonEngine PRIVATE src/pch.h)

find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(box2d CONFIG REQUIRED)
find_package(unofficial-sajson CONFIG REQUIRED)
find_package(openal CONFIG REQUIRED)
find_package(Sndfile CONFIG REQUIRED)
find_package(EnTT CONFIG REQUIRED)
find_path(STB_INCLUDE_DIRS "stb_image.h")
find_package(cereal CONFIG REQUIRED)

target_link_libraries(HorizonEngine 
	PUBLIC
	# Ws2_32.lib
	# Winmm.lib
	# Version.lib
	# Bcrypt.lib
	glfw
	glm::glm
	glad::glad
	spdlog::spdlog_header_only
	box2d::box2d
	unofficial::sajson::sajson
	OpenAL::OpenAL
	SndFile::sndfile
	EnTT::EnTT
	cereal::cereal
	${CMAKE_CURRENT_SOURCE_DIR}/vendor/mono/lib/$<$<CONFIG:Debug>:dbg>$<$<CONFIG:Release>:rel>/mono-2.0-sgen.lib
	# ${CMAKE_CURRENT_SOURCE_DIR}/vendor/mono/lib/$<$<CONFIG:Debug>:dbg>$<$<CONFIG:Release>:rel>/libmono-static-sgen.lib
)

# post build commands to copy resources from source to build directories for the HorizonEngine target
add_custom_command(
	TARGET HorizonEngine
	POST_BUILD
	COMMAND ${CMAKE_COMMAND}
	ARGS -E copy_directory
	${CMAKE_CURRENT_LIST_DIR}/api_assets
	$<TARGET_FILE_DIR:HorizonEngine>/api_assets
)

add_custom_command(
	TARGET HorizonEngine
	POST_BUILD
	COMMAND ${CMAKE_COMMAND}
	ARGS -E copy_directory
	${CMAKE_CURRENT_LIST_DIR}/vendor/mono/dylib/dbg
	$<TARGET_FILE_DIR:HorizonEngine>
)
# TODO: Add tests and install targets if needed.
message("VCPKG BIN DIR: " ${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/debug/bin)
install(DIRECTORY ${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/debug/bin DESTINATION . CONFIGURATIONS Debug)
install(DIRECTORY ${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/bin DESTINATION . CONFIGURATIONS Release)
install(TARGETS HorizonEngine DESTINATION bin)
install(DIRECTORY "api_assets" DESTINATION bin)
install(FILES "vendor/mono/lib/dbg/mono-2.0-sgen.lib" DESTINATION bin CONFIGURATIONS Debug)
install(FILES "vendor/mono/dylib/dbg/mono-2.0-sgen.dll" DESTINATION bin CONFIGURATIONS Debug)
install(FILES "vendor/mono/lib/rel/mono-2.0-sgen.lib" DESTINATION bin CONFIGURATIONS Release)
install(FILES "vendor/mono/dylib/rel/mono-2.0-sgen.dll" DESTINATION bin CONFIGURATIONS Release)