#pragma once

#ifndef HZN_HORIZON_ENGINE_H
#define HZN_HORIZON_ENGINE_H
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

// Reference additional headers your program requires here.

#include "HorizonEngine/Core/Core.h"
#include "HorizonEngine/Logging/Logging.h"
#include "HorizonEngine/App.h"

#include "HorizonEngine/Events/Event.h"
#include "HorizonEngine/Events/ApplicationEvent.h"
#include "HorizonEngine/Events/KeyEvent.h"
#include "HorizonEngine/Events/MouseEvent.h"

#include "HorizonEngine/Input.h"
#include "HorizonEngine/Codes/MouseCodes.h"
#include "HorizonEngine/Codes/KeyboardCodes.h"

#include "HorizonEngine/Layer.h"
#include "HorizonEngine/ImGui/ImguiLayer.h"

#include "HorizonEngine/SceneManagement/Scene.h"
#include "HorizonEngine/Components/Component.h"
#include "HorizonEngine/Components/ComponentFactory.h"

#endif