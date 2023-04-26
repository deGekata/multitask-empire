#ifndef STDLIKE_UNIQUE_PTR_HPP
#define STDLIKE_UNIQUE_PTR_HPP

#include <cstddef>
#include <cassert>

#include <stdlike/utility.hpp>

namespace stdlike {

template <typename Type>
class UniquePtr {
public:
    UniquePtr() {
    }

    explicit UniquePtr(Type* pointer)
        : pointer_(pointer) {
    }

    ~UniquePtr() {
        delete pointer_;
        pointer_ = nullptr;
    }

    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    UniquePtr(UniquePtr&& other)
        : pointer_(other.pointer_) {
        other.pointer_ = nullptr;
    }

    UniquePtr& operator=(UniquePtr&& other) {
        Reset(other.pointer_);
        other.pointer_ = nullptr;
        return *this;
    }

    void Reset(Type* pointer = nullptr) {
        delete pointer_;
        pointer_ = pointer;
    }

    Type* Release() noexcept {
        Type* temp = pointer_;
        pointer_ = nullptr;
        return temp;
    }

    void Swap(UniquePtr& other) noexcept {
        stdlike::Swap(pointer_, other.pointer_);
    }

    Type* Get() const noexcept {
        return pointer_;
    }

    explicit operator bool() const noexcept {
        return pointer_ != nullptr;
    }

    Type& operator*() const noexcept {
        assert(pointer_ != nullptr);
        return *pointer_;
    }

    Type* operator->() const noexcept {
        assert(pointer_ != nullptr);
        return pointer_;
    }

private:
    Type* pointer_{nullptr};
};

} // namespace stdlike

#endif // STDLIKE_UNIQUE_PTR_HPP
