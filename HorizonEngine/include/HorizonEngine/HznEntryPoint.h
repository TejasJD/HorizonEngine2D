#pragma once

#ifndef HZN_ENTRY_POINT_H
#define HZN_ENTRY_POINT_H

extern std::shared_ptr<Hzn::App> Hzn::createApp();

int main() {
  Hzn::Logging::Init();
  auto app = Hzn::createApp();
  app->run();
  return 0;
}

#endif  // !HZN_ENTRY_POINT_H
