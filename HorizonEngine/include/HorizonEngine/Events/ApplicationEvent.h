#pragma once

#ifndef HZN_APPLICATION_EVENT_H
#define HZN_APPLICATION_EVENT_H

#include <string>
#include <sstream>

#include "HorizonEngine/Core/Core.h"
#include "HorizonEngine/Events/Event.h"

namespace Hzn {
/// <summary>
/// WindowResizeEventclass inherits the event class
/// </summary>
class WindowResizeEvent : public Event {
 public:
  /// <summary>
  /// WindowResizeEvent constructor take in and initialises width & height
  /// </summary>
  /// <param name="width"></param>
  /// <param name="height"></param>
  WindowResizeEvent(unsigned int width, unsigned int height)
      : m_Width(width), m_Height(height) {}

  /// <summary>
  /// width & height getters
  /// </summary>
  /// <returns></returns>
  unsigned int GetWidth() const { return m_Width; }
  unsigned int GetHeight() const { return m_Height; }

  //! Returns window resizing as a string
  std::string ToString() const override {
    std::stringstream ss;
    ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
    return ss.str();
  }

  /// <summary>
  /// Call event macros from Event class and pass in the
  /// windowresizing event to be handled
  /// </summary>
  EVENT_CLASS_TYPE(WindowResize)
  EVENT_CLASS_CATEGORY(EventCategoryApplication)

 private:
  unsigned int m_Width, m_Height;
};

//! Close Event
class WindowCloseEvent : public Event {
 public:
  WindowCloseEvent() = default;

  EVENT_CLASS_TYPE(WindowClose)
  EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

class AppTickEvent : public Event {
 public:
  AppTickEvent() = default;

  EVENT_CLASS_TYPE(AppTick)
  EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

//! Handles update events
class AppUpdateEvent : public Event {
 public:
  AppUpdateEvent() = default;

  EVENT_CLASS_TYPE(AppUpdate)
  EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

//! Handles
class AppRenderEvent : public Event {
 public:
  AppRenderEvent() = default;

  EVENT_CLASS_TYPE(AppRender)
  EVENT_CLASS_CATEGORY(EventCategoryApplication)
};
}  // namespace Hzn
#endif  // !HZN_APPLICATION_EVENT_H
