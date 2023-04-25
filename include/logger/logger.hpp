#ifndef LOGGER_H
#define LOGGER_H

#include <fmt/color.h>
#include <fmt/format.h>

#include <string>
#include <typeinfo>

// https://stackoverflow.com/questions/281818/unmangling-the-result-of-stdtype-infoname
std::string demangle(const char* name);

template <class T>
std::string type(const T& t) {

    return demangle(typeid(t).name());
}

enum LOG_MODE {
    INFO,
    WARNING,
    ERROR
};


template<LOG_MODE mode, typename... Args>
void log(fmt::string_view format, const Args&... args) {
    fmt::v8::text_style info_style;

#ifdef LOG_ALL
    if(mode == INFO) {
        info_style = fmt::fg(fmt::detail::color_type(fmt::rgb(0x33ffcc)));    
    }
    else if(mode == WARNING) {
        info_style = fmt::fg(fmt::detail::color_type(fmt::rgb(0x6600ff)));   
    }
    else if(mode == ERROR) {
        info_style = fmt::fg(fmt::detail::color_type(fmt::rgb(0xc41e3a)));
        fmt::print(info_style, format, args...);
        exit(0);
    }
#else
    info_style = fmt::fg(fmt::detail::color_type(fmt::rgb(0xc41e3a)));
        fmt::print(info_style, format, args...);
        exit(0);
#endif // LOG_ALL

    fmt::print(info_style, format, args...);
}

template<typename... Args>
void log(fmt::v8::text_style info_style, fmt::string_view format, const Args&... args) {
    fmt::print(info_style, format, args...);
}

#endif // LOGGER_H
