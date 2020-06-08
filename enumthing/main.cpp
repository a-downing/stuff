#include <iostream>

#include "enum.h"

struct Color : public Enum<int, Color, -1> {
    constexpr static auto _entries() {
        return to_array({
            {"red", 42},
            {"green"},
            {"blue", 45},
            {"black"}
        });
    }
};

int main() {
    for(auto entry : Color::entries()) {
        std::cout << entry.name << " = " << entry.value << '\n';
    }

    for(auto entry : Color::entries()) {
        std::cout << Color::name(Color::from(entry.name)) << " = " << Color::from(entry.value).value << '\n';
    }

    std::cout << Color::cfrom("black").value << '\n';
    std::cout << Color::cname(Color::cfrom(Color::cfrom("black").value)) << '\n';

    return 0;
}
