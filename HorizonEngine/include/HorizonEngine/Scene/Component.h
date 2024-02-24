#pragma once

#ifndef HZN_COMPONENT_H
#define HZN_COMPONENT_H

#include <cstdint>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <entt/entt.hpp>
#include <cereal/cereal.hpp>
#include <cereal/archives/json.hpp>

#include "HorizonEngine/Camera/Camera.h"
#include "HorizonEngine/Renderer/Sprite.h"

namespace Hzn {
class AssetManager;
class Scene;
class GameObject;

struct RelationComponent {
  size_t m_ChildCount = 0ULL;
  entt::entity m_Parent{entt::null};
  entt::entity m_FirstChild{entt::null};
  entt::entity m_Next{entt::null};
  entt::entity m_Prev{entt::null};

  RelationComponent() = default;
  RelationComponent(const RelationComponent& rhs) = default;
  RelationComponent& operator=(const RelationComponent& rhs) = default;
  ~RelationComponent() = default;

  template <typename Archive>
  void load(Archive& ar) {
    ar(cereal::make_nvp("childCount", m_ChildCount),
       cereal::make_nvp("parent", m_Parent),
       cereal::make_nvp("firstChild", m_FirstChild),
       cereal::make_nvp("nextSibling", m_Next),
       cereal::make_nvp("prevSibling", m_Prev));
  }

  template <typename Archive>
  void save(Archive& ar) const {
    ar(cereal::make_nvp("childCount", (m_ChildCount)),
       cereal::make_nvp("parent", entt::to_integral(m_Parent)),
       cereal::make_nvp("firstChild", entt::to_integral(m_FirstChild)),
       cereal::make_nvp("nextSibling", entt::to_integral(m_Next)),
       cereal::make_nvp("prevSibling", entt::to_integral(m_Prev)));
  }

  bool hasParent() const { return m_Parent != entt::null; }
};

struct NameComponent {
  NameComponent() = default;
  NameComponent(const NameComponent& name) = default;
  NameComponent(const std::string& name) : m_Name(name) {}
  ~NameComponent() = default;

  operator std::string() const& { return m_Name; }
  operator std::string() & { return m_Name; }

  std::string m_Name;

  template <typename Archive>
  void load(Archive& ar) {
    ar(m_Name);
  }

  template <typename Archive>
  void save(Archive& ar) const {
    ar(m_Name);
  }
};

struct TransformComponent {
  TransformComponent() = default;
  TransformComponent(const glm::vec3& position, const glm::vec3& scale)
      : m_Translation(position), m_Scale(scale) {}
  TransformComponent(const glm::vec3& position, const ::glm::vec3& rotation,
                     const glm::vec3& scale)
      : m_Translation(position), m_Scale(scale), m_Rotation(rotation) {}
  TransformComponent(const TransformComponent& rhs)
      : m_Translation(rhs.m_Translation),
        m_Scale(rhs.m_Scale),
        m_Rotation(rhs.m_Rotation) {}
  TransformComponent& operator=(const TransformComponent& rhs) = default;
  ~TransformComponent() = default;

  glm::vec3 m_Translation = glm::vec3(0.0f);
  glm::vec3 m_Scale = glm::vec3(1.0f);
  glm::vec3 m_Rotation = glm::vec3(0.0f);

  glm::mat4 getModelMatrix() const {
    glm::mat4 transform = glm::mat4(1.0f);
    transform = glm::translate(transform, m_Translation);
    transform = glm::rotate(transform, glm::radians(m_Rotation.x), {1, 0, 0});
    transform = glm::rotate(transform, glm::radians(m_Rotation.y), {0, 1, 0});
    transform = glm::rotate(transform, glm::radians(m_Rotation.z), {0, 0, 1});
    transform = glm::scale(transform, m_Scale);
    return transform;
  }

  template <typename Archive>
  void load(Archive& ar) {
    ar(m_Translation.x, m_Translation.y, m_Translation.z);
    ar(m_Rotation.x, m_Rotation.y, m_Rotation.z);
    ar(m_Scale.x, m_Scale.y, m_Scale.z);
  }

