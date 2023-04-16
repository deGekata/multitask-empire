#ifndef STDLIKE_VECTOR_HPP
#define STDLIKE_VECTOR_HPP

#include <cassert>

#include <stdlike/utility.hpp>
#include <stdlike/allocator.hpp>

namespace stdlike {

template <typename Type, class Alloc = stdlike::Allocator<Type>>
class Vector {
public:
    /* Iterator */

    class Iterator : public std::iterator<std::contiguous_iterator_tag, Type> {
    public:
        Iterator() : ptr_(nullptr), container_(nullptr) {
        }

        Iterator(Type* ptr, Vector* container = nullptr) : ptr_(ptr), container_(container) {
        }

        Iterator(const Iterator& other) : ptr_(other.ptr_), container_(other.container_) {
        }

        ~Iterator() {
            ptr_ = nullptr;
            container_ = nullptr;
        }

        Iterator& operator=(const Iterator& other) {
            ptr_ = other.ptr_;
            container_ = other.container_;
            return *this;
        }

        Type& operator*() const {
            assert(ptr_ && container_ && ptr_ < container_->End().ptr_);
            return *ptr_;
        }

        Type* operator->() const {
            assert(ptr_ && container_ && ptr_ < container_->End().ptr_);
            return ptr_;
        }

        Iterator& operator++() {
            ++ptr_;
            return *this;
        }

        Iterator operator++(int) {
            Iterator temp = *this;
            ++ptr_;
            return temp;
        }

        Iterator& operator--() {
            --ptr_;
            return *this;
        }

        Iterator operator--(int) {
            Iterator temp = *this;
            --ptr_;
            return temp;
        }

        Iterator& operator+=(ptrdiff_t diff) {
            if (diff >= 0) {
                while (diff--) {
                    ++(*this);
                }
            } else {
                while (diff++) {
                    --(*this);
                }
            }
            return *this;
        }

        Iterator& operator-=(ptrdiff_t diff) {
            return *this += -diff;
        }

        Type& operator[](ptrdiff_t diff) const {
            return ptr_[diff];
        }

        friend Iterator operator+(const Iterator& iter, ptrdiff_t diff) {
            Iterator temp = iter;
            return temp += diff;
        }

        friend Iterator operator+(ptrdiff_t diff, const Iterator& iter) {
            return iter + diff;
        }

        friend Iterator operator-(const Iterator& iter, ptrdiff_t diff) {
            Iterator temp = iter;
            return temp -= diff;
        }

        friend ptrdiff_t operator-(const Iterator& lhs, const Iterator& rhs) {
            return lhs.ptr_ - rhs.ptr_;
        }

        friend bool operator==(const Iterator& lhs, const Iterator& rhs) {
            return lhs.ptr_ == rhs.ptr_;
        }

        friend bool operator!=(const Iterator& lhs, const Iterator& rhs) {
            return !(lhs == rhs);
        }

        friend bool operator<(const Iterator& lhs, const Iterator& rhs) {
            return lhs.ptr_ < rhs.ptr_;
        }

        friend bool operator>(const Iterator& lhs, const Iterator& rhs) {
            return rhs < lhs;
        }

        friend bool operator<=(const Iterator& lhs, const Iterator& rhs) {
            return !(lhs > rhs);
        }

        friend bool operator>=(const Iterator& lhs, const Iterator& rhs) {
            return !(lhs < rhs);
        }

    private:
        Type* ptr_ = nullptr;
        Vector* container_ = nullptr;
    };

    Iterator Begin() {
        return Iterator(data_, this);
    }

    Iterator End() {
        return Iterator(data_ + size_, this);
    }

    Iterator begin() {
        return Begin();
    }

    Iterator end() {
        return End();
    }

    /* ConstIterator */

    class ConstIterator : public std::iterator<std::contiguous_iterator_tag, Type> { // ! template
    public:
        ConstIterator() : ptr_(nullptr), container_(nullptr) {
        }

        ConstIterator(Type* ptr, const Vector* container) : ptr_(ptr), container_(container) {
        }

        ConstIterator(const ConstIterator& other) : ptr_(other.ptr_), container_(other.container) {
        }

        ~ConstIterator() {
            ptr_ = nullptr;
            container_ = nullptr;
        }

        ConstIterator& operator=(const ConstIterator& other) {
            ptr_ = other.ptr_;
            container_ = other.container_;
        }

