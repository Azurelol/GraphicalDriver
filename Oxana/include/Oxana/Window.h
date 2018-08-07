#pragma once

#include "Include.h"

#include "ColorMap.h"
#include "Notification.h"
#include "NodeDisplay.h"
#include "Utilities/Image.h"

namespace Oxana
{
  using LogFunction = std::function<std::string(void)>;
  
  struct Window
  {
    bool enabled;
    std::string title;
    unsigned bufferSize;
    virtual void Draw(unsigned index) = 0;
    virtual void Update() = 0;
		virtual void Reset() = 0;
    Window() : enabled(true) {}
  };

  struct LogWindow : Window
  {
    std::deque<std::string> buffer;    
    LogFunction function;

    void Update();
		void Reset();
    void Draw(unsigned index) override;
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

    void Update();
		void Reset();
    void Draw(unsigned index);
		static void Draw(bool& enabled, const std::string& title, const std::string& message, bool& persistent);
  };

  struct ColorMapWindow : Window
  {
    std::deque<ColorMap::ColorGrid> buffer;
    ColorMap::Function function;
    ColorMap map;

    void Update();
		void Reset();
    void Draw(unsigned index) override;
  };

  struct NodeDisplayWindow : Window
  {
    std::deque<NodeDisplay::Data> buffer;
    std::unique_ptr<NodeDisplay> display;

    void Update();
		void Reset();
    void Draw(unsigned index) override;
  };

	struct ImageDisplayWindow : Window
	{
		Texture image;

		void Update();
		void Reset();
		void Draw(unsigned index) override;
	};

}