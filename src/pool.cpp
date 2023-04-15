#include <include/pool.hpp>

ecs::BasePool::BasePool(const uint64_t element_size, const uint64_t chunk_size)
    : element_size_(element_size), chunk_size_(chunk_size) {
}

uint64_t ecs::BasePool::GetSize() {
    return size_;
}

uint64_t ecs::BasePool::GetCapacity() {
    return capacity_;
}

uint64_t ecs::BasePool::GetChunksAmount() {
    return blocks_.size();
}

void ecs::BasePool::Resize(const uint64_t n) {
    if (n < size_) {
        return;
    }

    if (n >= capacity_) {
        Reserve(n);
    }

    size_ = n;
}

void ecs::BasePool::Reserve(const uint64_t n) {
    while (capacity_ < n) {
        char* new_chunk = new char[element_size_ * chunk_size_]();
        blocks_.push_back(new_chunk);
        capacity_ += chunk_size_;
    }
}

void* ecs::BasePool::GetElement(const uint64_t idx) {
    assert(idx < size_);

    return blocks_[idx / chunk_size_] + (idx % chunk_size_) * element_size_;
}

const void* ecs::BasePool::GetElement(const uint64_t idx) const {
    assert(idx < size_);

    return blocks_[idx / chunk_size_] + (idx % chunk_size_) * element_size_;
}