        const Type& operator*() const {
            assert(ptr_ && container_ && ptr_ < End().ptr_);
            return *ptr_;
        }

        const Type* operator->() const {
            assert(ptr_ && container_ && ptr_ < End().ptr_);
            return ptr_;
        }

        ConstIterator& operator++() {
            ++ptr_;
            return *this;
        }

        ConstIterator operator++(int) {
            ConstIterator temp = *this;
            ++ptr_;
            return temp;
        }

        ConstIterator& operator--() {
            --ptr_;
            return *this;
        }

        ConstIterator operator--(int) {
            ConstIterator temp = *this;
            --ptr_;
            return temp;
        }

        ConstIterator& operator+=(ptrdiff_t diff) {
            if (diff >= 0) {
                while (diff--) {
                    ++(*this);
                }
            } else {
                while (diff++) {
                    --(*this);
                }
            }
        }

        ConstIterator& operator-=(ptrdiff_t diff) {
            return *this += -diff;
        }

        const Type& operator[](ptrdiff_t diff) const {
            return ptr_[diff];
        }

        friend ConstIterator operator+(const ConstIterator& iter, ptrdiff_t diff) {
            ConstIterator temp = iter;
            return temp += diff;
        }

        friend ConstIterator operator+(ptrdiff_t diff, const ConstIterator& iter) {
            return iter + diff;
        }

        friend ConstIterator operator-(const ConstIterator& iter, ptrdiff_t diff) {
            ConstIterator temp = iter;
            return temp -= diff;
        }

        friend ptrdiff_t operator-(const ConstIterator& lhs, const ConstIterator& rhs) {
            return lhs.ptr_ - rhs.ptr_;
        }

        friend bool operator==(const ConstIterator& lhs, const ConstIterator& rhs) {
            return lhs.ptr_ == rhs.ptr_;
        }

        friend bool operator!=(const ConstIterator& lhs, const ConstIterator& rhs) {
            return !(lhs == rhs);
        }

        friend bool operator<(const ConstIterator& lhs, const ConstIterator& rhs) {
            return lhs.ptr_ < rhs.ptr_;
        }

        friend bool operator>(const ConstIterator& lhs, const ConstIterator& rhs) {
            return rhs < lhs;
        }

        friend bool operator<=(const ConstIterator& lhs, const ConstIterator& rhs) {
            return !(lhs > rhs);
        }

        friend bool operator>=(const ConstIterator& lhs, const ConstIterator& rhs) {
            return !(lhs < rhs);
        }

    private:
        const Type* ptr_ = nullptr;
        const Vector* container_ = nullptr;
    };

    ConstIterator Begin() const {
        return ConstIterator(data_, this);
    }

    ConstIterator End() const {
        return ConstIterator(data_ + size_, this);
    }

    ConstIterator begin() const {
        return Begin();
    }

    ConstIterator end() const {
        return End();
    }

    /*
     * For ReverseIterator and ConstReverseIterator
     * use std::make_reverse_iterator()
     */

public:
    /* Vector<Type> */

    Vector() : size_(0), capacity_(0), data_(nullptr) {
    }

    explicit Vector(size_t init_size, const Type& value = Type())
        : allocator_(Alloc()), size_(init_size), capacity_(init_size), data_(allocator_.allocate(capacity_)) {

        this->Initialize(data_, 0, size_, value);
    }

    Vector(const Vector& other)
        : allocator_(Alloc())
        , size_(other.Size())
        , capacity_(other.Capacity())
        , data_(allocator_.allocate(other.Capacity())) {

        this->Copy(data_, 0, other.Size(), other.Data());
    }

    Vector(Vector&& temp) {
        *this = std::move(temp);
    }

    ~Vector() {
        this->Release(data_, 0, size_);
        allocator_.deallocate(data_, capacity_);
        size_ = 0;
        capacity_ = 0;
        data_ = nullptr;
    }

    Vector& operator=(const Vector& other) {
        this->~Vector();
        size_ = other.Size();
        capacity_ = other.Capacity();
        data_ = allocator_.allocate(other.Capacity());

        this->Copy(data_, 0, other.Size(), other.Data());

        return *this;
    }

    Vector& operator=(Vector&& temp) {
        std::swap(temp.size_, size_);
        std::swap(temp.capacity_, capacity_);
        std::swap(temp.data_, data_);

        return *this;
    }

