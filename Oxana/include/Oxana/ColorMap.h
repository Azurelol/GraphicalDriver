#pragma once

//#include "GraphicalDriverInclude.h"

namespace Oxana
{
  // Represents how a map should be drawn
  struct ColorMap
  {   
		friend class GUI;

  // Represents a 2D grid of colors using integers to represent individual colors
		struct ColorGrid : std::vector<std::vector<int>>
		{
			using Row = std::vector<int>;
			size_t rows;
			size_t columns;
		};

    std::unordered_map<int, sf::Color> colorsVec4;
    std::unordered_map<int, ImU32> colorsU32;
		std::unordered_map<int, std::string> cellDescriptions;
    using Function = std::function<ColorGrid(void)>;
    float cellSize;
		bool dynamicSize;

    ColorMap() : cellSize(48.0f), dynamicSize(true)
    {
			// Adjust size dynamically based on current window size?
    }

		ColorMap(float cellSize) : cellSize(cellSize), dynamicSize(false)
		{
		}


		void Describe(int index, sf::Color color, const std::string& description)
		{
			cellDescriptions[index] = description;
			Describe(index, color);
		}

		void Describe(int index, sf::Color color)
		{
			colorsVec4[index] = color;
			colorsU32[index] = ImColor(color);
		}


		template <typename T>
		static ColorGrid FillGrid(const std::vector<std::vector<T>>& container)
		{
			ColorGrid grid;
			grid.rows = container.size();
			grid.columns = container[0].size();

			for (size_t i = 0; i < grid.rows; ++i)
			{
				grid.push_back(ColorGrid::Row());
				for (size_t j = 0; j < grid.columns; ++j)
				{
					grid[i].push_back(container[i][j]);
				}
			}

			return grid;
		}

    template <typename T>
    static ColorGrid FillGrid(const std::vector<std::vector<T>>& container, std::function<int(const T&)> valueFunction)
    {      
      ColorGrid grid;
      grid.rows = container.size();
      grid.columns = container[0].size();

      for (size_t i = 0; i < grid.rows; ++i)
      {
        grid.push_back(ColorGrid::Row());
        for (size_t j = 0; j < grid.columns; ++j)
        {
					int value = valueFunction(container[i][j]);
          grid[i].push_back(value);
        }
      }

      return grid;
    }
  };


}