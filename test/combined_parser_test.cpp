#include "catch.hpp"
#include <parser/parser_builder.hpp>

TEST_CASE("A combined parser does the same thing as both componenets")
{
    const auto text1 = "description desc";
    const auto text2 = "book-name Dune";
    
    const auto descParser = createParser<std::string>()
        .matching("description")
        .selecting(1)
        .invoking([](const std::string& description, std::string& output){
            output = description;
        }).finalize();
    
    const auto bookNameParser = createParser<std::string>()
        .matching("book-name")
        .selecting(1)
        .invoking([](const std::string& name, std::string& output){
            output = name;
        }).finalize();
    
    const Parser<std::string> combinedParser = descParser & bookNameParser;
    
    std::string descOutput;
    combinedParser.parse(text1, &descOutput);
    REQUIRE(descOutput == "desc");
    
    std::string nameOutput;
    combinedParser.parse(text2, &nameOutput);
    REQUIRE(nameOutput == "Dune");
}
