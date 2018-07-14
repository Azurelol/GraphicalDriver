#pragma once
#include "GraphicalDriverInclude.h"

namespace GraphicalDriver
{
  struct Notification
  {
    bool enabled;
    std::string title;
    std::string message;
  };

}