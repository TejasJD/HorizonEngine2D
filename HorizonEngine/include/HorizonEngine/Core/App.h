#pragma once

#include <cstdint>
#include <memory>
#include <mutex>
#include <functional>
#include <vector>
#include <string>
#include <filesystem>

#include "HorizonEngine/Core/Window.h"
#include "HorizonEngine/Events/Event.h"
#include "HorizonEngine/Events/ApplicationEvent.h"
#include "HorizonEngine/Core/Layer.h"
#include "HorizonEngine/Core/LayerStack.h"
#include "HorizonEngine/ImGui/ImGuiLayer.h"
#include "HorizonEngine/Core/TimeStep.h"

namespace Hzn {
//! App class
class App {
 public:
  //! Constructor
  App();
  //! Destructor
  virtual ~App();

  //! Method to add new layer e.g. from main.cpp
  void addLayer(Layer* layer) {
    //! Adds the new layer to m_layers stack
    m_Layers.addLayer(layer);
    //! attachs the new layer
    layer->onAttach();
  }

  //! Method to add new layer to overlay
  void addOverlay(Layer* layer) {
    //! add the layer to the overlay stack
    m_Layers.addOverlay(layer);
    //! attachs the new layer to overlay stack
    layer->onAttach();
  }

  void run();
  void onEvent(Event& e);
  void close() { m_Running = false; }

  //! retrieves the imGuiLayer
  Layer* getImguiLayer() { return m_ImguiLayer; }

  //! Close the window
  bool onWindowClose(WindowCloseEvent& e);
  //! Resize window
  bool onWindowResize(WindowResizeEvent& e);

  //! returns the instance of the App
  static App& getApp() { return *m_Instance; }
  //! Returns the window
  Window& getAppWindow() const { return *m_Window; }

  //! It executes the main thread
  void executeMainThreadQueue();

  void submitMainThreadQueue(const std::function<void()>& fn);

  //! Returns the executable path
  std::filesystem::path getExecutablePath() const { return m_ExecutablePath; }

 protected:
  static App* m_Instance;

 private:
  bool m_Running = true;
  bool m_Minimized = false;

  std::shared_ptr<Window> m_Window;
  std::vector<std::function<void()>> m_MainThreadQueue;
  std::mutex m_MainThreadQueueLock;
  std::filesystem::path m_ExecutablePath;

  ImguiLayer* m_ImguiLayer;
  LayerStack m_Layers;
  TimeStep m_TimeStep;
  float lastFrameTime = 0.0f;
};

// to be defined by the application that implements this function
std::shared_ptr<App> createApp();
}  // namespace Hzn
