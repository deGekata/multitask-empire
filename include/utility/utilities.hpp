#ifndef H_UTILITIES
#define H_UTILITIES

#include <cmath>

#include <limits>

template <typename T>
bool IsEqual(T lhs, T rhs) {
    return std::abs(rhs - lhs) < std::numeric_limits<T>::epsilon();
}

#endif
