#ifndef PARSER_HPP
#define PARSER_HPP

#include <memory>
#include "detail/parser_core.hpp"
#include "detail/combined_parser_core.hpp"

template <typename OutputT, typename LambdaReturn = void>
class Parser
{
    using ParserCoreT = ParserCore<OutputT, LambdaReturn>;
public:
    Parser() = default;
    
    Parser(std::vector<std::string> matchingTokens, SelectionContainer<OutputT, LambdaReturn> selectionContainer) :
        parsingCore(std::make_shared<ParserCoreT>(std::move(matchingTokens), std::move(selectionContainer)))
    {}
    
    Parser(std::vector<std::shared_ptr<ParserCoreT>> parsers) :
        parsingCore(std::make_shared<CombinedParserCore<OutputT, LambdaReturn>>(std::move(parsers)))
    {}
    
    void parse(const std::string& text, OutputT* output) const
    {
		for (const auto& line : Text::splitString(text, '\n'))
		{
			parsingCore->parse(line, output);
		}
    }
    
    Parser mergeWithParser(const Parser& other) const
    {
        return Parser({ parsingCore, other.parsingCore });
    }
    
private:
    std::shared_ptr<ParserCoreT> parsingCore;
};

template <typename OutputT, typename LambdaReturn>
Parser<OutputT, LambdaReturn> operator&(const Parser<OutputT, LambdaReturn>& lhs,
                                        const Parser<OutputT, LambdaReturn>& rhs)
{
    return lhs.mergeWithParser(rhs);
}

#endif
