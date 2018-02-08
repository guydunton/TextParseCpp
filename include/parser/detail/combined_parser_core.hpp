#ifndef CombinedParser_hpp
#define CombinedParser_hpp

#include <memory>
#include <vector>
#include "parser_core.hpp"

template<typename T, typename U>
class Parser;

template <typename OutputT, typename LambdaReturn>
class CombinedParserCore : public ParserCore<OutputT, LambdaReturn>
{
    using NestedParserPtr = std::shared_ptr<ParserCore<OutputT, LambdaReturn>>;
    
public:
    CombinedParserCore(std::vector<NestedParserPtr> parsers) :
        parsers(std::move(parsers))
    {}
    
protected:
    void performParse(const std::string& text, OutputT& output) const override
    {
        for(const auto& parser : parsers)
        {
            parser->parse(text, &output);
        }
    }
    
private:
    std::vector<NestedParserPtr> parsers;
};

#endif
