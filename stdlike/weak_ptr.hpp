#ifndef STDLIKE_WEAK_PTR_HPP
#define STDLIKE_WEAK_PTR_HPP

#include <stdlike/utility.hpp>
#include <stdlike/control_block.hpp>

namespace stdlike {

template <typename Any> class SharedPtr;

template <typename PtrType>
class WeakPtr {
	template <typename Any> friend class SharedPtr;

public:
	WeakPtr() {
	}

	WeakPtr(const SharedPtr<PtrType>& shared)
		: control_block_(shared.control_block_) {
		if (control_block_) {
			control_block_->weak_count++;
		}
	}

	WeakPtr(const WeakPtr& other)
		: control_block_(other.control_block_) {
		if (control_block_ != nullptr) {
			control_block_->weak_count++;
		}
	}

	WeakPtr(WeakPtr&& other)
		: control_block_(other.control_block_) {
		other.control_block_ = nullptr;
	}

	WeakPtr& operator=(const WeakPtr& other) {
		WeakPtr temp{other};
		Swap(temp);
		return *this;
	}

	WeakPtr& operator=(WeakPtr&& other) {
		Swap(other);
		other.Reset();
		return *this;
	}

	~WeakPtr() {
		DecreaseCounter();
	}

	void Reset() {
		DecreaseCounter();
		control_block_ = nullptr;
	}

	void Swap(WeakPtr& other) {
		Swap(control_block_, other.control_block_);
	}

	size_t UseCount() {
		if (control_block_ == nullptr) {
			return 0;
		}
		return control_block_->weak_count;
	}

	bool Expired() {
		return UseCount() == 0;
	}

	SharedPtr<PtrType> Lock() {
		return Expired() ? SharedPtr<PtrType>() : SharedPtr<PtrType>(*this);
	}

private:
	void DecreaseCounter() {
		if (control_block_ == nullptr) {
			return;
		}
		control_block_->weak_count--;

		if (control_block_->weak_count == 0 && control_block_->shared_count == 0) {
			delete control_block_;
			control_block_ = nullptr;
		}
	}

private:
	ControlBlock<PtrType>* control_block_{nullptr};
};

} // namespace stdlike

#endif // STDLIKE_WEAK_PTR_HPP
