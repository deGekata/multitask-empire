#ifndef STDLIKE_FUNCTION_HPP
#define STDLIKE_FUNCTION_HPP

#include <stdlike/unique_ptr.hpp>

namespace stdlike {

template <typename Signature>
class Function {
};

template <typename ReturnType, typename... ArgTypes>
class Function<ReturnType (ArgTypes...)> {
public:
    Function()
        : invoker_() {
    }

    template <typename FunctionType>
    Function(FunctionType function)
        : invoker_(new FreeFunctionContainer<FunctionType>(function)) {
    }

    template <typename FunctionType, typename ObjectType>
    Function(FunctionType ObjectType::* function)
        : invoker_(new MemberFunctionContainer<FunctionType, ArgTypes...>(function)) {
    }

    Function(const Function& other)
        : invoker_(other.invoker_->clone()) {
    }

    Function& operator=(const Function& other) {
        invoker_ = other.invoker_->clone();
        return *this;
    }

    ReturnType operator()(ArgTypes... args) {
        return invoker_->invoke(args...);
    }

private:
    class FunctionContainerBase {
    public:
        FunctionContainerBase() {
        }

        virtual ~FunctionContainerBase() {
        }

        virtual ReturnType invoke(ArgTypes... args) = 0;

        virtual stdlike::UniquePtr<FunctionContainerBase> clone() = 0;

    private:
        FunctionContainerBase(const FunctionContainerBase&) {
        }

        FunctionContainerBase& operator=(const FunctionContainerBase&) {
        }
    };

    template <typename FunctionType>
    class FreeFunctionContainer : public FunctionContainerBase {
    public:
        FreeFunctionContainer(FunctionType function)
            : FunctionContainerBase()
            , function_(function) {
        }

        virtual ReturnType invoke(ArgTypes... args) final {
            return function_(args...);
        }

        virtual stdlike::UniquePtr<FunctionContainerBase> clone() final {
            return stdlike::UniquePtr<FunctionContainerBase>(new FreeFunctionContainer(function_));
        }

    private:
        FunctionType function_;
    };

    template <typename FunctionType, typename ObjectType, typename... OtherArgTypes>
    class MemberFunctionContainer : public FunctionContainerBase {
    public:
        MemberFunctionContainer(FunctionType ObjectType::* function)
            : FunctionContainerBase()
            , function_(function) {
        }

        virtual ReturnType invoke(ObjectType object, OtherArgTypes... args) final {
            return (object.*function_)(args...);
        }

        virtual stdlike::UniquePtr<FunctionContainerBase> clone() final {
            return stdlike::UniquePtr<FunctionContainerBase>(new MemberFunctionContainer(function_));
        }

    private:
        FunctionType ObjectType::* function_{nullptr};
    };

private:
    stdlike::UniquePtr<FunctionContainerBase> invoker_{nullptr};
};

} // namespace stdlike

#endif // STDLIKE_FUNCTION_HPP