  template <typename Archive>
  void save(Archive& ar) const {
    ar(m_Translation.x, m_Translation.y, m_Translation.z);
    ar(m_Rotation.x, m_Rotation.y, m_Rotation.z);
    ar(m_Scale.x, m_Scale.y, m_Scale.z);
  }
};

struct RenderComponent {
  RenderComponent() = default;
  RenderComponent(const glm::vec4& color) : m_Color(color){};
  ~RenderComponent() = default;

  glm::vec4 m_Color = glm::vec4(1.0f);
  std::shared_ptr<Sprite2D> m_Sprite;

  glm::vec2 m_Pos = glm::vec2(0.0f);

  std::string texturePath;
  std::string spritePath;

  template <typename Archive>
  void load(Archive& ar) {
    ar(texturePath, spritePath, m_Pos.x, m_Pos.y);
    ar(m_Color.x, m_Color.y, m_Color.z, m_Color.w);
  }

  template <typename Archive>
  void save(Archive& ar) const {
    ar(texturePath, spritePath, m_Pos.x, m_Pos.y);
    ar(m_Color.x, m_Color.y, m_Color.z, m_Color.w);
  }
};

struct CameraComponent {
  SceneCamera2D m_Camera;
  bool m_Primary = true;

  CameraComponent(const float aspectRatio = 1.0f, const float zoom = 1.0f)
      : m_Camera(aspectRatio, zoom) {}

  ~CameraComponent() = default;

  template <typename Archive>
  void load(Archive& ar) {
    ar(m_Camera, m_Primary);
  }

  template <typename Archive>
  void save(Archive& ar) const {
    ar(m_Camera, m_Primary);
  }
};

struct RigidBody2DComponent {
  enum class BodyType { Static = 0, Kinematic, Dynamic };

  BodyType m_Type = BodyType::Dynamic;

  bool m_FixedRotation = false;
  // runtime body opaque-pointer (will be stored at different location).
  void* m_RuntimeBody = nullptr;

  RigidBody2DComponent() = default;
  RigidBody2DComponent(const RigidBody2DComponent& body) = default;
  ~RigidBody2DComponent() = default;

  template <typename Archive>
  void load(Archive& ar) {
    ar((BodyType)m_Type, m_FixedRotation);
  }

  template <typename Archive>
  void save(Archive& ar) const {
    ar((int)m_Type, m_FixedRotation);
  }
};

struct BoxCollider2DComponent {
  glm::vec2 offset = {0.0f, 0.0f};
  glm::vec2 size = {0.5f, 0.5f};

  float m_Density = 1.0f;
  float m_Friction = 0.5f;
  float m_Restitution = 0.0f;
  float m_RestitutionThreshold = 0.5f;
  bool m_IsSensor = false;

  // runtime fixture opaque-pointer (will be stored at different location).
  void* m_RuntimeFixture = nullptr;

  template <typename Archive>
  void load(Archive& ar) {
    ar(offset.x, offset.y, size.x, size.y, m_Density, m_Friction, m_Restitution,
       m_RestitutionThreshold, m_IsSensor);
  }

  template <typename Archive>
  void save(Archive& ar) const {
    ar(offset.x, offset.y, size.x, size.y, m_Density, m_Friction, m_Restitution,
       m_RestitutionThreshold, m_IsSensor);
  }
};

struct ScriptComponent {
  ScriptComponent() = default;
  ScriptComponent(const std::string& scriptName) : m_ScriptName(scriptName) {}
  ~ScriptComponent() = default;
  std::string m_ScriptName;

  template <typename Archive>
  void load(Archive& ar) {
    ar(cereal::make_nvp("ScriptName", m_ScriptName));
  }

  template <typename Archive>
  void save(Archive& ar) const {
    ar(cereal::make_nvp("ScriptName", m_ScriptName));
  }
};

template <typename... Component>
struct ComponentGroup {};

using AllComponents =
    ComponentGroup<NameComponent, RelationComponent, TransformComponent,
                   RigidBody2DComponent, BoxCollider2DComponent,
                   RenderComponent, CameraComponent, ScriptComponent>;
using SelectableComponents =
    ComponentGroup<RenderComponent, RigidBody2DComponent,
                   BoxCollider2DComponent, CameraComponent, ScriptComponent>;

void addComponent(GameObject& obj, std::string& componentName);
bool hasComponent(const GameObject& obj, std::string& componentName);
}  // namespace Hzn

#endif