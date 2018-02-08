#ifndef FunctionImpl_hpp
#define FunctionImpl_hpp

namespace ParserSpace {
    namespace Detail {
        template <typename Ret, typename... Args>
        struct FuncBase {
            virtual Ret call(Args&&... args) const = 0;
        };
        
        template <typename T, typename Ret, typename... Args>
        struct LambdaFunc : public FuncBase<Ret, Args...> {
            T t;
            LambdaFunc(T t) : t(t) {}
            Ret call(Args&&... args) const override {
                return t(std::forward<Args>(args)...);
            };
        };
        
        template <typename Ret, typename... Args>
        struct PtrFunc : public FuncBase<Ret, Args...>{
            Ret (*f)(Args...);
            PtrFunc(Ret(*f)(Args...)) : f(f) {}
            Ret call(Args&&... args) const override {
                return f(std::forward<Args>(args)...);
            }
        };
        
        template <typename Class, typename Ret, typename... Args>
        struct MemberPtrFunc : public FuncBase<Ret, Args..., Class&> {
            Ret (Class::*func)(Args...);
            MemberPtrFunc(Ret (Class::*f)(Args...)) : func(f) {}
            
            Ret call(Args&&... args, Class& c) const override {
                return (c.*func)(std::forward<Args>(args)...);
            };
        };
        
        template <typename Class, typename Ret, typename... Args>
        struct MemberPtrConstFunc : public FuncBase<Ret, Args..., Class&> {
            Ret (Class::*func)(Args...) const;
            MemberPtrConstFunc(Ret (Class::*f)(Args...) const) : func(f) {}
            
            Ret call(Args&&... args, Class& c) const override {
                return (c.*func)(std::forward<Args>(args)...);
            };
        };
        
        template <typename Class, typename Ret, typename Val>
        struct MemberObjFunc : public FuncBase<void, Val, Class&> {
            Ret (Class::*func);
            MemberObjFunc(Ret (Class::*fun)) : func(fun) {}
            void call(Val&& val, Class& c) const override {
                (c.*func) = std::forward<Val>(val);
            }
        };
    }
}

#endif
