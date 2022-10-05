#include "pch.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "HorizonEngine/Events/MouseEvent.h"
#include "HorizonEngine/Events/KeyEvent.h"
#include "HorizonEngine/Events/ApplicationEvent.h"
#include "HorizonEngine/Layer.h"
#include "HorizonEngine/App.h"

#include "ImguiLayer.h"

namespace Hzn
{
	ImguiLayer::ImguiLayer() : Layer("ImGuiLayer")
	{

	}

	ImguiLayer::~ImguiLayer()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui::DestroyContext();
	}
	void ImguiLayer::onAttach()
	{
		HZN_CORE_INFO("Imgui Layer Attached!");
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGuiIO& io = ImGui::GetIO();
		
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

		io.KeyMap[ImGuiKey_Backspace] = Key::Backspace;
		io.KeyMap[ImGuiKey_Enter] = Key::Enter;
		io.KeyMap[ImGuiKey_Escape] = Key::Escape;
		io.KeyMap[ImGuiKey_Space] = Key::Space;
		
		ImGui_ImplOpenGL3_Init("#version 410");
	}
	void ImguiLayer::onDetach()
	{
	}
	void ImguiLayer::onUpdate()
	{
		/*HZN_CORE_TRACE("Imgui Layer Update!");*/
		ImGui_ImplOpenGL3_NewFrame();

		ImGuiIO& io = ImGui::GetIO();
		App& app = App::getApp();

		float time = (float)glfwGetTime();
		io.DeltaTime = m_Time > 0.0 ? (time - m_Time) : (1.0f / 144.0f);
		io.DisplaySize = ImVec2(app.getAppWindow().getWidth(), app.getAppWindow().getHeight());

		ImGui::NewFrame();
		
		static bool show = true;
		ImGui::ShowDemoWindow(&show);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
	void ImguiLayer::onEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseScrolledEvent>(std::bind(&ImguiLayer::onMouseScroll, this, std::placeholders::_1));
		dispatcher.Dispatch<MouseButtonPressedEvent>(std::bind(&ImguiLayer::onMousePressed, this, std::placeholders::_1));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(std::bind(&ImguiLayer::onMouseReleased, this, std::placeholders::_1));
		dispatcher.Dispatch<MouseMovedEvent>(std::bind(&ImguiLayer::onMouseMoved, this, std::placeholders::_1));
		dispatcher.Dispatch<KeyPressedEvent>(std::bind(&ImguiLayer::onKeyPressed, this, std::placeholders::_1));
		dispatcher.Dispatch<KeyPressedEvent>(std::bind(&ImguiLayer::onKeyRepeat, this, std::placeholders::_1));
		dispatcher.Dispatch<KeyReleasedEvent>(std::bind(&ImguiLayer::onKeyReleased, this, std::placeholders::_1));	
		dispatcher.Dispatch<KeyTypedEvent>(std::bind(&ImguiLayer::onKeyTyped, this, std::placeholders::_1));	
		dispatcher.Dispatch<WindowResizeEvent>(std::bind(&ImguiLayer::onWindowResize, this, std::placeholders::_1));	
	}
	
	bool ImguiLayer::onMousePressed(MouseButtonPressedEvent& event)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[event.GetMouseButton()] = true;
		return false;
	}

	bool ImguiLayer::onMouseReleased(MouseButtonReleasedEvent& event)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[event.GetMouseButton()] = false;
		return false;
	}

	bool ImguiLayer::onMouseScroll(MouseScrolledEvent& event)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseWheelH += event.GetXOffset();
		io.MouseWheel += event.GetYOffset();
		return false;
	}

	bool ImguiLayer::onKeyPressed(KeyPressedEvent& event)
	{
		HZN_CORE_TRACE(event.ToString());
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[event.GetKeyCode()] = true;
		io.KeyCtrl = io.KeysDown[Key::LeftControl] || io.KeysDown[Key::RightControl];
		io.KeyAlt = io.KeysDown[Key::LeftAlt] || io.KeysDown[Key::RightAlt];
		io.KeyShift = io.KeysDown[Key::LeftShift] || io.KeysDown[Key::RightShift];
		io.KeySuper = io.KeysDown[Key::LeftSuper] || io.KeysDown[Key::RightSuper];
		return false;
	}
	
	bool ImguiLayer::onKeyReleased(KeyReleasedEvent& event)
	{
		HZN_CORE_TRACE(event.ToString());
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[event.GetKeyCode()] = false;
		return false;
	}
	
	bool ImguiLayer::onKeyRepeat(KeyPressedEvent& event)
	{
		HZN_CORE_TRACE(event.ToString());
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[event.GetKeyCode()] = true;
		io.KeyRepeatRate = m_Time;
		return false;
	}

	bool ImguiLayer::onMouseMoved(MouseMovedEvent& event)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MousePos = ImVec2(event.GetX(), event.GetY());
		return false;
	}
	
	bool ImguiLayer::onKeyTyped(KeyTypedEvent& event)
	{
		ImGuiIO& io = ImGui::GetIO();
		int c = event.GetKeyCode();
		if (c > 0 && c < 0x10000)
		{
			io.AddInputCharacter((unsigned short)c);
		}
		return false;
	}

	bool ImguiLayer::onWindowResize(WindowResizeEvent& event)
	{
		ImGuiIO& io = ImGui::GetIO();
		unsigned int width = event.GetWidth();
		unsigned int height = event.GetHeight();
		io.DisplaySize = ImVec2(width, height);
		glViewport(0, 0, width, height);

		return false;
	}
	
}