    bool operator==(const Vector&) const = delete;

    /* Capacity */

    bool Empty() const {
        return size_ == 0;
    }

    size_t Size() const {
        return size_;
    }

    size_t Capacity() const {
        return capacity_;
    }

    void Reserve(size_t new_capacity) {
        if (new_capacity > capacity_) {
            this->ChangeCapacity(new_capacity);
        }
    }

    void ShrinkToFit() {
        if (capacity_ > size_) {
            this->ChangeCapacity(size_);
        }
    }

    /* Element access */

    const Type& At(size_t pos) const {
        Type& ret = const_cast<Vector*>(this)->At(pos);
        return const_cast<const Type&>(ret);
    }

    Type& At(size_t pos) {
        assert(pos < size_);
        return data_[pos];
    }

    const Type& operator[](size_t pos) const {
        Type& ret = const_cast<Vector*>(this)->operator[](pos);
        return const_cast<const Type&>(ret);
    }

    Type& operator[](size_t pos) {
        return data_[pos];
    }

    const Type& Front() const {
        Type& ret = const_cast<Vector*>(this)->Front();
        return const_cast<const Type&>(ret);
    }

    Type& Front() {
        return data_[0];
    }

    const Type& Back() const {
        Type& ret = const_cast<Vector*>(this)->Back();
        return const_cast<const Type&>(ret);
    }

    Type& Back() {
        return data_[size_ - 1];
    }

    const Type* Data() const {
        Type* ret = const_cast<Vector*>(this)->Data();
        return const_cast<const Type*>(ret);
    }

    Type* Data() {
        return data_;
    }

    /* Modifiers */

    void Clear() {
        this->Release(data_, 0, size_);
        size_ = 0;
    }

    Iterator Insert(Iterator pos, const Type& value) {
        ptrdiff_t offset = pos - Begin();
        /* Invalidates Iterators */
        if (size_ >= capacity_) {
            this->Reserve(size_ ? size_ * 2 : 1);
        }

        pos = Begin() + offset;
        allocator_.construct(data_ + size_++, value);
        for (Iterator it = End() - 1; it > pos; it--) {
            std::swap(*it, *(it - 1));
        }

        *pos = value;
        return pos;
    }

    Iterator Erase(Iterator pos) {
        if (pos >= End()) {
            return End();
        }

        for (Iterator it = pos; it < End() - 1; it++) {
            std::swap(*it, *(it + 1));
        }

        allocator_.destroy(data_ + --size_);
        return pos;
    }

    void PushBack(const Type& value) {
        this->Insert(End(), value);
    }

    void PopBack() {
        if (size_ > 0) {
            this->Erase(End() - 1);
        }
    }

    void Resize(size_t new_size, const Type& value = Type()) {
        if (size_ >= new_size) {
            this->Release(data_, new_size, size_);
        } else {
            this->Reserve(new_size);
            this->Initialize(data_, size_, new_size, value);
            size_ = new_size;
        }
    }

    void Swap(Vector& other) {
        Vector temp = stdlike::Move(other);
        other = stdlike::Move(*this);
        *this = stdlike::Move(temp);
    }

private:
    /* Helper functions */

    void ChangeCapacity(size_t new_capacity) {
        Type* new_data = allocator_.allocate(new_capacity);
        size_t new_size = this->Copy(new_data, 0, std::min(size_, new_capacity), data_);
        this->~Vector();

        size_ = new_size;
        capacity_ = new_capacity;
        data_ = new_data;
    }

    inline size_t Release(Type* data, size_t start, size_t end) {
        if (start == end) {
            return 0;
        }

        assert(data);
        for (size_t i = start; i < end; i++) {
            allocator_.destroy(data + i);
        }

        return end - start;
    }

    inline size_t Initialize(Type* data, size_t start, size_t end, const Type& value) {
        if (start == end) {
            return 0;
        }

        assert(data);
        for (size_t cur_offset = start; cur_offset < end; cur_offset++) {
            allocator_.construct(data + cur_offset, value);
        }

        return end - start;
    }

    inline size_t Copy(Type* dest, size_t start, size_t end, const Type* src) {
        if (start == end) {
            return 0;
        }

        assert(dest && src);
        for (size_t cur_offset = start; cur_offset < end; cur_offset++) {
            allocator_.construct(dest + cur_offset, src[cur_offset]);
        }

        return end - start;
    }

private:
    Alloc allocator_ = {};
    size_t size_ = 0;
    size_t capacity_ = 0;
    Type* data_ = nullptr;
};

template <>
class Vector<bool, Allocator<bool>> {
public:
    /* BitReference */

