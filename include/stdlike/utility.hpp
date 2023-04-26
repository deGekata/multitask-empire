#ifndef STDLIKE_UTILITY_HPP
#define STDLIKE_UTILITY_HPP

namespace stdlike {

template <typename Type>
struct RemoveReference {
    using type = Type;
};

template <typename Type>
struct RemoveReference<Type&> {
    using type = Type;
};

template <typename Type>
struct RemoveReference<Type&&> {
    using type = Type;
};

template <typename Type>
constexpr typename RemoveReference<Type>::type&& Move(Type&& arg) noexcept {
    return static_cast<typename RemoveReference<Type>::type&&>(arg);
}

template <typename Type>
constexpr Type&& Forward(typename RemoveReference<Type>::type& arg) noexcept {
    return static_cast<Type&&>(arg);
}

template <typename Type>
constexpr Type&& Forward(typename RemoveReference<Type>::type&& arg) noexcept {
    return static_cast<Type&&>(arg);
}

template <typename Type>
void Swap(Type& lhs, Type& rhs) noexcept {
    Type temp = Move(lhs);
    lhs = Move(rhs);
    rhs = Move(temp);
}

} // namespace stdlike

#endif // STDLIKE_UTILITY_HPP
