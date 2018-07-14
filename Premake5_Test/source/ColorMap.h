#pragma once

#include "GraphicalDriverInclude.h"

namespace GraphicalDriver
{
  // Represents a 2D grid
  struct Grid : std::vector<std::vector<int>>
  {
    using Row = std::vector<int>;
    size_t rows;
    size_t columns;
  };

  // Represents how a map should be drawn
  struct ColorMap
  {   
    std::unordered_map<int, sf::Color> colorsVec4;
    std::unordered_map<int, ImU32> colorsU32;

    // The function to be invoked in order to get the latest grid    
    //std::vector<Function> functions;    

    using Function = std::function<Grid(void)>;
    float cellSize;
    std::string cellDescription;

    ColorMap() : cellSize(48.0f)
    {
    }

    //void AddFunction(const Function& gridFunc)
    //{
    //  functions.push_back(gridFunc);
    //}

    void AddColor(int index, sf::Color color)
    {
      colorsVec4[index] = color;
      colorsU32[index] = ImColor(color);
    }

    template <typename T>
    static Grid FillGrid(const std::vector<std::vector<T>>& container)
    {      
      Grid grid;
      grid.rows = container.size();
      grid.columns = container[0].size();
      //size_t rows, columns;
      //rows = columns = container.size();
      //map.rows = map.columns = container.size();

      for (size_t i = 0; i < grid.rows; ++i)
      {
        grid.push_back(Grid::Row());
        for (size_t j = 0; j < grid.columns; ++j)
        {
          grid[i].push_back(container[i][j]);
        }
      }

      return grid;
    }
  };


}