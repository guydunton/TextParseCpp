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

TEST_CASE("We can add together more than 2 parsers")
{
	const auto descParser = createParser<std::string>()
		.matching("description")
		.selecting(1)
		.invoking([](const std::string& desc, std::string& output){	
			output = desc;
		}).finalize();
	const auto nameParser = createParser<std::string>()
		.matching("name")
		.selecting(1)
		.invoking([](const std::string& name, std::string& output){
			output = name;
		}).finalize();
	const auto dateParser = createParser<std::string>()
		.matching("date")
		.selecting(1)
		.invoking([](const std::string& date, std::string& output){
			output = date;
		}).finalize();
	const auto pagesParser = createParser<std::string>()
		.matching("pages")
		.selecting(1)
		.invoking([](const std::string& pages, std::string& output){
			output = pages;
		}).finalize();

	const auto combined = descParser & nameParser & dateParser & pagesParser;
	std::string output;


	combined.parse("description desc", &output);
	REQUIRE(output == "desc");

	combined.parse("name john", &output);
	REQUIRE(output == "john");

	combined.parse("date today", &output);
	REQUIRE(output == "today");

	combined.parse("pages 412", &output);
	REQUIRE(output == "412");
}
