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
    if (buffer.size() == bufferSize)
      buffer.pop_front();

    buffer.push_back(function());
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

    const Grid& grid = buffer[index];

    ImGui::Columns(2);
    ImGui::SetColumnWidth(0, 100.0f);
    {
      ImGui::Text("LEGEND:");
      for (auto& pair : map.colorsVec4)
      {
        int index = pair.first;
        if (index == 0)
          continue;

        // optimize plz
        std::string legend = map.cellDescription + " " + std::to_string(index);
        ImGui::TextColored(pair.second, legend.c_str());
      }

      ImGui::NextColumn();
      ImDrawList* draw_list = ImGui::GetWindowDrawList();
      const int rows = static_cast<int>(grid.rows);
      const int cols = static_cast<int>(grid.columns);
      const ImVec2 screenSize = ImGui::GetWindowSize();
      const ImVec2 cellSize(screenSize.x / cols, screenSize.y / rows);
      //static ImVec4 col(1.0f, 1.0f, 0.4f, 1.0f);
      //const ImU32 col32 = ImColor(col);
      const float spacing = 2.0f;
      const float sz = map.cellSize;
      const ImVec2 p = ImGui::GetCursorScreenPos();

      float y = p.y + 4.0f;
      for (int i = 0; i < rows; ++i)
      {
        float x = p.x + 4.0f;
        for (int j = 0; j < cols; ++j)
        {
          int index = grid[i][j];
          ImVec2 a = ImVec2(x, y);
          ImVec2 b = ImVec2(x + sz, y + sz);
          draw_list->AddRectFilled(a, b, map.colorsU32[index]);
          draw_list->AddText(a, map.colorsU32[index], "hi");
          //draw_list->AddRectFilled(ImVec2(x, y), ImVec2(x + sz, y + sz), col32);
          x += sz + spacing;
          //draw_list->AddRectFilled(ImVec2(x, y), ImVec2(x + cellSize.x, y + cellSize.y), col32);
          //x += cellSize.x + spacing;
        }
        y += sz + spacing;
      }

      // Now use a dummy to separate
      ImGui::Dummy(ImVec2((sz + spacing) * cols, (sz + spacing * 2) * rows));
      ImGui::Separator();

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