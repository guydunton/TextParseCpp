#ifndef TEXT_HPP
#define TEXT_HPP
#include <vector>
#include <algorithm>

namespace Text
{
	inline std::vector<std::string> splitString(const std::string& text, char separator) {
		std::vector<std::string> elems;
		auto start = text.begin();
		auto until = text.end();
		do {
			until = std::find(start, text.end(), separator);
			elems.emplace_back(start, until);

			// Don't like this but just until it's working correctly
			if (until != text.end()) start = until + 1;
		} while (until != text.end());
		return elems;
	}

	inline std::vector<std::string> lineParts(const std::string& line) {
		auto result = splitString(line, ' ');
		result.erase(std::remove(result.begin(), result.end(), ""), result.end());
		return result;
	}

}

#endif