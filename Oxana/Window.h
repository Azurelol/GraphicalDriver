#pragma once

#include "GraphicalDriverInclude.h"

#include "Grid.h"

namespace GraphicalDriver
{
  using LogFunction = std::function<std::string(void)>;
  
  struct Window
  {
    bool enabled;
    std::string title;
    virtual void Draw(unsigned index) = 0;
    Window() : enabled(true) {}
  };

  struct LogWindow : Window
  {
    std::deque<std::string> buffer;
    unsigned bufferSize;
    LogFunction function;

    void Update();
    void Draw(unsigned index) override;
    std::string GetLog(unsigned step);
  };

  struct NotificationWindow : Window
  {
    using ConditionalFunction = std::function<bool(void)>;
    using MessageFunction = std::function<std::string(void)>;

    NotificationWindow() : persistent(false), hasActivated(false) {}
    ConditionalFunction onCheckCondition;
    MessageFunction onNotify;
    bool persistent;
    bool hasActivated;

    void Draw(unsigned index);
    void Update();
  };

  struct ColorMapWindow : Window
  {
    ColorMap map;
    void Update();
    void Draw(unsigned index) override;
  };

}