    class BitReference {
    public:
        BitReference() : source_(nullptr), mask_(0) {
        }

        BitReference(uint32_t* source, uint32_t mask) : source_(source), mask_(mask) {
        }

        BitReference(const BitReference& other) : source_(other.source_), mask_(other.mask_) {
        }

        ~BitReference() {
            source_ = nullptr;
            mask_ = 0;
        }

        BitReference& operator=(bool value) {
            assert(source_);
            if (value) {
                *source_ |= mask_;
            } else {
                *source_ &= ~mask_;
            }
            return *this;
        }

        BitReference& operator=(const BitReference& other) {
            return *this = bool(other);
        }

        operator bool() const {
            assert(source_);
            return mask_ & *source_;
        }

        friend void swap(BitReference lhs, BitReference rhs) {
            bool temp = lhs;
            lhs = rhs;
            rhs = temp;
        }

        friend bool operator==(const BitReference& lhs, const BitReference& rhs) {
            return bool(lhs) == bool(rhs);
        }

        friend bool operator!=(const BitReference& lhs, const BitReference& rhs) {
            return !(lhs == rhs);
        }

        friend bool operator<(const BitReference& lhs, const BitReference& rhs) {
            return !bool(lhs) && bool(rhs);
        }

        friend bool operator>(const BitReference& lhs, const BitReference& rhs) {
            return rhs < lhs;
        }

        friend bool operator<=(const BitReference& lhs, const BitReference& rhs) {
            return !(lhs > rhs);
        }

        friend bool operator>=(const BitReference& lhs, const BitReference& rhs) {
            return !(lhs < rhs);
        }

    private:
        uint32_t* source_ = nullptr;
        uint32_t mask_ = 0;
    };

public:
    /* Iterator */

    class Iterator {
    public:
        using iterator_category = std::random_access_iterator_tag;
        using difference_type = ptrdiff_t;
        using iterator = Iterator;

        using value_type = bool;
        using pointer = BitReference*;
        using reference = BitReference;

        Iterator() : data_(nullptr), shift_(0), container_(nullptr) {
        }

        Iterator(uint32_t* data, uint32_t shift, Vector* container = nullptr)
            : data_(data), shift_(shift), container_(container) {
        }

        Iterator(const Iterator& other) : data_(other.data_), shift_(other.shift_), container_(other.container_) {
        }

        ~Iterator() {
            data_ = nullptr;
            shift_ = 0;
            container_ = nullptr;
        }

        Iterator& operator=(const Iterator& other) {
            data_ = other.data_;
            shift_ = other.shift_;
            container_ = other.container_;
            return *this;
        }

        reference operator*() const {
            assert(data_ && container_);
            assert((data_ < container_->End().data_) ||
                   (data_ == container_->End().data_ && shift_ > container_->End().shift_));
            return reference(data_, 1u << shift_);
        }

        Iterator& operator++() {
            if (shift_-- == 0) {
                shift_ = 31;
                data_++;
            }
            return *this;
        }

        Iterator operator++(int) {
            Iterator old = *this;
            ++(*this);
            return old;
        }

        Iterator& operator--() {
            if (shift_++ == 31) {
                shift_ = 0;
                data_--;
            }
            return *this;
        }

        Iterator operator--(int) {
            Iterator old = *this;
            --(*this);
            return old;
        }

        Iterator& operator+=(difference_type diff) {
            if (diff > 0) {
                while (diff--) {
                    ++(*this);
                }
            } else {
                while (diff++) {
                    --(*this);
                }
            }
            return *this;
        }

        Iterator& operator-=(difference_type diff) {
            return *this += -diff;
        }

        reference operator[](difference_type diff) const {
            return *(*this + diff);
        }

        friend difference_type operator-(const Iterator& lhs, const Iterator& rhs) {
            return (32 * (lhs.data_ - rhs.data_) + rhs.shift_ - lhs.shift_);
        }

        friend Iterator operator+(const Iterator& iter, difference_type diff) {
            Iterator temp = iter;
            return temp += diff;
        }

        friend Iterator operator+(difference_type diff, const Iterator& iter) {
            return iter + diff;
        }

