#ifndef SELECTION_CONTAINER_HPP
#define SELECTION_CONTAINER_HPP
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include "Meta.hpp"

template <typename OutputT, typename ReturnT>
class SelectionContainer {

	class Base {
	public:

		Base(std::vector<size_t> selectionIndices) : selectionIndices(std::move(selectionIndices)) {}

		virtual ~Base() = default;
		virtual ReturnT performMatch(const std::vector<std::string>& parts, OutputT& output) const = 0;

		bool canMatch(const std::vector<std::string>& parts) const {
			const size_t maxGrabIndex = *std::max_element(selectionIndices.begin(), selectionIndices.end());
			return parts.size() >= maxGrabIndex + 1;
		}
	protected:
		std::vector<size_t> selectionIndices;
	};

	template <typename MyRet, typename... Args>
	class Derived : public Base {
	public:

		using InvokeFunctionT = std::function<ReturnT(const Args&..., OutputT&)>;

		Derived(std::vector<size_t> selectionIndices, InvokeFunctionT invokeFunction) : 
			Base(std::move(selectionIndices)),
			invokeFunction(std::move(invokeFunction))
		{}

		template <int... Indices>
		MyRet callMatchFunction(const std::vector<std::string>& parts, OutputT& output, IndexSequence<Indices...>) const {
			invokeFunction((parts.at(selectionIndices.at(Indices)))..., output);
		}

		MyRet performMatch(const std::vector<std::string>& parts, OutputT& output) const override {
			callMatchFunction(parts, output, IndexSequence_t<sizeof...(Args)>{});
		}

	private:
		InvokeFunctionT invokeFunction;
	};


public:

	SelectionContainer() = default;

	template <typename... Args>
	void init(std::vector<size_t> selectionIndices, std::function<ReturnT(const Args&..., OutputT&)> invokeFunction) {
		this->self.reset(new Derived<ReturnT, Args...>{ std::move(selectionIndices), std::move(invokeFunction) });
	}


	ReturnT performMatch(const std::vector<std::string>& parts, OutputT& output) const {
		return self->performMatch(parts, output);
	}

	bool canMatch(const std::vector<std::string>& parts) const {
		return self->canMatch(parts);
	}

private:
	std::shared_ptr<Base> self;
};

#endif