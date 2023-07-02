#pragma once

#include <ulib/string.h>
#include <ulib/u8.h>

namespace ulib
{
    class RuntimeError : public std::runtime_error
    {
    public:
        RuntimeError(ulib::string_view view) : std::runtime_error((const char *)ulib::string(view).c_str()) {}
        RuntimeError(ulib::u8string_view view) : std::runtime_error((const char *)ulib::u8string(view).c_str()) {}

    private:
    };
} // namespace ulib

#define ULIB_RUNTIME_ERROR(ClassName)                                                                                   \
    class ClassName : public ulib::RuntimeError                                                                        \
    {                                                                                                                  \
    public:                                                                                                            \
        ClassName(ulib::string_view str) : ulib::RuntimeError(str)                                                     \
        {                                                                                                              \
        }                                                                                                              \
        ClassName(ulib::u8string_view str) : ulib::RuntimeError(str)                                                   \
        {                                                                                                              \
        }                                                                                                              \
    };