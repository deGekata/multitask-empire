#ifndef STDLIKE_SHARED_PTR_HPP
#define STDLIKE_SHARED_PTR_HPP

#include <cassert>

#include <stdlike/utility.hpp>
#include <stdlike/control_block.hpp>

namespace stdlike {

template <typename Any> class WeakPtr;

template <typename PtrType>
class SharedPtr {
private:
	template <typename Any> friend class WeakPtr;

public:
	SharedPtr() {
	}

	SharedPtr(PtrType* pointer)
		: control_block_(new ControlBlock<PtrType>(pointer)) {
	}

	SharedPtr(const WeakPtr<PtrType>& weak)
		: control_block_(weak.control_block_) {
		if (control_block_) {
			control_block_->shared_count++;
		}
	}

	SharedPtr(const SharedPtr& other)
		: control_block_(other.control_block_) {
		if (control_block_) {
			control_block_->shared_count++;
		}
	}

	SharedPtr(SharedPtr&& other)
		: control_block_(other.control_block_) {
		other.control_block_ = nullptr;
	}

	SharedPtr& operator=(const SharedPtr& other) {
		SharedPtr temp{other};
		Swap(temp);
		return *this;
	}

	SharedPtr& operator=(SharedPtr&& other) {
		Swap(other);
		other.Reset();
		return *this;
	}

	~SharedPtr() {
		DecreaseCounter();
	}

	PtrType* Get() {
		return control_block_->data;
	}

	PtrType& operator*() const {
		assert(control_block_);
		return *control_block_->data;
	}

	PtrType* operator->() const {
		assert(control_block_);
		return control_block_->data;
	}

	size_t UseCount() const {
		if (control_block_ == nullptr) {
			return 0;
		}
		return *control_block_->shared_count;
	}

	void Reset() {
		DecreaseCounter();
		control_block_ = nullptr;
	}

	void Reset(PtrType* new_pointer) {
		assert(new_pointer);
		DecreaseCounter();
		control_block_ = new ControlBlock<PtrType>(new_pointer);
	}

	void Swap(SharedPtr& other) {
		Swap(control_block_, other.control_block_);
	}

private:
	void DecreaseCounter() {
		if (control_block_ == nullptr) {
			return;
		}
		control_block_->shared_count--;
		
		if (control_block_->shared_count == 0) {
			delete control_block_->data;
			control_block_->data = nullptr;

			if (control_block_->weak_count == 0) {
				delete control_block_;
				control_block_ = nullptr;
			}
		}
	}

private:
	ControlBlock<PtrType>* control_block_{nullptr};
};

template <typename PtrType, typename... ArgTypes>
SharedPtr<PtrType> MakeShared(ArgTypes&&... args) {
	auto pointer = new PtrType(Forward<ArgTypes>(args)...);
	return SharedPtr<PtrType>(pointer);
}

} // namespace stdlike

#endif // STDLIKE_SHARED_PTR_HPP
