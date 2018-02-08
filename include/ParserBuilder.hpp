#ifndef MATCH_BUILDER_HPP
#define MATCH_BUILDER_HPP
#include <string>
#include <vector>
#include "Parser.hpp"
#include "Meta.hpp"
#include "Function.hpp"

template <typename OutputT, typename LambdaReturn, typename... Args>
class ParserBuilderImpl {
public:
	ParserBuilderImpl() = default;

	template <typename... OtherArgs>
	ParserBuilderImpl(ParserBuilderImpl<OtherArgs...>&& other) :
		matchingTokens(std::move(other.matchingTokens)),
		selectionIndices(std::move(other.selectionIndices))
	{}
	
	ParserBuilderImpl(ParserBuilderImpl&& other) :
		matchingTokens(std::move(other.matchingTokens)),
		selectionIndices(std::move(other.selectionIndices)),
		invokeFunction(std::move(other.invokeFunction))
	{}

	template <typename... Tokens>
	ParserBuilderImpl& matching(Tokens&&... tokens) {
		foldIntoVector(matchingTokens, tokens...);
		return *this;
	}

	ParserBuilderImpl<OutputT, LambdaReturn, Args..., std::string> selecting(size_t index) {
		selectionIndices.emplace_back(index);
		return std::move(*this);
	}

	template <typename T, typename T2= typename std::enable_if<!std::is_member_pointer<T>::value>::type>
	ParserBuilderImpl<OutputT, typename FunctionTraits<T>::ReturnType, Args...> invoking(T lambda) {
		using NewMatchBuilder = ParserBuilderImpl<OutputT, typename FunctionTraits<T>::ReturnType, Args...>;
		NewMatchBuilder newBuilder{ std::move(*this) };
		newBuilder.invokeFunction = lambda;
		return newBuilder;
	}
    
    template <typename Ret>
    ParserBuilderImpl<OutputT, Ret, Args...> invoking(Ret(*functionPtr)(Args&&...)) {
        ParserBuilderImpl<OutputT, Ret, Args...> newBuilder{ std::move(*this) };
        newBuilder.invokeFunction = functionPtr;
        return newBuilder;
    }

	template <typename Ret, typename Class, typename... FuncArgs>
	ParserBuilderImpl<OutputT, Ret, Args...> invoking(Ret(Class::*memberFunc)(FuncArgs...)) {
		ParserBuilderImpl<OutputT, Ret, Args...> newBuilder{ std::move(*this) };
		newBuilder.invokeFunction = memberFunc;
		return newBuilder;
	}

	template <typename Class, typename Val>
	ParserBuilderImpl<OutputT, void, Args...> invoking(Val(Class::*memberObj)) {
		ParserBuilderImpl<OutputT, void, Args...> newBuilder{ std::move(*this) };
		newBuilder.invokeFunction = memberObj;
		return newBuilder;
	}
	
	Parser<OutputT, LambdaReturn> finalize() {
		SelectionContainer<OutputT, LambdaReturn> selection;
		selection.init(std::move(selectionIndices), std::move(invokeFunction));

		return Parser<OutputT, LambdaReturn>(matchingTokens, selection);
	}

private:

	template <typename OtherOutput, typename OtherReturn, typename... OtherArgs>
	friend class ParserBuilderImpl;

	std::vector<std::string> matchingTokens;
	std::vector<size_t> selectionIndices;
    ParserSpace::Function<LambdaReturn(const Args&..., OutputT&)> invokeFunction;
};

template <typename OutputT>
ParserBuilderImpl<OutputT, void> createParser() {
	return {};
}


#endif
