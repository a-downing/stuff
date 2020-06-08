#ifndef ENUM_H
#define ENUM_H

#include <cstddef>
#include <array>
#include <cassert>
#include <algorithm>
#include <cstring>

template<typename T, T sentinel> struct enum_entry {
    const char *name;
    T value = sentinel;
};

template<typename T, typename U, T sentinel>
struct Enum {
    using value_type = T;
    value_type value;

    constexpr auto operator<=>(const Enum<T, U, sentinel>&) const = default;

    template<std::size_t N>
    static consteval auto to_array(const enum_entry<T, sentinel> (&entries)[N]) {
        std::array<enum_entry<T, sentinel>, N> result;
        std::copy(std::begin(entries), std::end(entries), std::begin(result));
        return result;
    }

    consteval static auto entries() {
        T val = 0;
        auto ent = U::_entries();

        for(auto &x : ent) {
            if(x.value == sentinel) {
                val++;
            } else {
                val = x.value;
            }

            x.value = val;
        }

        return ent;
    }

    constexpr static U from(const char *name) {
        T val = 0;

        for(auto x : entries()) {
            if(x.value == sentinel) {
                val++;
            } else {
                val = x.value;
            }

            if(std::strcmp(name, x.name) == 0) {
                return U{val};
            }
        }

        // bug?
        ///*if(name == name)*/ assert(!"no such name");
        return U{0};
    }

    constexpr static U from(value_type value) {
        return U{value};
    }

    constexpr static const char *name(U u) {
        for(auto x : entries()) {
            if(x.value == u.value) {
                return x.name;
            }
        }

        // bug?
        ///*if(u == u)*/ assert(!"no such value");
        return "";
    }

    consteval static U cfrom(const char *name) {
        return from(name);
    }

    consteval static U cfrom(value_type value) {
        return from(value);
    }

    consteval static const char *cname(U u) {
        return name(u);
    }
};

#endif