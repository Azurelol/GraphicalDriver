#include "Window.h"

namespace GraphicalDriver
{
  void LogWindow::Update()
  {
    if (buffer.size() == bufferSize)
      buffer.pop_front();

    buffer.push_back(function());
  }

  void ColorMapWindow::Update()
  {
    map.cells = map.function();
  }

  void NotificationWindow::Update()
  {
    bool canNotify = onCheckCondition();
    if (canNotify && !hasActivated)
    {
      //hasActivated = true;
      enabled = true;
      //ImGui::OpenPopup(title.c_str());
    }
  }

  std::string LogWindow::GetLog(unsigned step)
  {
    return buffer[step % bufferSize];
  }

  void LogWindow::Draw(unsigned index)
  {
    ImGui::SetNextWindowSize(ImVec2(400, 400), ImGuiCond_FirstUseEver);
    ImGui::Begin(title.c_str());
    if (!buffer.empty())
      ImGui::TextUnformatted(buffer[index].c_str());
    ImGui::End();
  }


  void ColorMapWindow::Draw(unsigned index)
  {
    ImGui::SetNextWindowSize(ImVec2(400, 400), ImGuiCond_FirstUseEver);
    ImGui::Begin(title.c_str());

    //static float arr[] = {0.6f, 0.1f, 1.0f, 0.5f, 0.92f, 0.1f, 0.2f};
    //ImGui::PlotLines("Curve", arr, IM_ARRAYSIZE(arr));

    //ImVec2 offset(0, 0);
    //ImVec2 winPos = ImGui::GetCursorScreenPos();
    //ImU32 gridColor = ImColor(200, 200, 200, 40);
    //const sf::Color gridColor = sf::Color(200, 200, 200, 40);
    //{
    //  //const float gridSize = 64.0f;
    //  //ImVec2 canvasSize = ImGui::GetWindowSize();
    //  //for (float x = fmodf(offset.x, gridSize); x < canvasSize.x; x += gridSize)
    //  //  drawList->AddLine(ImVec2(x + winPos.x, 0.0f), ImVec2(x + winPos.x, canvasSize.y + winPos.y), gridColor);
    //  //for (float y = fmodf(offset.y, gridSize); y < canvasSize.y; y += gridSize)
    //  //  drawList->AddLine(ImVec2(0.0f, winPos.y + y), ImVec2(canvasSize.x + winPos.x, winPos.y + y), gridColor);
    //}

    //auto displaySize = ImGui::GetIO().DisplaySize;
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    auto winSize = ImGui::GetWindowSize();
    const ImVec2 size(50, 50);
    const auto rows = map.cells.size();
    const auto columns = map.cells[0].size();

    ImGui::Columns(columns);
    for (int row = 0; row < rows; row++)
    {
      ImGui::Separator();
      for (int col = 0; col < columns; col++)
      {
        const int i = row + col;
        ImVec2 pos(col, row);
        char c = map.cells[row][col];
        ImGui::Text(std::string(1, c).c_str());
        ImGui::NextColumn();
      }
    }

    ImGui::Columns(1);
    ImGui::End();
  }



  void NotificationWindow::Draw(unsigned index)
  {
    if (enabled)
      ImGui::OpenPopup(title.c_str());

    if (ImGui::BeginPopupModal(title.c_str(), &enabled, ImGuiWindowFlags_AlwaysAutoResize))
    {      
      std::string msg = onNotify();
      ImGui::Text(msg.c_str());

      ImGui::Separator();
      static bool dont_ask_me_next_time = false;
      ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
      ImGui::Checkbox("Don't ask me next time", &dont_ask_me_next_time);
      ImGui::PopStyleVar();

      if (ImGui::Button("OK", ImVec2(120, 0))) 
      { 
        ImGui::CloseCurrentPopup(); 
        enabled = false;
      }
      //ImGui::SameLine();
      //
      //if (ImGui::Button("Cancel", ImVec2(120, 0))) 
      //{ 
      //  ImGui::CloseCurrentPopup(); 
      //}
      ImGui::EndPopup();
    }
  }


}