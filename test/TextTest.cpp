#include "catch.hpp"
#include "Text.hpp"

TEST_CASE("Split string works")
{
	const auto text1 = "it is a trap";
	const auto result1 = Text::splitString(text1, ' ');

	REQUIRE(result1.size() == 4);

	const auto text2 = "something::jim";
	const auto result2 = Text::splitString(text2, ':');
	REQUIRE(result2.size() == 3);
}

TEST_CASE("lineParts works correctly")
{
	const auto text1 = "    hello world";
	const auto result1 = Text::lineParts(text1);

	REQUIRE(result1.size() == 2);
}