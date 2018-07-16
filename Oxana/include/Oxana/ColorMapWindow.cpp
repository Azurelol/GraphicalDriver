#include "Window.h"

namespace Oxana
{
	void ColorMapWindow::Update()
	{
		if (buffer.size() == bufferSize)
			buffer.pop_front();

		buffer.push_back(function());
	}

	void ColorMapWindow::Reset()
	{
		buffer.clear();

	}

	void ColorMapWindow::Draw(unsigned index)
	{
		ImGui::SetNextWindowSize(ImVec2(400, 400), ImGuiCond_FirstUseEver);
		ImGui::Begin(title.c_str());

		const ColorMap::ColorGrid& grid = buffer[index];

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, 100.0f);
		{
			ImGui::Text("LEGEND:");
			for (auto& pair : map.colorsVec4)
			{
				int index = pair.first;
				bool hasDescription = map.cellDescriptions.count(index) > 0;
				std::string legend = hasDescription ? map.cellDescriptions[index] : "Cell " + std::to_string(index);
				ImGui::TextColored(pair.second, legend.c_str());
			}

			ImGui::NextColumn();
			ImDrawList* draw_list = ImGui::GetWindowDrawList();
			const int rows = static_cast<int>(grid.rows);
			const int cols = static_cast<int>(grid.columns);
			//const ImVec2 screenSize = ImGui::GetWindowSize();
			//const ImVec2 cellSize(screenSize.x / cols, screenSize.y / rows);
			//static ImVec4 col(1.0f, 1.0f, 0.4f, 1.0f);
			//const ImU32 col32 = ImColor(col);
			const float spacing = 2.0f;
			const float cellSize = map.dynamicSize ? map.cellSize : map.cellSize;
			const ImVec2 p = ImGui::GetCursorScreenPos();

			float y = p.y + 4.0f;
			for (int i = 0; i < rows; ++i)
			{
				float x = p.x + 4.0f;
				for (int j = 0; j < cols; ++j)
				{
					int index = grid[i][j];
					ImVec2 a = ImVec2(x, y);
					ImVec2 b = ImVec2(x + cellSize, y + cellSize);
					draw_list->AddRectFilled(a, b, map.colorsU32[index]);
					draw_list->AddText(a, map.colorsU32[index], "hi");
					//draw_list->AddRectFilled(ImVec2(x, y), ImVec2(x + sz, y + sz), col32);
					x += cellSize + spacing;
					//draw_list->AddRectFilled(ImVec2(x, y), ImVec2(x + cellSize.x, y + cellSize.y), col32);
					//x += cellSize.x + spacing;
				}
				y += cellSize + spacing;
			}

			// Now use a dummy to separate
			//ImGui::Dummy(ImVec2((cellSize + spacing) * cols, (cellSize + spacing * 2) * rows));
			//ImGui::Separator();

		}
		ImGui::Columns(1);
		ImGui::End();
	}

}