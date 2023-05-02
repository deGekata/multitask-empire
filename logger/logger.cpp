#include "logger/logger.hpp"

namespace logger
{
#ifdef __GNUG__
#include <cstdlib>
#include <memory>
#include <cxxabi.h>

std::string Demangle(const char* name) {

    int status = -4;

    std::unique_ptr<char, void(*)(void*)> res {
        abi::__cxa_demangle(name, nullptr, nullptr, &status),
        std::free
    };

    return (status==0) ? res.get() : name ;
}

#else

std::string Demangle(const char* name) {
    return name;
}

#endif
};
