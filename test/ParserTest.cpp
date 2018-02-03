#include "catch.hpp"
#include "ParserBuilder.hpp"

TEST_CASE("Parser grabs correct part of string")
{
	const auto text1 = "description something";
	const auto parser = createParser<std::string>()
		.matching("description")
		.selecting(1)
		.invoking([](const std::string& description, std::string& output) {
			output = description;
		}).finalize();

	std::string result1;
	parser.parse(text1, &result1);
	REQUIRE(result1 == "something");

	const auto text2 = "something completely different";
	std::string result2;
	parser.parse(text2, &result2);
	REQUIRE(result2.empty());
}

TEST_CASE("Parser doesn't out-of-range exceptions when the line is too small")
{
	const auto text1 = "";
	const auto parser = createParser<std::string>()
		.matching("description")
		.selecting(1)
		.invoking([](const std::string&, std::string&){})
		.finalize();

	std::string result1;
	REQUIRE_NOTHROW(parser.parse(text1, &result1));
	REQUIRE(result1.empty());
}

TEST_CASE("Parser works matching multiple parts in order")
{
	bool lambdaCalled = false;
	const auto parser = createParser<std::string>()
		.matching("ip", "address")
		.selecting(2)
		.invoking([&lambdaCalled](const std::string&, std::string& )
		{
			lambdaCalled = true;
		}).finalize();

	SECTION("Works with text that matches")
	{
		const auto text1 = "ip address addressName";
		std::string result1;
		parser.parse(text1, &result1);
		REQUIRE(lambdaCalled);
	}

	SECTION("Doesn't call function that doesn't match")
	{
		const auto text2 = "ip address1 addressName";
		std::string result2;
		parser.parse(text2, &result2);
		REQUIRE(!lambdaCalled);
	}
}

TEST_CASE("Parser grabbing multiple parts works correctly")
{
	const auto parser = createParser<std::string>()
		.matching("ip")
		.selecting(1)
		.selecting(2)
		.invoking([](const std::string& part1, const std::string& part2, std::string& output)
		{
			output = part1 + " " + part2;
		}).finalize();

	const auto text1 = "ip matcher test";

	std::string result1;

	parser.parse(text1, &result1);
	REQUIRE(result1 == "matcher test");
}

