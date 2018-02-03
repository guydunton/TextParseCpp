#ifndef META_HPP
#define META_HPP

//-------------------------------------------------------------
// Function Traits
//-------------------------------------------------------------
template <typename T>
struct FunctionTraits : public FunctionTraits<decltype(&T::operator())> {};

template <typename Ret, typename Class, typename... Args>
struct FunctionTraits<Ret(Class::*)(Args...) const> {
	static const int Arity = sizeof...(Args);
	using ReturnType = Ret;
};

//-------------------------------------------------------------
// Function folding
//-------------------------------------------------------------
template <typename T, typename... Args>
void foldIntoVector(std::vector<T>& vec, Args&&... args) {
	// This is a terrible hack but it enables forwarding before C++17
	(void)std::initializer_list<int>{ (vec.emplace_back(std::forward<Args>(args)), 0)... };
}

//-------------------------------------------------------------
// Indices Sequence
//-------------------------------------------------------------
template <int... Vals> struct IndexSequence {};

namespace Detail {

	template <int ... Vals> struct IndexSequenceGen;

	template <int I, int... Args> struct IndexSequenceGen<I, Args...> {
		// Recursively create a sequence with decremented values
		using type = typename IndexSequenceGen<I - 1, I - 1, Args...>::type;
	};

	// Base of recursion
	template<int ... Args> struct IndexSequenceGen<0, Args...> {
		using type = IndexSequence<Args...>;
	};
}



// This will generate a sequence of integers. e.g. IndexSequence_t<5> => sequence<0, 1, 2, 3, 4>;
// To use put as parameter to template function e.g. 
// template <int... Indices> void func(sequence<Indices>);
template <int N>
using IndexSequence_t = typename Detail::IndexSequenceGen<N>::type;

static_assert(std::is_same<IndexSequence_t<3>, IndexSequence<0, 1, 2>>::value, "Failed");
#endif
