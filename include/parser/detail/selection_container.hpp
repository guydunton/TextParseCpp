#ifndef SELECTION_CONTAINER_HPP
#define SELECTION_CONTAINER_HPP
#include <string>
#include <vector>
#include <memory>
#include "../utility/meta.hpp"

template <typename OutputT, typename ReturnT>
class SelectionContainer {

	class Base {
	public:
		virtual ~Base() = default;
		virtual ReturnT performMatch(const std::vector<std::string>& parts, OutputT& output) const = 0;
		virtual bool canMatch(const std::vector<std::string>& parts) const = 0;
		
	};

	template <typename MyRet, typename Func>
	class Derived : public Base {
	public:
		Derived(std::vector<size_t> selectionIndices, Func invokeFunction) : 
			selectionIndices(std::move(selectionIndices)),
			invokeFunction(std::move(invokeFunction))
		{}

		template <int... Indices>
		MyRet callMatchFunction(const std::vector<std::string>& parts, OutputT& output, IndexSequence<Indices...>) const {
			invokeFunction((parts.at(selectionIndices.at(Indices)))..., output);
		}

		MyRet performMatch(const std::vector<std::string>& parts, OutputT& output) const override {
			callMatchFunction(parts, output, IndexSequence_t<FunctionTraits<Func>::Arity - 1>{});
		}
		
		bool canMatch(const std::vector<std::string>& parts) const override {
			const size_t maxGrabIndex = *std::max_element(selectionIndices.begin(), selectionIndices.end());
			return parts.size() >= maxGrabIndex + 1;
		}

	private:
		std::vector<size_t> selectionIndices;
		Func invokeFunction;
	};


public:

	SelectionContainer() = default;

	template <typename T>
	void init(std::vector<size_t> selectionIndices, T invokeFunction) {
		this->self.reset(new Derived<ReturnT, T>{ std::move(selectionIndices), std::move(invokeFunction) });
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
