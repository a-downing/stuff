#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <string_view>

class Lexer {
    const std::string_view *m_str;
    const char *m_current;
    const char *m_start;
    const char *m_end;
    int m_line;
    int m_col;
    int m_col_start;

protected:
    std::string get_string() {
        return std::string(m_start, m_current - m_start);
    }

    int line() {
        return m_line;
    }

    int col() {
        return m_col_start;
    }

    void reset() {
        m_start = m_current;
        m_col_start = m_col;
    }

    void newline() {
        m_line++;
        m_col = 1;
    }

    void advance() {
        m_current++;
        m_col++;
    }

    char peek() const {
        return end() ? '\0' : *m_current;
    }

    char consume() {
        char c = peek();
        advance();
        return c;
    }

    bool end() const {
        return m_current >= m_end;
    }

    template<typename ...Args>
    bool check(Args ...c) {
        return !end() && (... || (c == peek()));
    }

    template<typename ...Args>
    bool match(Args ...c) {
        if(!end() && ((c == peek()) || ...)) {
            advance();
            return true;
        }

        return false;
    }

public:
    void lex(const std::string_view &str) {
        m_str = &str;
        m_current = m_str->data();
        m_end = m_current + m_str->size();
        m_line = 1;
        m_col = 1;
    }
};

#endif
