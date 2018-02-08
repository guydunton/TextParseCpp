#ifndef Function_hpp
#define Function_hpp

#include <memory>
#include "function_impl.hpp"
#include "meta.hpp"

namespace ParserSpace {
    
    template <typename> class Function;
    
    template <typename Return, typename... Args>
    class Function<Return(Args...)> {
    public:
        
        Function() = default;
        ~Function() = default;

		Function(const Function&) = delete;
		Function& operator=(const Function&) = delete;

		Function(Function&& other) : 
    		base(std::move(other.base))
		{}

		Function& operator=(Function&& other)
		{
			if (this == &other) return *this;
			base = std::move(other.base);
			return *this;
		}
        
        template <typename T> Function(T lambda) :
        base(new Detail::LambdaFunc<T, Return, Args...>(lambda))
        {
            using Traits = FunctionTraits<T>;
            static_assert(std::is_same<typename Traits::ReturnType, Return>::value,
                          "Return type for the lambda must be the same as the Function");
			static_assert(std::is_same<typename Traits::TupleType, std::tuple<Args...>>::value,
				"Lambda args much mash Function args");
        }
        
        template <typename Ret, typename... FuncArgs>
        Function(Ret(*func)(FuncArgs...)) :
            base(new Detail::PtrFunc<Ret, FuncArgs...>(func))
        {
            static_assert(std::is_same<Ret, Return>::value,
                          "Function pointer return type must be the same as Function");
            static_assert(std::is_same<std::tuple<FuncArgs...>, std::tuple<Args...>>::value,
                          "Function arguments must match");
        }
        
        template <typename Ret, typename Class, typename... FuncArgs>
        Function(Ret(Class::*func)(FuncArgs...)) :
            base(new Detail::MemberPtrFunc<Class, Ret, FuncArgs...>(func))
        {
            static_assert(std::is_same<Ret, Return>::value,
                          "Function pointer return type must be the same as Function");
            static_assert(std::is_same<std::tuple<FuncArgs..., Class&>, std::tuple<Args...>>::value,
                          "Function arguments must match with Class& at the end");
        }
        
        template <typename Ret, typename Class, typename... FuncArgs>
        Function(Ret(Class::*func)(FuncArgs...) const) :
            base(new Detail::MemberPtrConstFunc<Class, Ret, FuncArgs...>(func))
        {
            static_assert(std::is_same<Ret, Return>::value,
                          "Function pointer return type must be the same as Function");
            static_assert(std::is_same<std::tuple<FuncArgs..., Class&>, std::tuple<Args...>>::value,
                          "Function arguments must match with Class& at the end");
        }
        
        template <typename T, typename Class>
        Function(T (Class::*objPtr)) :
        base(new Detail::MemberObjFunc<Class, T, typename TakeFirst<Args...>::type>(objPtr))
        {
            static_assert(sizeof...(Args) == 2, "2 Arguments must be provided for the Function");
//            static_assert(std::is_same<std::tuple<typename std::decay<Args>::type...>, std::tuple<T, Class>>::value,
//                          "Arguments must be the type of the value followed by a Class&");
            // We need to check each parameter in Args seperately because we need to decay the first but not the second
        }
        
        Return operator()(Args&&... args) const {
            return base->call(std::forward<Args>(args)...);
        }
        
    private:
        std::unique_ptr<Detail::FuncBase<Return, Args...>> base;
    };
}

#endif
