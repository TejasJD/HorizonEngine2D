#include "pch.h"

#include "HorizonEngine/Events/MouseEvent.h"
#include "HorizonEngine/Events/KeyEvent.h"
#include "HorizonEngine/Events/ApplicationEvent.h"
#include "HorizonEngine/Layer.h"
#include "HorizonEngine/App.h"
#include "HorizonEngine/Renderer/Renderer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define IMGUI_IMPL_API
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <ImGuizmo.h>

#include "ImguiLayer.h"

namespace Hzn
{
	ImguiLayer::ImguiLayer() : Layer("ImGui Layer")
	{
		HZN_CORE_INFO("Imgui Layer Attached!");
	}

	ImguiLayer::~ImguiLayer()
	{
		onDetach();
	}

	void ImguiLayer::onAttach()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		(void)io;

		// turning on certain viewport flags
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();

		// a workaround, so that window rounding is consistent.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		auto window = (GLFWwindow*)App::getApp().getAppWindow().getPlatformRawWindow();
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 420");
	}

	void ImguiLayer::onUpdate(TimeStep ts) {}

	void ImguiLayer::onDetach()
	{
		ImGui_ImplGlfw_Shutdown();
		ImGui_ImplOpenGL3_Shutdown();
		ImGui::DestroyContext();
	}

	void ImguiLayer::imguiBegin()
	{
		ImGui_ImplGlfw_NewFrame();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();
	}

	void ImguiLayer::imguiEnd()
	{
		ImGuiIO& io = ImGui::GetIO();
		GLFWwindow* window = (GLFWwindow*)App::getApp().getAppWindow().getPlatformRawWindow();

		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}

	void ImguiLayer::onEvent(Event& e)
	{
		if(absorbEvents)
		{
			ImGuiIO& io = ImGui::GetIO();
			if (io.WantCaptureKeyboard && e.IsInCategory(EventCategoryKeyboard))
			{
				e.Handled = true;
			}

			if(io.WantCaptureMouse && e.IsInCategory(EventCategoryMouse))
			{
				e.Handled = true;
			}
		}
	}
}