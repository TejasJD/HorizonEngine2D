#pragma once

#ifndef HZN_HORIZONENGINE_H
#define HZN_HORIZON_ENGINE_H

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "glm/glm.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

// Reference additional headers your program requires here.

#include "HorizonEngine/Core/Core.h"
#include "HorizonEngine/Logging/Logging.h"
#include "HorizonEngine/Core/TimeStep.h"
#include "HorizonEngine/Events/Event.h"
#include "HorizonEngine/Events/ApplicationEvent.h"
#include "HorizonEngine/Events/KeyEvent.h"
#include "HorizonEngine/Events/MouseEvent.h"

#include "HorizonEngine/Input.h"
#include "HorizonEngine/Codes/MouseCodes.h"
#include "HorizonEngine/Codes/KeyboardCodes.h"


#include "HorizonEngine/Layer.h"
#include "HorizonEngine/Renderer/Renderer.h"
#include "HorizonEngine/Renderer/RenderCall.h"
#include "HorizonEngine/Renderer/Buffer.h"
#include "HorizonEngine/Renderer/Shader.h"
#include "HorizonEngine/Renderer/VertexArray.h"
#include "HorizonEngine/Renderer/Texture.h"
#include "HorizonEngine/Camera.h"
#include "Platform/OpenGL/FDWindows.h"
#include "HorizonEngine/ImGui/ImguiLayer.h" 
#include "HorizonEngine/App.h"
#endif // !HZN_HORIZONENGINE_H