        friend Iterator operator-(const Iterator& iter, difference_type diff) {
            Iterator temp = iter;
            return temp -= diff;
        }

        friend bool operator==(const Iterator& lhs, const Iterator& rhs) {
            return (lhs.data_ == rhs.data_ && lhs.shift_ == rhs.shift_);
        }

        friend bool operator!=(const Iterator& lhs, const Iterator& rhs) {
            return !(lhs == rhs);
        }

        friend bool operator<(const Iterator& lhs, const Iterator& rhs) {
            return (lhs.data_ < rhs.data_) || (lhs.data_ == rhs.data_ && lhs.shift_ > rhs.shift_);
        }

        friend bool operator>(const Iterator& lhs, const Iterator& rhs) {
            return (rhs < lhs);
        }

        friend bool operator<=(const Iterator& lhs, const Iterator& rhs) {
            return !(lhs > rhs);
        }

        friend bool operator>=(const Iterator& lhs, const Iterator& rhs) {
            return !(lhs < rhs);
        }

    private:
        uint32_t* data_ = nullptr;
        uint32_t shift_ = 0;
        Vector* container_ = nullptr;
    };

    Iterator Begin() {
        return Iterator(data_, 31, this); // ! magic const
    }

    Iterator End() {
        return Iterator(data_ + DivideByThirtyTwo(size_), 31 - ThirtyTwoModulo(size_), this);
    }

    Iterator begin() {
        return Begin();
    }

    Iterator end() {
        return End();
    }

    /* ConstIterator */

    class ConstIterator {
    public:
        using iterator_category = std::random_access_iterator_tag;
        using difference_type = ptrdiff_t;
        using const_iterator = ConstIterator;

        using value_type = bool;
        using pointer = BitReference*;
        using reference = BitReference;

        ConstIterator() : data_(nullptr), shift_(0), container_(nullptr) {
        }

        ConstIterator(uint32_t* data, uint32_t shift, const Vector* container = nullptr)
            : data_(data), shift_(shift), container_(container) {
        }

        ConstIterator(const ConstIterator& other)
            : data_(other.data_), shift_(other.shift_), container_(other.container_) {
        }

        ~ConstIterator() {
            data_ = nullptr;
            shift_ = 0;
            container_ = nullptr;
        }

        ConstIterator& operator=(const ConstIterator& other) {
            data_ = other.data_;
            shift_ = other.shift_;
            container_ = other.container_;
            return *this;
        }

        const reference operator*() const {
            assert(data_ && container_);
            assert((data_ < container_->End().data_) ||
                   (data_ == container_->End().data_ && shift_ > container_->End().shift_));
            return reference(data_, 1u << shift_);
        }

        ConstIterator& operator++() {
            if (shift_-- == 0) {
                shift_ = 31;
                data_++;
            }
            return *this;
        }

        ConstIterator operator++(int) {
            ConstIterator old = *this;
            ++(*this);
            return old;
        }

        ConstIterator& operator--() {
            if (shift_++ == 31) {
                shift_ = 0;
                data_--;
            }
            return *this;
        }

        ConstIterator operator--(int) {
            ConstIterator old = *this;
            --(*this);
            return old;
        }

        ConstIterator& operator+=(difference_type diff) {
            if (diff > 0) {
                while (diff--) {
                    ++(*this);
                }
            } else {
                while (diff++) {
                    --(*this);
                }
            }
            return *this;
        }

        ConstIterator& operator-=(difference_type diff) {
            return *this += -diff;
        }

        const reference operator[](difference_type diff) const {
            return *(*this + diff);
        }

        friend difference_type operator-(const ConstIterator& lhs, const ConstIterator& rhs) {
            return (32 * (lhs.data_ - rhs.data_) + rhs.shift_ - lhs.shift_);
        }

        friend ConstIterator operator+(const ConstIterator& iter, difference_type diff) {
            ConstIterator temp = iter;
            return temp += diff;
        }

        friend ConstIterator operator+(difference_type diff, const ConstIterator& iter) {
            return iter + diff;
        }

        friend ConstIterator operator-(const ConstIterator& iter, difference_type diff) {
            ConstIterator temp = iter;
            return temp -= diff;
        }

        friend bool operator==(const ConstIterator& lhs, const ConstIterator& rhs) {
            return (lhs.data_ == rhs.data_ && lhs.shift_ == rhs.shift_);
        }

