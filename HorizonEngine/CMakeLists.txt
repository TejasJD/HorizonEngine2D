# CMakeList.txt : CMake project for HorizonEngine, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.15)

set(HZN_SOURCE_FILES
	"src/HorizonEngine/App.cpp"
	"src/HorizonEngine/Utils/Time.cpp"
	"src/HorizonEngine/Utils/Math.cpp"
	"src/HorizonEngine/Utils/TreeNode.cpp"
	"Platform/OpenGL/GLWindow.cpp"
	"src/HorizonEngine/Logging/Logging.cpp"
	"src/HorizonEngine/LayerStack.cpp"
	"src/HorizonEngine/Layer.cpp"
	"src/HorizonEngine/Imgui/ImguiLayer.cpp"
	"src/HorizonEngine/FileManagement/ProjectFile.cpp"
	"Platform/OpenGL/GLInput.cpp"
	"src/HorizonEngine/Audio/SoundDevice.cpp"
	"src/HorizonEngine/Audio/SoundBuffer.cpp"
	"src/HorizonEngine/Audio/AudioSource.cpp"
	"src/HorizonEngine/SceneManagement/Scene.cpp"
	"Platform/OpenGL/GLWindow.cpp"
	"src/HorizonEngine/Logging/Logging.cpp"
	"Platform/OpenGL/GLContext.cpp"
	"Platform/OpenGL/GLShader.cpp"
	"Platform/OpenGL/GLBuffer.cpp"
	"src/HorizonEngine/Renderer/Buffer.cpp"
	"src/HorizonEngine/Renderer/Shader.cpp"
	"src/HorizonEngine/Renderer/RendererAPI.cpp"
	"src/HorizonEngine/Window.cpp"
	"src/HorizonEngine/Renderer/RenderContext.cpp"
	"Platform/OpenGL/GLVertexArray.cpp"
	"src/HorizonEngine/Renderer/VertexArray.cpp"
	"src/HorizonEngine/Renderer/Renderer.cpp"
	"src/HorizonEngine/Camera/Camera.h"
	"src/HorizonEngine/Renderer/RenderCall.cpp"
	"Platform/OpenGL/GLRendererAPI.cpp"
	"src/HorizonEngine/Camera/Camera.cpp"
	"Platform/OpenGL/FDWindows.cpp"
	"src/HorizonEngine/Renderer/Texture.cpp"
	"Platform/OpenGL/GLTexture.cpp"
	"vendor/stb/stbimage.cpp"
	"src/HorizonEngine/Camera/CameraController.cpp"
	"src/HorizonEngine/Renderer/Renderer2D.cpp"
	"src/HorizonEngine/Renderer/Renderer3D.cpp"
	"src/HorizonEngine/Renderer/Sprite.cpp"
	"src/HorizonEngine/Renderer/FrameBuffer.cpp"
	"Platform/OpenGL/GLFrameBuffer.cpp"
	"src/HorizonEngine/SceneManagement/SceneManager.cpp"
	"src/HorizonEngine/AssetManagement/AssetManager.cpp"
	"src/HorizonEngine/SceneManagement/GameObject.cpp"
	"src/HorizonEngine/FileManagement/ProjectManager.cpp"
	"src/HorizonEngine/FileManagement/Project.cpp"
	"src/HorizonEngine/Scripting/ScriptEngine.cpp"
	"src/HorizonEngine/Scripting/ScriptRegistry.cpp"
	"src/HorizonEngine/Components/Component.cpp"
	"src/HorizonEngine/SceneManagement/FunctionRegistry.cpp"
 )

