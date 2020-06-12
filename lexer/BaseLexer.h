#ifndef BASE_LEXER_H
#define BASE_LEXER_H

#include <cstdlib>
#include <string>
#include <vector>

#include "BaseLexerCore.h"

template<typename T>
concept LexerToken = requires(T a) {
    { a.value } -> std::convertible_to<typename T::value_type>;
    { a.text } -> std::convertible_to<std::string>;
    { a.line } -> std::convertible_to<int>;
    { a.col } -> std::convertible_to<int>;
    { a.begin } -> std::convertible_to<std::size_t>;
    { a.end } -> std::convertible_to<std::size_t>;
    { T{.value = a.value, .text = a.text, .line = a.line, .col = a.col, .begin = a.begin, .end = a.end} };
};

template<LexerToken Token_T>
class BaseLexer : public BaseLexerCore {
public:
    struct Error {
        std::string error;
        std::string text;
        int line;
        int col;
    };

private:
    std::vector<Token_T> *m_tokens;
    std::vector<Error> *m_errors;
    bool m_fail;

protected:
    [[nodiscard]] bool ok() const {
        return !m_fail;
    }

    void fail(bool x) {
        m_fail = x;
    }

    void add_error(std::string str, int line, int col) {
        m_errors->push_back({str, get_string(), line, col});
    }

    void make_token(typename Token_T::value_type value) {
        m_tokens->push_back({value, get_string(), line(), col(), begin_offset(), end_offset()});
    }

    void lex(const std::string &str, std::vector<Token_T> &tokens, std::vector<Error> &errors) {
        m_tokens = &tokens;
        m_errors = &errors;
        m_fail = false;
        BaseLexerCore::lex(str);
    }
};

#endif
