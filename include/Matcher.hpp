#ifndef MATCHER_HPP
#define MATCHER_HPP

#include "Text.hpp"
#include "Meta.hpp"
#include <functional>
#include <vector>

/**
 *
 * line: description Loopback
 * auto matcher = createMatcher()
 *		.matching("description")
 *		.grab(1)
 *		.calling([](const std::string& desc, Config& config){
 *			config->description = desc;
 *		});
 *
 * decltype(matcher) == Matcher<void, std::string, Config>;
 *
 */

template <typename Output_t, typename... Args>
struct Matcher
{
private:
	std::vector<std::string> matchers;
	std::vector<int> grabbing;
	std::function<void(const Args&..., Output_t&)> callingFunc;

	bool canParse(const std::vector<std::string>& parts) const
	{
		const int maxGrabIndex = *std::max_element(grabbing.begin(), grabbing.end());
		return parts.size() >= std::max(static_cast<unsigned int>(matchers.size()), 
		static_cast<unsigned int>(maxGrabIndex) + 1);
	}

	template <int... Indices>
	void callFunc(const std::vector<std::string>& parts, Output_t& output, sequence<Indices...>) const
	{
		callingFunc( (parts.at(grabbing.at(Indices)))... , output);
	}

public:
	Matcher() = default;
	Matcher(std::vector<std::string> matchers, std::vector<int> grabbing):
		matchers(std::move(matchers)),
		grabbing(std::move(grabbing))
	{}

	template <typename... MatchingArgs>
	Matcher& matching(MatchingArgs&&... args)
	{
		(void)std::initializer_list<int>{ (matchers.push_back(args), 0)...};
		return *this;
	}

	auto grab(int index) -> Matcher<Output_t, Args..., std::string>
	{
		grabbing.push_back(index);
		return { std::move(matchers), std::move(grabbing) };
	}

	Matcher& call(const std::function<void(const Args&..., Output_t&)>& callingFunc)
	{
		this->callingFunc = callingFunc;
		return *this;
	}

	void match(const std::string& text, Output_t* output) const
	{
		const auto splitText = Text::lineParts(text);

		if(canParse(splitText))
		{
			if (std::equal(matchers.begin(), matchers.end(), splitText.begin()))
			{
				callFunc(splitText, *output, sequence_t<sizeof...(Args)>{});
			}
		}
	}
};

template <typename T>
Matcher<T> createMatcher()
{
	return Matcher<T>{};
}


#endif