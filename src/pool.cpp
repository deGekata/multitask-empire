#include "../include/pool.hpp"

namespace ecs {

BasePool::BasePool(const uint64_t element_size, const uint64_t chunk_size)
    : element_size_(element_size), chunk_size_(chunk_size) {
}

BasePool::~BasePool() {
    for (char* ptr : blocks_) {
        delete[] ptr;
    }
}

uint64_t BasePool::GetSize() {
    return size_;
}

uint64_t BasePool::GetCapacity() {
    return capacity_;
}

uint64_t BasePool::GetChunksAmount() {
    return blocks_.size();
}

void BasePool::Resize(const uint64_t n) {
    if (n < size_) {
        return;
    }

    if (n >= capacity_) {
        Reserve(n);
    }

    size_ = n;
}

void BasePool::Reserve(const uint64_t n) {
    while (capacity_ < n) {
        char* new_chunk = new char[element_size_ * chunk_size_]();
        blocks_.push_back(new_chunk);
        capacity_ += chunk_size_;
    }
}

void* BasePool::GetElement(const uint64_t idx) {
    assert(idx < size_);

    return blocks_[idx / chunk_size_] + (idx % chunk_size_) * element_size_;
}

const void* BasePool::GetElement(const uint64_t idx) const {
    assert(idx < size_);

    return blocks_[idx / chunk_size_] + (idx % chunk_size_) * element_size_;
}

};  // namespace ecs
