#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <initializer_list>
#include <algorithm>
#include <random>
#include <iostream>

namespace Oxana
{
	static std::random_device randomDevice;
	static std::mt19937 randomNumberGenerator(randomDevice());

	// Represents a grid in 2D spce
	template <class T>
	struct Grid
	{
		// A position within the grid
		struct Position
		{
			int x;
			int y;
			Position() : x(0), y(0) {}
			Position(int x, int y) : x(x), y(y) {}
		};

		// A cell within the grid
		struct Cell
		{
			T value;
			int row;
			int col;

			Cell() : row(0), col(0) {}
			Cell(const T& value, int r, int c) : value(value), row(r), col(c) {}

			//void SetValue(const T& value) { (*value) = value; }
			//T GetValue() const { return *value; }
			//bool Valid() const { return value != nullptr; }

			friend std::ostream& operator<<(std::ostream& os, const Cell& cellRef)
			{
				//if (reference != nullptr)
				//	os << cellRef.GetValue();
				os << "(" << cellRef.row << "," << cellRef.col << ")";
				return os;
			}
		};

		using CellReference = Cell*;

		//--------------------------------------------------------------------------/
		// Fields
		//--------------------------------------------------------------------------/
		std::vector<std::vector<Cell>> cells;
		int size;
		size_t hash;
		T defaultValue;
		std::string stringRepresentation;

		//--------------------------------------------------------------------------/
		// CTOR
		//-------		-------------------------------------------------------------------/
		template <typename T>
		Grid(int size, T defaultValue) : size(size), defaultValue(defaultValue), cells(size, std::vector<Cell>(size))
		{
			this->Initialize();
		}
		operator std::vector<std::vector<T>>() const { return cells; }

		//--------------------------------------------------------------------------/
		// Methods
		//--------------------------------------------------------------------------/
		void Initialize()
		{
			for (int r = 0; r < size; ++r)
			{
				for (int c = 0; c < size; ++c)
				{
					Cell& cell = this->cells[r][c];
					cell.row = r;
					cell.col = c;
					cell.value = this->defaultValue;
				}
			}
		}
		
		// Records the current state of this grid, providing a hash and a string representation
		void Record()
		{
			static std::hash<std::string> hasher;

			std::stringstream builder;

			for (auto& row : cells)
			{
				for (auto& col : row)
				{
					builder << col << ",";
				}
				builder << "\n";
			}

			this->stringRepresentation = builder.str();
			this->hash = hasher(this->stringRepresentation);
		}

		template <typename T>
		void FillRandomly(std::initializer_list<T> list)
		{
			std::vector<T> values(list);
			FillRandomly(values);
		}

		template <typename T>
		void FillRandomly(std::vector<T> values)
		{
			std::uniform_int_distribution<int> randomRange(0, static_cast<int>(values.size()) - 1);

			for (auto& row : cells)
			{
				for (auto& cell : row)
				{
					int randomIndex = randomRange(randomNumberGenerator);
					cell.value = values[randomIndex];
				}
			}
		}

		CellReference GetRandomCellReference()
		{
			std::uniform_int_distribution<int> randomRange(0, (size)-1);
			int rowIndex = randomRange(randomNumberGenerator);
			int colIndex = randomRange(randomNumberGenerator);
			return &this->cells[rowIndex][colIndex];
		}

		//CellReference GetRandomCellReference(std::function<bool(Cell) validateFunction)
		//{
		//	bool valid = false;
		//	CellReference cell = nullptr;
		//	while (!valid)
		//	{
		//		cell = GetRandomCellReference();
		//		valid = 
		//	}
		//
		//	std::uniform_int_distribution<int> randomRange(0, (size)-1);
		//	int rowIndex = randomRange(randomNumberGenerator);
		//	int colIndex = randomRange(randomNumberGenerator);
		//	return &this->cells[rowIndex][colIndex];
		//}

		template <typename T>
		void SetRandomCell(T value)
		{
			std::uniform_int_distribution<int> randomRange(0, (size)-1);
			int rowIndex = randomRange(randomNumberGenerator);
			int colIndex = randomRange(randomNumberGenerator);
			this->cells[rowIndex][colIndex].value = value;
		}

		std::vector<CellReference> GetNeigborReferences(int row, int col, bool includeDiagonal = false)
		{
			std::vector<Cell*> neighbors;
			bool isAtRightEdge = row == (size - 1);
			bool isAtLeftEdge = row == 0;
			bool isAtTopEdge = col == (size - 1);
			bool isAtBottomEdge = col == 0;

			if (!isAtLeftEdge)
				neighbors.push_back(GetReference(row - 1, col));
			if (!isAtRightEdge)
				neighbors.push_back(GetReference(row + 1, col));
			if (!isAtTopEdge)
				neighbors.push_back(GetReference(row, col + 1));
			if (!isAtBottomEdge)
				neighbors.push_back(GetReference(row, col - 1));

			if (includeDiagonal)
			{
				if (!isAtTopEdge)
				{
				// Top left
					if (!isAtLeftEdge)
						neighbors.push_back(GetReference(row - 1, col + 1));
					// Top right
					if (!isAtRightEdge)
						neighbors.push_back(GetReference(row + 1, col + 1));
				}
				if (!isAtBottomEdge)
				{
					// Bottom left
					if (!isAtLeftEdge)
						neighbors.push_back(GetReference(row - 1, col - 1));
					// Bottom right
					if (!isAtRightEdge)
						neighbors.push_back(GetReference(row + 1, col - 1));
				}

			}
			
			return neighbors;
		}

		std::string ToString()
		{
			return stringRepresentation;
		}

		private:
		CellReference GetReference(int row, int col)
		{
			return &(this->cells[row][col]);
		}

	};
}