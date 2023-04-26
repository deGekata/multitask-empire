#ifndef LOGGER_H
#define LOGGER_H

#include <fmt/color.h>
#include <fmt/format.h>

#include <string>
#include <typeinfo>
#include <ctime>

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

template<typename... Args>
void debug_log(const char*, const char*, int, fmt::text_style info_style, fmt::string_view format, const Args&... args) {
    
    time_t rawtime = time(nullptr);
    struct tm *ptm = localtime(&rawtime);

    char time_str[20] = "";
    strftime(time_str, 16, "%H:%M:%S", ptm);

    fmt::print(fmt::bg(fmt::detail::color_type(fmt::rgb(0x4B2D9F))) | info_style, "[{}]", time_str);
    fmt::print(" ");

    fmt::print(info_style, format, args...);
}


template<typename... Args>
void debug_log(const char* file_name, const char* func_name, int n_line, LOG_MODE, fmt::string_view format, const Args&... args) {
    fmt::text_style info_style;

#ifdef LOG_ALL
    if(mode == WARNING) {
        info_style = fmt::fg(fmt::detail::color_type(fmt::rgb(0x6600ff))); // todo:  
    }
    else if(mode == ERROR) {
        info_style = fmt::fg(fmt::detail::color_type(fmt::rgb(0xc41e3a))); // todo:
        fmt::print(info_style, format, args...);
        exit(0);
    }
#else
    info_style = fmt::fg(fmt::detail::color_type(fmt::rgb(0xc41e3a)));
        fmt::print(info_style, format, args...);
        exit(0);
#endif // LOG_ALL

    debug_log(file_name, func_name, n_line, info_style, format, args...);
}

#define log(...) debug_log((__FILE__), (__FUNCTION__) , (__LINE__), __VA_ARGS__)

#endif // LOGGER_H
