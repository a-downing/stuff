#ifndef COLOR_ENUM_H
#define COLOR_ENUM_H
#include "enum_meta.h"
enum class Color {
    RED = 42,
    GREEN = 43,
    BLUE = 44,
};
template<> struct enum_name<Color, Color::RED> {
    using value_type = const char *;
    constexpr static value_type value = "RED";
};
template<> struct enum_name<Color, Color::GREEN> {
    using value_type = const char *;
    constexpr static value_type value = "GREEN";
};
template<> struct enum_name<Color, Color::BLUE> {
    using value_type = const char *;
    constexpr static value_type value = "BLUE";
};
template<> struct enum_entries<Color> {
    using value_type = std::array<enum_entry<Color>, 3>;
    constexpr static value_type value = {
        enum_entry<Color>{"RED", Color::RED},
        enum_entry<Color>{"GREEN", Color::GREEN},
        enum_entry<Color>{"BLUE", Color::BLUE},
    };
};
#endif