#include <cstdlib>
#include <iostream>

#include "Color.enum.h"

int main() {
    for(std::size_t i = 0; i < enum_entries<Color>::value.size(); i++) {
        const char *name = enum_entries<Color>::value[i].name;
        Color value = enum_entries<Color>::value[i].value;

        std::cout << name << " = " << static_cast<std::underlying_type<Color>::type>(value) << '\n';
    }

    std::cout << "name: " << enum_name<Color, Color::RED>::value << '\n';
    std::cout << "name: " << enum_name<Color, Color::GREEN>::value << '\n';
    std::cout << "name: " << enum_name<Color, Color::BLUE>::value << '\n';

    return 0;
}