#include "catch.hpp"
#include "Matcher.hpp"

TEST_CASE("Matcher grabs correct part of string")
{
	const auto text1 = "description something";
	const auto matcher = createMatcher<std::string>()
		.matching("description")
		.grab(1)
		.call([](const std::string& description, std::string& output)
		{
			output = description;
		});

	std::string result1;
	matcher.match(text1, &result1);
	REQUIRE(result1 == "something");

	const auto text2 = "something completely different";
	std::string result2;
	matcher.match(text2, &result2);
	REQUIRE(result2.empty());
}

TEST_CASE("Matcher doesn't out-of-range exceptions when the line is too small")
{
	const auto text1 = "";
	const auto matcher = createMatcher<std::string>()
		.matching("description")
		.grab(1)
		.call(nullptr);

	std::string result1;
	REQUIRE_NOTHROW(matcher.match(text1, &result1));
	REQUIRE(result1.empty());
}

TEST_CASE("Matcher works matching multiple parts in order")
{
	bool lambdaCalled = false;
	const auto matcher = createMatcher<std::string>()
		.matching("ip", "address")
		.grab(2)
		.call([&lambdaCalled](const std::string&, std::string& )
		{
			lambdaCalled = true;
		});

	SECTION("Works with text that matches")
	{
		const auto text1 = "ip address addressName";
		std::string result1;
		matcher.match(text1, &result1);
		REQUIRE(lambdaCalled);
	}

	SECTION("Doesn't call function that doesn't match")
	{
		const auto text2 = "ip address1 addressName";
		std::string result2;
		matcher.match(text2, &result2);
		REQUIRE(!lambdaCalled);
	}
}

TEST_CASE("Matcher grabbing multiple parts works correctly")
{
	const auto matcher = createMatcher<std::string>()
		.matching("ip")
		.grab(1)
		.grab(2)
		.call([](const std::string& part1, const std::string& part2, std::string& output)
		{
			output = part1 + " " + part2;
		});

	const auto text1 = "ip matcher test";

	std::string result1;

	matcher.match(text1, &result1);
	REQUIRE(result1 == "matcher test");
}
