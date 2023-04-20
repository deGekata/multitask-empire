/**
 * @defgroup   SIGNAL signal
 *
 * @brief      This file Signal system needed for ECS Events.
 *
 * @author     Stanislav
 * @date       2023
 */

#ifndef SIGNAL_HPP
#define SIGNAL_HPP

// Common
#include <algorithm>
#include <functional>
#include <memory>
#include <utility>

// Containers
#include <list>
#include <vector>

namespace signal {


/**
 * @brief      Collects callback results and yields back
 *             the last invoked callback result. Emits infinitely.
 *
 * @tparam     ResultType  Callback result.
 */
template <typename ResultType>
class CollectorLast {
public:
	using CollectorResult = ResultType;

	CollectorLast()
		: last_result_() {
	}

	bool operator()(ResultType result) {
		last_result_ = result;
		return true;
	}

	CollectorResult Result() {
		return last_result_;
	}

private:
	ResultType last_result_;
};


/**
 * @brief      A void CollectorLast specialization.
 */
template <>
class CollectorLast<void> {
public:
	using CollectorResult = void;

	CollectorLast() {
	}

	bool operator()() {
		return true;
	}

	CollectorResult Result() {
	}
};

/**
 * @brief      Similar to CollectorLast, but also makes
 *             Signal emit callbacks untill result is false.
 *
 * @tparam     ResultType  Callback result.
 */
template <typename ResultType>
class CollectorWhileFalse {
public:
	using CollectorResult = ResultType;

	CollectorWhileFalse()
		: result_() {
	}

	CollectorResult Result() {
		return result_;
	}

	bool operator()(ResultType result) {
		result_ = result;
		return result_ ? false : true;
	}

private:
	ResultType result_;
};

/**
 * @brief      Similar to CollectorLast, but also makes
 *             Signal emit callbacks untill result is true.
 *
 * @tparam     ResultType  Callback result.
 */
template <typename ResultType>
class CollectorWhileTrue {
public:
	using CollectorResult = ResultType;

	CollectorWhileTrue()
		: result_() {
	}

	CollectorResult Result() {
		return result_;
	}

	bool operator()(ResultType result) {
		result_ = result;
		return result_ ? true : false;
	}

private:
	ResultType result_;
};

/**
 * @brief      Yields results of all the emmited callbacks
 *             stored in a vector. Emits infinitely.
 *
 * @tparam     ResultType  Callback result.
 */
template <typename ResultType>
class CollectorVector {
public:
	using CollectorResult = std::vector<ResultType>;

	CollectorVector()
		: result_() {
	}

	CollectorResult Result() {
		return result_;
	}

	bool operator()(ResultType result) {
		result_.push_back(result);
		return true;
	}

private:
	std::vector<ResultType> result_;
};

template <typename Collector, typename Signature>
class CollectorInvoker {
};

/**
 * @brief      Invokes a collector.
 *
 * @tparam     Collector   Collector type used.
 * @tparam     ReturnType  Callback return type.
 * @tparam     ArgTypes    Callback arguments.
 */
template <typename Collector, typename ReturnType, typename... ArgTypes>
class CollectorInvoker<Collector, ReturnType (ArgTypes...)> {
public:
	CollectorInvoker() {
	}

	bool Invoke(Collector& collector, const std::function<ReturnType (ArgTypes...)>& func, ArgTypes... args) const {
		return collector(func(args...));
	}
};

/**
 * @brief      A void CollbackInvoker specialization.
 *
 * @tparam     Collector  Collector type used.
 * @tparam     ArgTypes   Callback arguments.
 */
template <typename Collector, typename... ArgTypes>
class CollectorInvoker<Collector, void (ArgTypes...)> {
public:
	CollectorInvoker() {
	}

