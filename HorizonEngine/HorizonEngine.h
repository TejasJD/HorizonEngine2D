#pragma once

#ifndef HZN_HORIZON_ENGINE_H
#define HZN_HORIZON_ENGINE_H

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "glm/glm.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/compatibility.hpp"

#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "ImGuizmo.h"

// Reference additional headers your program requires here.
#include "HorizonEngine/Core/Core.h"
#include "HorizonEngine/Logging/Logging.h"
#include "HorizonEngine/Core/TimeStep.h"
#include "HorizonEngine/Events/Event.h"
#include "HorizonEngine/Events/ApplicationEvent.h"
#include "HorizonEngine/Events/KeyEvent.h"
#include "HorizonEngine/Events/MouseEvent.h"

#include "HorizonEngine/Audio/AudioSource.h"
#include "HorizonEngine/Audio/SoundBuffer.h"
#include "HorizonEngine/Audio/SoundDevice.h"

#include "HorizonEngine/Components/Component.h"

#include "HorizonEngine/FileManagement/ProjectFile.h"

#include "HorizonEngine/SceneManagement/GameObject.h"
#include "HorizonEngine/SceneManagement/Scene.h"
#include "HorizonEngine/SceneManagement/SceneManager.h"

#include "HorizonEngine/Input.h"
#include "HorizonEngine/Codes/MouseCodes.h"
#include "HorizonEngine/Codes/KeyboardCodes.h"

#include "HorizonEngine/Utils/Math.h"
#include "HorizonEngine/Utils/Time.h"
#include "HorizonEngine/Utils/Rect.h"
#include "HorizonEngine/Utils/MaxRectsBinPack.h"
#include "HorizonEngine/Utils/SpriteSheetGenerator.h"


#include "HorizonEngine/AssetManagement/AssetManager.h"

#include "HorizonEngine/Layer.h"

#include "HorizonEngine/Renderer/RenderCall.h"
#include "HorizonEngine/Renderer/Renderer.h"
#include "HorizonEngine/Renderer/Renderer2D.h"
#include "HorizonEngine/Renderer/Renderer3D.h"
#include "HorizonEngine/Renderer/Buffer.h"
#include "HorizonEngine/Renderer/Shader.h"
#include "HorizonEngine/Renderer/VertexArray.h"
#include "HorizonEngine/Renderer/Texture.h"
#include "HorizonEngine/Renderer/Sprite.h"
#include "HorizonEngine/Renderer/FrameBuffer.h"

#include "HorizonEngine/Camera/Camera.h"
#include "HorizonEngine/Camera/CameraController.h"

#include "Platform/OpenGL/FDWindows.h"
#include "HorizonEngine/ImGui/ImguiLayer.h" 
#include "HorizonEngine/App.h"
#endif // !HZN_HORIZONENGINE_H
