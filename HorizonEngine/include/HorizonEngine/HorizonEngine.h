#pragma once

#ifndef HZN_HORIZON_ENGINE_H
#define HZN_HORIZON_ENGINE_H

#include "glm/glm.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/compatibility.hpp"

#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "ImGuizmo.h"
#include "imnodes.h"
#include "IconsFontAwesome5.h"

// Reference additional headers your program requires here.
#include "HorizonEngine/Core/Core.h"
#include "HorizonEngine/Logging/Logging.h"
#include "HorizonEngine/AssetManagement/AssetManager.h"
#include "HorizonEngine/Core/TimeStep.h"
#include "HorizonEngine/Events/Event.h"
#include "HorizonEngine/Events/ApplicationEvent.h"
#include "HorizonEngine/Events/KeyEvent.h"
#include "HorizonEngine/Events/MouseEvent.h"

#include "HorizonEngine/Scene/Component.h"

#include "HorizonEngine/Project/Project.h"
#include "HorizonEngine/Project/ProjectManager.h"

#include "HorizonEngine/Scene/GameObject.h"
#include "HorizonEngine/Scene/Scene.h"
#include "HorizonEngine/Scene/SceneManager.h"

#include "HorizonEngine/Core/Input.h"
#include "HorizonEngine/Core/Layer.h"

#include "HorizonEngine/Codes/MouseCodes.h"
#include "HorizonEngine/Codes/KeyboardCodes.h"

#include "HorizonEngine/Renderer/RenderCall.h"
#include "HorizonEngine/Renderer/Renderer.h"
#include "HorizonEngine/Renderer/Renderer2D.h"
#include "HorizonEngine/Renderer/Renderer3D.h"
#include "HorizonEngine/Renderer/Buffer.h"
#include "HorizonEngine/Renderer/Shader.h"
#include "HorizonEngine/Renderer/VertexArray.h"
#include "HorizonEngine/Renderer/Texture.h"
#include "HorizonEngine/Renderer/Sprite.h"
#include "HorizonEngine/Renderer/Framebuffer.h"

#include "HorizonEngine/Camera/Camera.h"
#include "HorizonEngine/Camera/CameraController.h"

#include "HorizonEngine/Platform/FileDialogs.h"
#include "HorizonEngine/Core/App.h"

#include "HorizonEngine/Scripting/ScriptEngine.h"
#endif  // !HZN_HORIZONENGINE_H