        friend bool operator!=(const ConstIterator& lhs, const ConstIterator& rhs) {
            return !(lhs == rhs);
        }

        friend bool operator<(const ConstIterator& lhs, const ConstIterator& rhs) {
            return (lhs.data_ < rhs.data_) || (lhs.data_ == rhs.data_ && lhs.shift_ > rhs.shift_);
        }

        friend bool operator>(const ConstIterator& lhs, const ConstIterator& rhs) {
            return (rhs < lhs);
        }

        friend bool operator<=(const ConstIterator& lhs, const ConstIterator& rhs) {
            return !(lhs > rhs);
        }

        friend bool operator>=(const ConstIterator& lhs, const ConstIterator& rhs) {
            return !(lhs < rhs);
        }

    private:
        uint32_t* data_ = nullptr;
        uint32_t shift_ = 0;
        const Vector* container_ = nullptr;
    };

    ConstIterator Begin() const {
        return ConstIterator(data_, 31, this);
    }

    ConstIterator End() const {
        return ConstIterator(data_ + DivideByThirtyTwo(size_), 31 - ThirtyTwoModulo(size_), this);
    }

    ConstIterator begin() const {
        return Begin();
    }

    ConstIterator end() const {
        return End();
    }

    /*
     * For ReverseIterator and ConstReverseIterator
     * use std::make_reverse_iterator()
     */

public:
    /* Vector<bool> */

    Vector() : size_(0), capacity_(0), data_(nullptr) {
    }

    explicit Vector(size_t init_size, bool value = false)
        : size_(init_size)
        , capacity_(RoundUpToThirtyTwoMultiple(init_size))
        , data_(new uint32_t[BitsToBytes(capacity_)]()) {

        this->Initialize(data_, 0, size_, value);
    }

    Vector(const Vector& other)
        : size_(other.Size()), capacity_(other.Capacity()), data_(new uint32_t[BitsToBytes(other.Capacity())]()) {

        this->Copy(data_, 0, other.Size(), other.Data());
    }

    Vector(Vector&& temp) {
        *this = std::move(temp);
    }

    ~Vector() {
        delete[] data_;
        size_ = 0;
        capacity_ = 0;
        data_ = nullptr;
    }

    Vector& operator=(const Vector& other) {
        this->~Vector();
        size_ = other.Size();
        capacity_ = other.Capacity();
        data_ = new uint32_t[BitsToBytes(other.Capacity())]();

        this->Copy(data_, 0, other.Size(), other.Data());

        return *this;
    }

    Vector& operator=(Vector&& temp) {
        std::swap(temp.size_, size_);
        std::swap(temp.capacity_, capacity_);
        std::swap(temp.data_, data_);

        return *this;
    }

    bool operator==(const Vector&) const = delete;

    /* Capacity */

    bool Empty() const {
        return size_ == 0;
    }

    size_t Size() const {
        return size_;
    }

    size_t Capacity() const {
        return capacity_;
    }

    void Reserve(size_t new_capacity) {
        if (new_capacity > capacity_) {
            this->ChangeCapacity(RoundUpToThirtyTwoMultiple(new_capacity));
        }
    }

    void ShrinkToFit() {
        if (capacity_ > size_) {
            this->ChangeCapacity(RoundUpToThirtyTwoMultiple(size_));
        }
    }

    /* Element access */

    const BitReference At(size_t pos) const {
        BitReference ret = const_cast<Vector*>(this)->At(pos);
        return static_cast<const BitReference>(ret);
    }

    BitReference At(size_t pos) {
        assert(pos < size_);
        return BitReference(data_ + DivideByThirtyTwo(pos), 1u << (31u - ThirtyTwoModulo(pos)));
    }

    const BitReference operator[](size_t pos) const {
        BitReference ret = const_cast<Vector*>(this)->operator[](pos);
        return static_cast<const BitReference>(ret);
    }

    BitReference operator[](size_t pos) {
        return BitReference(data_ + DivideByThirtyTwo(pos), 1u << (31u - ThirtyTwoModulo(pos)));
    }

    const BitReference Front() const {
        BitReference ret = const_cast<Vector*>(this)->Front();
        return static_cast<const BitReference>(ret);
    }

    BitReference Front() {
        return this->operator[](0);
    }

    const BitReference Back() const {
        BitReference ret = const_cast<Vector*>(this)->Back();
        return static_cast<const BitReference>(ret);
    }