set(HZN_HEADER_FILES
	"HorizonEngine.h"
	"pch.h"
	"src/HorizonEngine/Core/Core.h"
	"src/HorizonEngine/App.h"
	"src/HorizonEngine/Window.h"
	"src/HorizonEngine/Utils/Math.h"
	"src/HorizonEngine/Utils/Time.h"
	"src/HorizonEngine/Utils/TreeNode.h"
	"src/HorizonEngine/SceneManagement/Scene.h"
	"Platform/OpenGL/GLWindow.h"
	"src/HorizonEngine/Logging/Logging.h"
	"src/HorizonEngine/Events/Event.h"
	"src/HorizonEngine/Events/MouseEvent.h"
	"src/HorizonEngine/Events/KeyEvent.h"
	"src/HorizonEngine/Events/ApplicationEvent.h"
	"src/HorizonEngine/Codes/MouseCodes.h"
	"src/HorizonEngine/Codes/KeyboardCodes.h"
	"src/HorizonEngine/LayerStack.h"
	"src/HorizonEngine/Layer.h"
	"src/HorizonEngine/Core/Core.h"
	"src/HorizonEngine/Imgui/ImguiLayer.h"
	"src/HorizonEngine/FileManagement/ProjectFile.h"
	"src/HorizonEngine/Audio/SoundDevice.h"
	"src/HorizonEngine/Audio/SoundBuffer.h"
	"src/HorizonEngine/Audio/AudioSource.h"
	"src/HorizonEngine/Imgui/ImguiLayer.h"
	"src/HorizonEngine/Input.h"
	"src/HorizonEngine/Renderer/RenderContext.h"
	"Platform/OpenGL/GLContext.h"
	"src/HorizonEngine/Renderer/Shader.h"
	"Platform/OpenGL/GLShader.h"
	"src/HorizonEngine/Renderer/Buffer.h"
	"Platform/OpenGL/GLBuffer.h"
	"src/HorizonEngine/Renderer/Renderer.h"
	"Platform/OpenGL/GLRenderer.h"
	"src/HorizonEngine/Renderer/VertexArray.h"
	"Platform/OpenGL/GLVertexArray.h"
	"HznEntryPoint.h"
	"src/HorizonEngine/Renderer/RendererAPI.h"
	"src/HorizonEngine/Renderer/RenderCall.h"
	"Platform/OpenGL/GLRendererAPI.h"
	"Platform/OpenGL/FDWindows.h"
	"src/HorizonEngine/Core/TimeStep.h"
	"Platform/OpenGL/GLTexture.h"
	"src/HorizonEngine/Renderer/Texture.h"
	"src/HorizonEngine/Camera/CameraController.h"
	"src/HorizonEngine/Renderer/Renderer2D.h"
	"src/HorizonEngine/Renderer/Renderer3D.h"
	"src/HorizonEngine/Renderer/Sprite.h"
	"src/HorizonEngine/Renderer/FrameBuffer.h"
	"Platform/OpenGL/GLFrameBuffer.h"
	"src/HorizonEngine/SceneManagement/GameObject.h"
	"src/HorizonEngine/SceneManagement/SceneManager.h"
	"src/HorizonEngine/AssetManagement/AssetManager.h"
	"src/HorizonEngine/SceneManagement/GameObject.h"
	"src/HorizonEngine/FileManagement/ProjectManager.h"
	"src/HorizonEngine/FileManagement/Project.h"
	"src/HorizonEngine/Scripting/ScriptEngine.h"
	"src/HorizonEngine/Scripting/ScriptRegistry.h"
	"src/HorizonEngine/SceneManagement/FunctionRegistry.h"
	"src/HorizonEngine/Scripting/ScriptReflection.h"
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
	.
	src
	src/HorizonEngine
	platform
	vendor/ImGuizmo
	vendor/imnodes
	vendor/mono/include
	vendor/filewatch
	vendor/imgui
	vendor/IconsFontAwesome
)

# precompiled headers to target 'HorizonEngine'
target_precompile_headers(HorizonEngine PUBLIC pch.h)

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

#testing
add_executable(SampleTest "tests/SampleTest.cpp")
target_include_directories(SampleTest
	PUBLIC
	.
	src
	src/HorizonEngine
	platform
	${STB_INCLUDE_DIRS}
)

add_executable(SampleTest2 "tests/SampleTest2.cpp")
target_include_directories(SampleTest2
	PUBLIC
	.
	src
	src/HorizonEngine
	platform
)

target_link_libraries(SampleTest PUBLIC GTest::gtest_main)
target_link_libraries(SampleTest2 PUBLIC GTest::gtest_main)

gtest_discover_tests(SampleTest)
gtest_discover_tests(SampleTest2)

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

message(STATUS ${CMAKE_BINARY_DIR})
message(STATUS ${CMAKE_CURRENT_BINARY_DIR})
# TODO: Add tests and install targets if needed.