	bool Invoke(Collector& collector, const std::function<void (ArgTypes...)>& func, ArgTypes... args) const {
		func(args...);
		return collector();
	}
};

template <typename Signature, typename Collector>
class SignalBase {
};

/**
 * @brief      Holds the main Signal logic.
 *
 * @tparam     Collector   Collector type used.
 * @tparam     ReturnType  Callback return type.
 * @tparam     ArgTypes    Callback arguments.
 */
template <typename Collector, typename ReturnType, typename... ArgTypes>
class SignalBase<ReturnType (ArgTypes...), Collector>
	: private CollectorInvoker<Collector, ReturnType (ArgTypes...)> {
public:
	using CollectorResult = typename Collector::CollectorResult;
	using CallBack = std::function<ReturnType (ArgTypes...)>;
	using CallBackId = uintptr_t;

	SignalBase()
		: CollectorInvoker<Collector, ReturnType (ArgTypes...)>()
		, callback_list_() {
	}

	SignalBase(const CallBack& callback)
		: CollectorInvoker<Collector, ReturnType (ArgTypes...)>()
		, callback_list_() {

		Connect(callback);
	}

	CallBackId Connect(const CallBack& callback) {
		// Check whether std::function holds a callable object
		if (!static_cast<bool>(callback)) {
			return 0;
		}
		callback_list_.emplace_back(std::make_unique<CallBack>(callback));
		return reinterpret_cast<CallBackId>(callback_list_.back().get());
	}

	bool Disconnect(CallBackId id) {
		auto begin = callback_list_.begin();
		auto end   = callback_list_.end();

		auto remove = std::remove_if(begin, end, [id](const std::unique_ptr<CallBack>& callback) {
			return id == reinterpret_cast<CallBackId>(callback.get());
		});

	    bool removed = remove != end;
    	callback_list_.erase(remove, end);
	    return removed;
	}

	CollectorResult Emit(ArgTypes... args) const {
		Collector collector;
		for (auto& callback : callback_list_) {
			// Collector policy determines whether to continue emission or not
			if (!this->Invoke(collector, *callback, args...)) {
				break;
			}
		}
		return collector.Result();
	}

	size_t Size() const {
		return callback_list_.size();
	}

private:
	std::list<std::unique_ptr<CallBack>> callback_list_;
};

/**
 * @brief      Provides all the SignalBase functionality as well
 *             as a better interface with a default collector.
 *
 * @tparam     Signature  Callback signature.
 * @tparam     Collector  Collector type used.
 */
template <typename Signature, typename Collector = CollectorLast<typename std::function<Signature>::result_type>>
class Signal final
	: public SignalBase<Signature, Collector> {
public:
	using BaseSignal = SignalBase<Signature, Collector>;
	using CallBack = typename BaseSignal::CallBack;

	Signal(const CallBack& callback = CallBack())
		: BaseSignal(callback) {
	}
};

/**
 * @brief      Creates a connectable object from a class method.
 *
 * @param      object      The Class object.
 * @param[in]  method      The Class method.
 *
 * @tparam     Instance    An instance of the class.
 * @tparam     Class       The class itsef.
 * @tparam     ReturnType  Method return value type.
 * @tparam     ArgTypes    Method argument types.
 *
 * @return     Connectable object.
 */
template <typename Instance, typename Class, typename ReturnType, class... ArgTypes>
std::function<ReturnType (ArgTypes...)> Slot(Instance& object, ReturnType (Class::*method)(ArgTypes...)) {
	auto slot = [&object, method](ArgTypes... args) {
		return (object.*method)(args...);
	};
	return slot;
}

/**
 * @brief      Creates a connectable object from a class method.
 *
 * @param      object      The Class object.
 * @param[in]  method      The Class method.
 *
 * @tparam     Class       The class itself.
 * @tparam     ReturnType  Method return value type.
 * @tparam     ArgTypes    Method argument types.
 *
 * @return     Connectable object.
 */
template <typename Class, typename ReturnType, class... ArgTypes>
std::function<ReturnType (ArgTypes...)> Slot(Class* object, ReturnType (Class::*method)(ArgTypes...)) {
	auto slot = [object, method](ArgTypes... args) {
		return (object->*method)(args...);
	};
	return slot;
}

} // namespace signal

#endif // SIGNAL_HPP
