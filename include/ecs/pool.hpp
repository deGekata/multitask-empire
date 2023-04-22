#ifndef H_POOL
#define H_POOL

#pragma once

#include <cassert>
#include <cstdint>

#include <vector>

namespace ecs {
class BasePool {
public:
    constexpr static uint64_t kDefaultChunkSize = 8192;

    explicit BasePool(const uint64_t element_size, const uint64_t chunk_size = kDefaultChunkSize);
    virtual ~BasePool();

    uint64_t GetSize();
    uint64_t GetCapacity();
    uint64_t GetChunksAmount();

    void Resize(const uint64_t n);
    void Reserve(const uint64_t n);

    void* GetElement(const uint64_t idx);
    const void* GetElement(const uint64_t idx) const;

    virtual void DestroyElement(const uint64_t idx) = 0;

protected:
    std::vector<char*> blocks_;

    uint64_t element_size_;
    uint64_t chunk_size_;
    uint64_t size_ = 0;
    uint64_t capacity_ = 0;
};

template <typename T, uint64_t ChunkSize = BasePool::kDefaultChunkSize>
class Pool : public BasePool {
public:
    Pool() : BasePool(sizeof(T), ChunkSize) {
    }

    virtual ~Pool() override {
    }

    virtual void DestroyElement(const uint64_t idx) override {
        assert(idx < size_);
        T* ptr = static_cast<T*>(GetElement(idx));
        ptr->~T();
    }
};

};  // namespace ecs

#endif
