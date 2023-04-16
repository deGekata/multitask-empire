#ifndef STDLIKE_CONTROL_BLOCK_HPP
#define STDLIKE_CONTROL_BLOCK_HPP

#include <cassert>
#include <cstddef>

namespace stdlike {

template <typename DataType>
struct ControlBlock {
    DataType* data{nullptr};
    size_t shared_count{1};
    size_t weak_count{0};
};

} // namespace stdlike

#endif // STDLIKE_CONTROL_BLOCK_HPP
