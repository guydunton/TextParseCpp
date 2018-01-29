#include "Text.h"
#include <sstream>
#include <algorithm>
#include <iterator>

namespace
{
	template<typename Out>
	void split(const std::string &s, char delim, Out result) {
		std::stringstream ss(s);
		std::string item;
		while (std::getline(ss, item, delim)) {
			*(result++) = item;
		}
	}

	std::vector<std::string> split(const std::string &s, char delim) {
		std::vector<std::string> elems;
		split(s, delim, std::back_inserter(elems));
		return elems;
	}
}

namespace Text
{

	

	std::vector<std::string> splitString(const std::string& text, char separator)
	{
		return split(text, separator);
	}

	std::vector<std::string> lineParts(const std::string& line)
	{
		auto result = split(line, ' ');
		result.erase(std::remove(result.begin(), result.end(), ""), result.end());
		return result;
	}

}