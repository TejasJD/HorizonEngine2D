#include "pch.h"

#include "HorizonEngine/Events/MouseEvent.h"
#include "HorizonEngine/Layer.h"
#include "HorizonEngine/App.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <ImGuizmo.h>
#include <imnodes.h>

#include <IconsFontAwesome5.h>
#include "ImguiLayer.h"

namespace Hzn
{
	//! Constructor - Log Imgui layer attached
	ImguiLayer::ImguiLayer() : Layer("ImGui Layer")
	{
		HZN_CORE_INFO("Imgui Layer Attached!");
	}

	//!Destructor
	ImguiLayer::~ImguiLayer()
	{
		onDetach();
	}

	/// <summary>
	/// Method to call when you want to  attach ImGui layer to render on screen
	/// </summary>
	void ImguiLayer::onAttach()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImNodes::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		(void)io;

		// turning on certain viewport flags
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();

		// Imnodes style color
		ImNodes::StyleColorsDark();

		auto& colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

		// Headers
		colors[ImGuiCol_Header] = ImVec4{ 0.1f, 0.105f, 0.1f, 1.0f };
		colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_HeaderActive] = ImVec4{ 0.1f, 0.105f, 0.1f, 1.0f };

		// Buttons
		colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		// Frame Bg
		colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.105f, 0.151f, 1.0f };

		// Tabs
		colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.105f, 0.151f, 1.0f };
		colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
		colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
		colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

		// Title 
		colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		// MenuBar
		colors[ImGuiCol_MenuBarBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };
		
		// imnodes style confi
		auto& nodeStyle = ImNodes::GetStyle();
		nodeStyle.PinOffset = 8.0f;
		nodeStyle.Colors[ImNodesCol_Pin] = IM_COL32(227, 255, 20, 255);
		nodeStyle.Colors[ImNodesCol_PinHovered] = IM_COL32_WHITE;
		nodeStyle.Colors[ImNodesCol_Link] = IM_COL32(227, 255, 20, 255);
		nodeStyle.Colors[ImNodesCol_LinkHovered] = IM_COL32_WHITE;
		nodeStyle.Colors[ImNodesCol_LinkSelected] = IM_COL32_WHITE;
		nodeStyle.Colors[ImNodesCol_TitleBar] = IM_COL32(30, 30, 30, 255);
		nodeStyle.Colors[ImNodesCol_TitleBarHovered] = IM_COL32(50, 50, 50, 255);
		nodeStyle.Colors[ImNodesCol_TitleBarSelected] = IM_COL32(50, 50, 50, 255);



		// a workaround, so that window rounding is consistent.
		ImGuiStyle& style = ImGui::GetStyle();

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}


		// ** Updated Fonts For Game Engine Editor ** 
		// Fonts stored with Index values from 0..n 

		// Regular Styled Font [0]
		io.Fonts->AddFontFromFileTTF("assets/fonts/opensans/OpenSans-Bold.ttf", 18.0f);
		ImFontConfig config;
		config.MergeMode = true;
		//config.GlyphMaxAdvanceX = -2.0f;
		//config.PixelSnapH = true;
		config.PixelSnapH = true;
		config.GlyphMinAdvanceX = 13.0f; // Use if you want to make the icon monospaced
		static const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
		auto val = io.Fonts->AddFontFromFileTTF("assets/fonts/fa-solid-900.ttf", 13.0f, &config, icon_ranges);
		io.FontDefault = io.Fonts->Fonts[0];

		auto window = (GLFWwindow*)App::getApp().getAppWindow().getPlatformRawWindow();
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 420");
	}

	//! onUpdate ImGui
	void ImguiLayer::onUpdate(TimeStep ts) {}

	//! Detach ImGui
	void ImguiLayer::onDetach()
	{
		ImGui_ImplGlfw_Shutdown();
		ImGui_ImplOpenGL3_Shutdown();

		ImNodes::DestroyContext();
		ImGui::DestroyContext();
	}

	/// <summary>
	/// Handle roducing Imgui contents when imGui begins
	/// ie. what imgui contents goes into the window.
	/// </summary>
	void ImguiLayer::imguiBegin()
	{
		ImGui_ImplGlfw_NewFrame();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();
	}

	/// <summary>
	/// Handle ImGUI contents when ended.
	/// </summary>
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

	/// <summary>
	/// Handle ImGui Keybord and mouse events
	/// </summary>
	/// <param name="e"></param>
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