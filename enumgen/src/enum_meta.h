#ifndef ENUM_META_H
#define ENUM_META_H

#include <array>
#include <cassert>
#include <type_traits>

template<typename T> struct enum_entry {
    const char *name;
    T value;
};

template<typename T> struct enum_entries {
    static_assert(sizeof(T) != sizeof(T), "You must specialize enum_entries<typename T>");
};

template<typename T, T u> struct enum_name {
    static_assert(sizeof(T) != sizeof(T), "You must specialize enum_name<typename T, typename U, U u>");
};

#endif