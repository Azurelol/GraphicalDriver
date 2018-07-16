#include "Window.h"

//#include <ImGui\>
//#include <ImGui\addons\imguinodegrapheditor\imguinodegrapheditor.h>

#include <ImGui\imgui.h>
#include <ImGui\imgui-SFML.h>

namespace Oxana
{
  void NodeDisplayWindow::Update()
  {
    if (buffer.size() == bufferSize)
      buffer.pop_front();

    display->Record();
    buffer.push_back(display->latest);
  }

	void NodeDisplayWindow::Reset()
	{
		buffer.clear();
	}

  void NodeDisplayWindow::Draw(unsigned index)
  { 
    //ImGui::TestNodeGraphEditor();

    ImGui::SetNextWindowSize(ImVec2(400, 400), ImGuiCond_FirstUseEver);
    ImGui::Begin(title.c_str());
    
    const NodeDisplay::Data& data = buffer[index];

    bool hasParent = !data.parentData.empty();
    if (hasParent)
      ImGui::Columns(2);

    // Describe the node
    ImGui::TextColored(sf::Color::Green, "Current");
    ImGui::Text(data.nodeData.c_str());
    if (hasParent)
    {
      ImGui::NextColumn();
      ImGui::TextColored(sf::Color::Red, "Parent");
      ImGui::Text(data.parentData.c_str());
    }
    ImGui::Separator();

    ImGui::Columns(1);
        
    size_t numChildren = data.childrenData.size();
    if (numChildren > 1)
    {
      ImGui::Columns(numChildren);
      for (auto& child : data.childrenData)
      {
        ImGui::TextColored(sf::Color::Yellow, "Neighbor");
        ImGui::Text(child.c_str());
        ImGui::NextColumn();
      }
    }

    
    
    ImGui::Columns(1);
    ImGui::End();
  }

}