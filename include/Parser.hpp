#ifndef MATCHER_HPP
#define MATCHER_HPP

#include "Text.hpp"
#include "Meta.hpp"
#include "SelectionContainer.hpp"
#include <functional>
#include <vector>

template <typename OutputT, typename LambdaReturn = void>
class Parser
{
public:

	Parser() = default;

	Parser(std::vector<std::string> matchingTokens, SelectionContainer<OutputT, LambdaReturn> selectionContainer) :
		matchingTokens(std::move(matchingTokens)), selectionContainer(std::move(selectionContainer))
	{}


	void match(const std::string& text, OutputT* output) const
	{
		const auto splitText = Text::lineParts(text);

		if(canParse(splitText))
		{
			if (std::equal(matchingTokens.begin(), matchingTokens.end(), splitText.begin()))
			{
				selectionContainer.performMatch(splitText, *output);
			}
		}
	}

	bool canParse(const std::vector<std::string>& parts) const
	{
		return parts.size() >= static_cast<unsigned int>(matchingTokens.size()) && selectionContainer.canMatch(parts);
	}

private:
	std::vector<std::string> matchingTokens;
	SelectionContainer<OutputT, LambdaReturn> selectionContainer;
};


#endif
