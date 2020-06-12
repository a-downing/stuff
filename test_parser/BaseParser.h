#ifndef BASE_PARSER_H
#define BASE_PARSER_H

#include <cstddef>
#include <vector>
#include <concepts>

template<typename Token_T>
class BaseParser {
protected:
    const std::vector<Token_T> *m_tokens{};
    std::size_t m_index;

    void parse(const std::vector<Token_T> &tokens) {
        m_tokens = &tokens;
        m_index = 0;
    }

    [[nodiscard]] const Token_T *peek() const {
        return end() ? nullptr : m_tokens->data() + m_index;
    }

    void advance() {
        m_index++;
    }

    bool end() const {
        return m_index >= m_tokens->size();
    }

    template<typename ...Args> requires ((std::same_as<typename Token_T::value_type, Args>) && ...)
    bool check(Args ...c) {
        return !end() && ((c == peek()->value) || ...);
    }

    template<typename ...Args> requires ((std::same_as<typename Token_T::value_type, Args>) && ...)
    bool match(Args ...c) {
        if(check(c...)) {
            advance();
            return true;
        }

        return false;
    }
};

#endif