    BitReference Back() {
        return this->operator[](size_ - 1);
    }

    const uint32_t* Data() const {
        uint32_t* ret = const_cast<Vector*>(this)->Data();
        return const_cast<const uint32_t*>(ret);
    }

    uint32_t* Data() {
        return data_;
    }

    /* Modifiers */

    void Clear() {
        size_ = 0;
    }

    Iterator Insert(Iterator pos, bool value) {
        ptrdiff_t offset = pos - Begin();
        if (size_ >= capacity_) { /* Invalidates Iterators */
            this->Reserve(size_ ? size_ * 2 : 1);
        }

        pos = Begin() + offset;
        size_++;
        for (Iterator it = End() - 1; it > pos; it--) {
            *it = stdlike::Move(*(it - 1));
        }

        *pos = value;
        return pos;
    }

    Iterator Erase(Iterator pos) {
        if (pos >= End()) {
            return End();
        }

        for (Iterator it = pos; it < End() - 1; it++) {
            *it = stdlike::Move(*(it + 1));
        }

        size_--;
        return pos;
    }

    void PushBack(bool value) {
        this->Insert(End(), value);
    }

    void PopBack() {
        if (size_ > 0) {
            this->Erase(End() - 1);
        }
    }

    void Resize(size_t new_size, bool value = false) {
        if (size_ < new_size) {
            this->Reserve(RoundUpToThirtyTwoMultiple(new_size));
            this->Initialize(data_, size_, new_size, value);
            size_ = new_size;
        }
    }

    void Swap(Vector& other) {
        Vector temp = stdlike::Move(other);
        other = stdlike::Move(*this);
        *this = stdlike::Move(temp);
    }

private:
    /* Helper functions */

    void ChangeCapacity(size_t new_capacity) {
        new_capacity = RoundUpToThirtyTwoMultiple(new_capacity);
        uint32_t* new_data = new uint32_t[BitsToBytes(new_capacity)];
        size_t new_size = this->Copy(new_data, 0, std::min(size_, new_capacity), data_);
        this->~Vector();

        size_ = new_size;
        capacity_ = new_capacity;
        data_ = new_data;
    }

    inline size_t Initialize(uint32_t* data, size_t start, size_t end, bool value) {
        assert(data);
        for (size_t cur_pos = start; cur_pos < end; cur_pos++) {
            SetValue(data, cur_pos, value);
        }

        return end - start;
    }

    inline size_t Copy(uint32_t* dest, size_t start, size_t end, const uint32_t* src) {
        assert(dest && src);
        for (size_t cur_pos = start; cur_pos < end; cur_pos++) {
            SetValue(dest, cur_pos, GetValue(src, cur_pos));
        }

        return end - start;
    }

    static inline bool GetValue(const uint32_t* data, size_t pos) {
        BitReference bit_ref(const_cast<uint32_t*>(data) + DivideByThirtyTwo(pos), 1u << (31u - ThirtyTwoModulo(pos)));
        return bool(bit_ref);
    }

    static inline void SetValue(uint32_t* data, size_t pos, bool value) {
        BitReference bit_ref(data + DivideByThirtyTwo(pos), 1u << (31u - ThirtyTwoModulo(pos)));
        bit_ref = value;
    }

    /* Utility functions */

    static inline uint64_t BitsToBytes(uint64_t bits) {
        return bits >> 3;
    }

    static inline uint32_t RoundUpToThirtyTwoMultiple(uint64_t num) {
        return (num + 31) & ~31u;
    }

    static inline uint32_t DivideByThirtyTwo(uint64_t num) {
        return static_cast<uint32_t>(num >> 5);
    }

    static inline uint32_t ThirtyTwoModulo(uint64_t num) {
        return num & 31;
    }

private:
    size_t size_ = 0;     /* in bits */
    size_t capacity_ = 0; /* in bits, always == 0 (mod 8), at least 8 */
    uint32_t* data_ = nullptr;
};

template <class Type>
std::ostream& operator<<(std::ostream& stream, const Vector<Type>& vec) {
    for (size_t i = 0; i < vec.Size(); i++) {
        stream << vec.At(i);
        if (i != vec.Size() - 1) {
            stream << " ";
        }
    }

    return stream;
}

}  // namespace stdlike

#endif  // STDLIKE_VECTOR_HPP
