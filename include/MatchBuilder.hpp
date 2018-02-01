#ifndef MATCH_BUILDER_HPP
#define MATCH_BUILDER_HPP
#include <functional>
#include <string>
#include <vector>
#include "Matcher.hpp"
#include "Meta.hpp"

template <typename T>
struct Tester;

template <typename OutputT, typename LambdaReturn, typename... Args>
class MatchBuilderImpl {
public:
	MatchBuilderImpl() = default;

	template <typename... OtherArgs>
	MatchBuilderImpl(MatchBuilderImpl<OtherArgs...>&& other) :
		matchingTokens(std::move(other.matchingTokens)),
		selectionIndices(std::move(other.selectionIndices))
	{}
	
	MatchBuilderImpl(MatchBuilderImpl&& other) :
		matchingTokens(std::move(other.matchingTokens)),
		selectionIndices(std::move(other.selectionIndices)),
		invokeFunction(std::move(other.invokeFunction))
	{}

	template <typename... Tokens>
	MatchBuilderImpl& matching(Tokens&&... tokens) {
		foldIntoVector(matchingTokens, tokens...);
		return *this;
	}

	MatchBuilderImpl<OutputT, LambdaReturn, Args..., std::string> selecting(size_t index) {
		selectionIndices.emplace_back(index);
		return std::move(*this);
	}

	template <typename T>
	MatchBuilderImpl<OutputT, typename FunctionTraits<T>::ReturnType, Args...> invoking(T lambda) {
		using NewMatchBuilder = MatchBuilderImpl<OutputT, typename FunctionTraits<T>::ReturnType, Args...>;
		NewMatchBuilder newBuilder{ std::move(*this) };
		newBuilder.invokeFunction = lambda;
		return newBuilder;
	}
	
	Matcher<OutputT, LambdaReturn> finalize() {
		SelectionContainer<OutputT, LambdaReturn> selection;
		selection.template init<Args...>(std::move(selectionIndices), std::move(invokeFunction));

		return Matcher<OutputT, LambdaReturn>(matchingTokens, selection);
	}

private:

	template <typename OtherOutput, typename OtherReturn, typename... OtherArgs>
	friend class MatchBuilderImpl;

	std::vector<std::string> matchingTokens;
	std::vector<size_t> selectionIndices;
	std::function<LambdaReturn(const Args&..., OutputT&)> invokeFunction;
};

template <typename OutputT>
MatchBuilderImpl<OutputT, void> createMatcher() {
	return {};
}


#endif
