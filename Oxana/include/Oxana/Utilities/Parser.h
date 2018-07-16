#pragma once

#include <utility>
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <functional>
#include <regex>
#include <numeric>

namespace Oxana
{
	class CSVParse
	{
		private:
		std::string fileName;
		std::fstream file;

		public:
		std::unordered_map<std::string, int> columnIndeces;
		std::vector<std::vector<std::string>> rows;
		std::vector<std::vector<std::string>> columns;
		std::vector<std::string> columnNames;
		int numberOfRows;
		int numberOfColumns;
		std::string separator;
		bool strict;

		CSVParse(const std::string& fileName) : fileName(fileName), separator(","), numberOfRows(0), numberOfColumns(0), strict(true)
		{
		}

		CSVParse(const std::string& fileName, char separator) : fileName(fileName), separator(std::to_string(separator)), numberOfRows(0), numberOfColumns(0), strict(true)
		{
		}

		bool Parse(bool hasColumnHeaders)
		{
			// Open the file
			this->file.open(this->fileName);
			if (!this->file.good())
				return false;

			std::string line;

			// Get the first line
			std::getline(file, line);
			std::vector<std::string> firstRow = Split(line, separator);
			numberOfColumns = static_cast<int>(firstRow.size());
			columns.resize(numberOfColumns);

			// Used to fill columns for each row
			static auto fillColumns = [&](const std::vector<std::string>& row)
			{
				for (int c = 0; c < numberOfColumns; ++c)
				{
					columns[c].push_back(row[c]);
				}
			};

			// If the first row is the column headers, set it
			if (hasColumnHeaders)
			{
				columnNames = firstRow;
				for (int i = 0; i < columnNames.size(); ++i)
				{
					columnIndeces.insert(std::make_pair(columnNames[i], i));
				}
			}
			else
			{
				rows.push_back(firstRow);
				fillColumns(firstRow);
				numberOfRows++;
			}

			// Now parse the rest
			numberOfRows = numberOfColumns = 0;
			while (std::getline(file, line))
			{
				std::vector<std::string> row = Split(line, ",");
				numberOfColumns = static_cast<int>(row.size());

				// Add to rows
				rows.push_back(row);

				// Add to each column
				fillColumns(row);

				numberOfRows++;
			}

			this->file.close();
			return true;
		}

		std::vector<float> GetRowAsFloats(int index)
		{
			return GetAsFloats(rows[index]);
		}

		std::vector<float> GetColumnAsFloats(int index)
		{
			return GetAsFloats(columns[index]);
		}

		std::vector<float> GetColumnAsFloats(std::string columnName)
		{
			int index = columnIndeces[columnName];
			return GetAsFloats(columns[index]);
		}

		std::vector<int> GetRowAsInts(int index)
		{
			return GetAsInts(rows[index]);
		}

		std::vector<int> GetColumnAsInts(int index)
		{
			return GetAsInts(columns[index]);
		}

		std::vector<int> GetColumnAsInts(std::string name)
		{
			int index = columnIndeces[name];
			return GetAsInts(columns[index]);
		}

		std::vector<std::string> GetColumn(std::string name)
		{
			int index = columnIndeces[name];
			return columns[index];
		}

		std::vector<std::string> GetColumn(int index)
		{
			return columns[index];
		}





		//template <typename T>
		//std::vector<T> Convert(const std::vector<std::string>& elements, std::function<T<const std::string&>> function)
		//{
		//	std::vector<T> values;
		//	for (int c = 0; c < elements.size(); ++c)
		//	{
		//		values.push_back(function(elements[c]));
		//	}
		//	return values;
		//}

		// Splits a string into a vector strings given a delimiter
		static std::vector<std::string> Split(const std::string & data, std::string delimiter)
		{
			std::vector<std::string> words;
			std::size_t prevPos = 0, pos;
			while ((pos = data.find_first_of(delimiter.c_str(), prevPos)) != std::string::npos)
			{
				if (pos > prevPos)
					words.push_back(data.substr(prevPos, pos - prevPos));
				prevPos = pos + 1;
			}

			if (prevPos < data.length())
				words.push_back(data.substr(prevPos, std::string::npos));

			return words;
		}

		static std::string StripToNumeric(const std::string& input)
		{
			// ^ start of a string
			// 	\d{ 1,2 } followed by one or two digits
			// 	(start of capture group
			// 		\.\d{ 1,2 } followed by a dot and one or two digits
			// 	) end of capture group
			// 	+ indicates the previous capture group be repeated 1 or more times
			// 	$ end of string
						 
			// R"([^A-Za-z0-9])" or R"([^A-Za-z\d])" = select non - alphabetic and non - numeric
			// 	R"([A-Za-z0-9])" or R"([A-Za-z\d])" = select alphanumeric
			// 	R"([0-9])" or R"([\d])" = select numeric
			// 	R"([^0-9])" or R"([^\d])" or R"([\D])" = select non - numeric

			static std::regex optionalNegative("^-");
			static std::regex numericExpression("[0-9]+(\.[0-9][0-9]?)?");

			std::string output;
			std::smatch match;

			if (std::regex_search(input, match, optionalNegative))
				output += match[0];			
			if (std::regex_search(input, match, numericExpression))
				output += match[0];
			
			return output;
		}

		static std::string StripToInteger(const std::string& str)
		{
			std::regex integerExpression("(\\+|-)?[[:digit:]]+");
			return std::regex_replace(str, integerExpression, "");
		}

		static std::string StripToFloat(const std::string& str)
		{
			//std::regex rr("((\\+|-)?[[:digit:]]+)(\\.(([[:digit:]]+)?))?");
			std::regex rr("[+-]?([0-9]*[.])?[0-9]+");
			return std::regex_replace(str, rr, "");
		}

		private:
		std::vector<int> GetAsInts(const std::vector<std::string>& elements)
		{
			std::vector<int> values;
			for (int c = 0; c < elements.size(); ++c)
			{
				std::string str = StripToNumeric(elements[c]);
				int value = std::stoi(str);
				values.push_back(value);
			}
			return values;
		}

		std::vector<float> GetAsFloats(const std::vector<std::string>& elements)
		{
			std::vector<float> values;
			for (int c = 0; c < elements.size(); ++c)
			{
				std::string str = StripToNumeric(elements[c]);
				float value = std::stof(str);
				values.push_back(value);
			}
			return values;
		}


	};
}