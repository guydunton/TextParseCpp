#ifndef META_HPP
#define META_HPP

//-------------------------------------------------------------
// Push Back
//-------------------------------------------------------------

template <typename List, typename New> struct PushBack;

template<template<typename...> class List, typename... Elements, typename New>
struct PushBack<List<Elements...>, New>
{
	using type = List<Elements..., New>;
};

//-------------------------------------------------------------
// Pop front
//-------------------------------------------------------------
template <typename List> struct PopFront;

template <template<typename...> class List, typename... Elements, typename Front>
struct PopFront<List<Front, Elements...>>
{
	using type = List<Elements...>;
};

//-------------------------------------------------------------
// Get Front
//-------------------------------------------------------------
template <typename List> struct Front;

template <template <typename...> class List, typename... Elements, typename First>
struct Front<List<First, Elements...>>
{
	using type = First;
};


//-------------------------------------------------------------
// Indices Sequence
//-------------------------------------------------------------
template <int... Vals> struct sequence {};

template <int ... Vals> struct seq_gen;

template <int I, int... Args> struct seq_gen<I, Args...>
{
	// Recursively create a sequence with decremented values
	using type = typename seq_gen<I - 1, I - 1, Args...>::type;
};
// Base of recursion
template<int ... Args> struct seq_gen<0, Args...>
{
	using type = sequence<Args...>;
};

// This will generate a sequence of integers. e.g. sequence_t<5> => sequence<0, 1, 2, 3, 4>;
// To use put as parameter to template function e.g. 
// template <int... Indices> void func(sequence<Indices>);
template <int N>
using sequence_t = typename seq_gen<N>::type;

#endif