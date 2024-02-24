#include "pch.h"

#include <FileWatch.hpp>

#include "HorizonEngine/Core/App.h"
#include "HorizonEngine/Scene/SceneManager.h"
#include "HorizonEngine/AssetManagement/AssetManager.h"
#include "HorizonEngine/Scripting/ScriptEngine.h"

#include "HorizonEngine/Project/ProjectManager.h"

namespace Hzn {
static std::unique_ptr<filewatch::FileWatch<std::string>> watch;
std::shared_ptr<Project> ProjectManager::s_Project = nullptr;

/// <summary>
/// Creates new, or opens a project depending on the path provided.
/// </summary>
/// <param name="name"></param>
/// <param name="directoryPath"></param>
/// <returns></returns>
std::shared_ptr<Project> ProjectManager::create(
    const std::string& name, const std::filesystem::path& directoryPath) {
  if (s_Project) {
    close();
  }
  s_Project = std::make_shared<Project>(name, directoryPath);

  if (std::filesystem::exists(s_Project->m_Path.parent_path().string() +
                              "\\bin")) {
    watch.reset(new filewatch::FileWatch<std::string>(
        s_Project->m_Path.parent_path().string() + "\\bin",
        [&](const std::string& watchPath, const filewatch::Event eventType) {
          /*HZN_CORE_INFO("{}", std::filesystem::absolute(watchPath).string());
          HZN_CORE_WARN("{}", s_Project->m_Path.parent_path().string() +
          "\\bin\\ScriptAppLib.dll");*/

          if (watchPath == "ScriptAppLib.dll") {
            HZN_CORE_DEBUG("Changes detected in ScriptAppLib.dll");
            if (!ScriptEngine::isReloadPending() &&
                (eventType == filewatch::Event::modified ||
                 eventType == filewatch::Event::added)) {
              ScriptEngine::startReload();
              Hzn::App::getApp().submitMainThreadQueue(
                  [&]() { ScriptEngine::ReloadAssembly(); });
            }
          }
        }));
  }
  //! here we will be starting the file watcher on this location if the path
  //! exists.
  if (std::filesystem::exists(s_Project->m_Path.parent_path().string() +
                              "\\load_target\\ScriptAppLib.dll")) {
    ScriptEngine::ReloadAssembly();
  }

  return s_Project;
}

//! If there isn't one, try create a scene in an empty project
bool ProjectManager::newScene(const std::string& name) {
  if (!s_Project) {
    HZN_CORE_ASSERT(false, "trying to create new Scene in empty project!");
    return false;
  }
  std::filesystem::path projectDir = s_Project->m_Path.parent_path();
  std::filesystem::path sceneDir = projectDir.string() + "\\scenes";

  //! create a scenes directory if it doesn't exist.
  if (!std::filesystem::exists(sceneDir)) {
    std::filesystem::create_directory(sceneDir);
  }

  //! create a scene file.
  std::filesystem::path sceneFilePath =
      sceneDir.string() + "\\" + name + ".scene";
  s_Project->m_Scene = SceneManager::create(sceneFilePath);

  return s_Project->m_Scene != nullptr;
}

//! If there isn't a scene open one
bool ProjectManager::openScene(const std::filesystem::path& sceneFilePath) {
  if (!s_Project) {
    HZN_CORE_ASSERT(false, "trying to open Scene in an empty project!");
    return false;
  }
  std::filesystem::path projectDir = s_Project->m_Path.parent_path();
  std::filesystem::path sceneDir = projectDir.string() + "\\scenes";

  HZN_CORE_ASSERT(std::filesystem::exists(sceneDir), "no scenes directory!");

  s_Project->m_Scene = SceneManager::open(sceneFilePath);
  return s_Project->m_Scene != nullptr;
}

//! Close a scene
bool ProjectManager::closeScene() {
  if (s_Project && s_Project->m_Scene) {
    return SceneManager::close();
  }
  return false;
}

//! save a scene
bool ProjectManager::saveScene() {
  if (s_Project && s_Project->m_Scene) {
    return SceneManager::save();
  }
  return false;
}

/// <summary>
/// Open a project in the provided file path
/// This applies to scripting functionality as well
/// </summary>
/// <param name="projectFilePath"></param>
/// <returns></returns>
std::shared_ptr<Project> ProjectManager::open(
    const std::filesystem::path& projectFilePath) {
  if (!projectFilePath.empty()) {
    if (s_Project) {
      // AssetManager::destroy();
      close();
    }
    try {
      s_Project = std::make_shared<Project>(projectFilePath);

      if (std::filesystem::exists(s_Project->m_Path.parent_path().string() +
                                  "\\bin")) {
        watch.reset(new filewatch::FileWatch<std::string>(
            s_Project->m_Path.parent_path().string() + "\\bin",
            [&](const std::string& watchPath,
                const filewatch::Event eventType) {
              /*HZN_CORE_INFO("{}",
              std::filesystem::absolute(watchPath).string());
              HZN_CORE_WARN("{}", s_Project->m_Path.parent_path().string() +
              "\\bin\\ScriptAppLib.dll");*/

              if (watchPath == "ScriptAppLib.dll") {
                HZN_CORE_DEBUG("Changes detected in ScriptAppLib.dll");
                if (!ScriptEngine::isReloadPending() &&
                    (eventType == filewatch::Event::modified ||
                     eventType == filewatch::Event::added)) {
                  ScriptEngine::startReload();
                  Hzn::App::getApp().submitMainThreadQueue(
                      [&]() { ScriptEngine::ReloadAssembly(); });
                }
              }
            }));
      }
      // here we will be starting the file watcher on this location if the path
      // exists.
      if (std::filesystem::exists(s_Project->m_Path.parent_path().string() +
                                  "\\load_target\\ScriptAppLib.dll")) {
        ScriptEngine::ReloadAssembly();
      }
    } catch (std::exception& e) {
      HZN_CORE_ERROR(e.what());
      return nullptr;
    }
  }

  return s_Project;
}

//! Save project
bool ProjectManager::save() {
  bool result = true;
  if (s_Project && s_Project->m_Scene) {
    std::filesystem::path projectFile = s_Project->m_Path;
    std::ofstream os(projectFile, std::ios::binary);
    os << "ActiveScene"
       << " "
       << ":"
       << " " << s_Project->m_Scene->m_Path;
    os.close();
    result = SceneManager::save();
  }
  return result;
}

//! close a project
bool ProjectManager::close() {
  bool result = save();
  if (s_Project && s_Project->m_Scene) {
    result = SceneManager::close();
    AssetManager::destroy();
    // here we should close the previous file watcher if it exists.
    watch.reset();
    s_Project.reset();
  }
  return result;
}

std::vector<std::filesystem::path> ProjectManager::getAllScenes() {
  std::vector<std::filesystem::path> sceneNames;

  if (s_Project) {
    for (auto& entry : std::filesystem::directory_iterator(
             s_Project->getPath().parent_path().string() + "\\scenes")) {
      sceneNames.push_back(entry.path());
    }
  }

  return sceneNames;
}
}  // namespace Hzn