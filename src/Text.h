#ifndef TEXT_H
#define TEXT_H
#include <vector>

namespace Text
{

	std::vector<std::string> splitString(const std::string& text, char separator);

	std::vector<std::string> lineParts(const std::string& line);

}

#endif