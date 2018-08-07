#pragma once

#include <string>
#include <sstream>

namespace Oxana
{
	// An utility class for building strings for logging purposes
	class StringBuilder
	{
		public:

		StringBuilder() {}
		StringBuilder(const StringBuilder& other)
		{
			this->stream << other.stream.str();
		}

		StringBuilder& operator=(const StringBuilder& other)
		{
			this->stream << other.stream.str();
			return *this;
		}

		template <typename T>
		void Append(const T& t)
		{
			stream << t;
			used = true;
		}

		template <typename First, typename... Rest>
		void Append(const First& first, const Rest&... rest)
		{
			stream << first;
			used = true;
			Append(rest...); // recursive call using pack expansion syntax  
		}

		template <typename T>
		void AppendLine(const T& t)
		{
			stream << t << std::endl;
			used = true;
		}

		template <typename First, typename... Rest>
		void AppendLine(const First& first, const Rest&... rest)
		{
			stream << first;
			used = true;
			AppendLine(rest...); // recursive call using pack expansion syntax  
		}

		void Clear()
		{
			stream = std::stringstream();
			used = false;
		}

		bool IsEmpty()
		{
			return !used;
		}

		std::string ToString()
		{
			return stream.str();
		}

		private:
		std::stringstream stream;
		bool used;

	};

}