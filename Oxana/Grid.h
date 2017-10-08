#pragma once

#include "GraphicalDriverInclude.h"

namespace GraphicalDriver
{

  struct ColorMap
  {
    using Cell = char;
    using Row = std::vector<Cell>;
    using Cells = std::vector<Row>;
    using Function = std::function<Cells(void)>;

    Cells cells;
    Function function;
    std::unordered_map<Cell, sf::Color> colors;
  };


}