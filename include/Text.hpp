#ifndef TEXT_HPP
#define TEXT_HPP
#include <vector>
#include <sstream>
#include <algorithm>
#include <iterator>

namespace Text
{
	inline std::vector<std::string> splitString(const std::string& text, char separator) {
		std::vector<std::string> elems;
		auto result = std::back_inserter(elems);
		std::stringstream ss(text);
		std::string item;
		while (std::getline(ss, item, separator)) {
			*(result++) = item;
		}
		return elems;
	}

	inline std::vector<std::string> lineParts(const std::string& line) {
		auto result = splitString(line, ' ');
		result.erase(std::remove(result.begin(), result.end(), ""), result.end());
		return result;
	}

}

#endif