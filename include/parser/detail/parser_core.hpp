#ifndef PARSER_CORE_HPP
#define PARSER_CORE_HPP

#include "../utility/text.hpp"
#include "selection_container.hpp"
#include <vector>

template <typename OutputT, typename LambdaReturn = void>
class ParserCore
{
public:

	ParserCore() = default;

	ParserCore(std::vector<std::string> matchingTokens, SelectionContainer<OutputT, LambdaReturn> selectionContainer) :
		matchingTokens(std::move(matchingTokens)), selectionContainer(std::move(selectionContainer))
	{}


	void parse(const std::string& text, OutputT* output) const
	{
        performParse(text, *output);
	}
    
protected:
    virtual void performParse(const std::string& text, OutputT& output) const
    {
		const auto splitText = Text::lineParts(text);

		if (canParse(splitText))
		{
			if (std::equal(matchingTokens.begin(), matchingTokens.end(), splitText.begin()))
			{
				selectionContainer.performMatch(splitText, output);
			}
		}
    }

private:
    
    bool canParse(const std::vector<std::string>& parts) const
    {
        return parts.size() >= static_cast<unsigned int>(matchingTokens.size()) && selectionContainer.canMatch(parts);
    }
    
	std::vector<std::string> matchingTokens;
	SelectionContainer<OutputT, LambdaReturn> selectionContainer;
};


#endif
