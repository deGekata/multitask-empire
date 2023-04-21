#ifndef UTILITY_NON_COPIABLE
#define UTILITY_NON_COPIABLE

namespace utility {

class NonCopiable {
protected:
    NonCopiable() = default;

    NonCopiable(const NonCopiable& other) = delete;
    NonCopiable& operator=(const NonCopiable& other) = delete;
};

} // namespace utility

#endif // UTILITY_NON_COPIABLE
