#include <cctype>
#include <string>
#include <cstdio>
#include <vector>
#include <fstream>

#include "lexer.h"

using namespace std::literals;

struct Token {
    enum class Kind {
        identifier,
        number,
        string,
        equals,
        minus,
        plus,
        star,
        star_star,
        slash,
        caret,
        lparen,
        rparen,
        semicolon
    };

    static constexpr const char *tokenKinds[] = {"identifier", "number", "string", "equals", "minus", "plus", "star", "star_star", "slash", "caret", "lparen", "rparen", "semicolon"};

    Kind kind;
    std::string text;
    int line;
    int col;

    std::size_t index() const {
        return static_cast<std::size_t>(kind);
    }
};

class TestLexer : public Lexer {
public:
    struct Error {
        std::string error;
        int line;
        int col;
    };

private:
    std::vector<Token> *m_tokens;
    std::vector<Error> *m_errors;
    bool m_fail;

    void make_token(Token::Kind kind) {
        m_tokens->emplace_back(kind, get_string(), line(), col());
        auto token = m_tokens->back();
    }

public:
    void lex(const std::string &str, std::vector<Token> &tokens, std::vector<Error> &errors) {
        m_tokens = &tokens;
        m_errors = &errors;
        m_fail = false;
        Lexer::lex(str);

        while(!end() && !m_fail) {
            reset();

            const char c = consume();
            bool handled = true;

            switch(c) {
            case ' ':
            case '\r':
                break;
            case '\n':
                newline();
                break;
            case '=':
                make_token(Token::Kind::equals);
                break;
            case '-':
                make_token(Token::Kind::minus);
                break;
            case '+':
                make_token(Token::Kind::plus);
                break;
            case '*':
                if(check('*')) {
                    advance();
                    make_token(Token::Kind::star_star);
                } else {
                    make_token(Token::Kind::star);
                }

                break;
            case '/':
                make_token(Token::Kind::slash);
                break;
            case '^':
                make_token(Token::Kind::caret);
                break;
            case '(':
                make_token(Token::Kind::lparen);
                break;
            case ')':
                make_token(Token::Kind::rparen);
                break;
            case ';':
                make_token(Token::Kind::semicolon);
                break;
            default:
                handled = false;
            }

            if(handled) {
                continue;
            }

            if(c == '_' || std::isalpha(c)) {
                eat_identifer();
                make_token(Token::Kind::identifier);
            } else if(c == '.' || std::isdigit(c)) {
                eat_number();
                make_token(Token::Kind::number);
            } else if(c == '"') {
                eat_string();
                make_token(Token::Kind::string);
            } else {
                m_errors->emplace_back("unhandled character \""s + c + "\"\n", line(), col());
                m_fail = true;
                return;
            }
        }
    }

    void eat_identifer() {
        while(check('_') || std::isalnum(peek())) {
            advance();
        }
    }

    void eat_number() {
        while(!end() && std::isdigit(peek())) {
            advance();
        }

        match('.');

        while(!end() && std::isdigit(peek())) {
            advance();
        }

        match('e');
        match('+', '-');

        while(!end() && std::isdigit(peek())) {
            advance();
        }
    }

    void eat_string() {
        while(!end() && !check('"')) {
            if(peek() == '\\') {
                advance();
            }

            if(check('\n')) {
                advance();
                newline();
            } else {
                advance();
            }
        }

        if(end()) {
            m_errors->emplace_back("unterminated string\n", line(), col());
            m_fail = true;
            return;
        }

        advance();
    }
};

std::string readFile(const char *filename) {
    std::ifstream f(filename);

    if(!f.good()) {
        return "";
    }

    f.seekg(0, f.end);
    std::size_t size = f.tellg();
    f.seekg(0, f.beg);
    std::string str(size, '\0');
    f.read(str.data(), size);
    return str;
}

int main() {
    TestLexer lexer;
    std::vector<Token> tokens;
    std::vector<TestLexer::Error> errors;
    std::string code = readFile("code.txt");

    lexer.lex(code, tokens, errors);

    for(const auto &token : tokens) {
        std::printf("token %s: \"%s\"\n    line: %d\n    col: %d\n\n", Token::tokenKinds[token.index()], token.text.c_str(), token.line, token.col);
    }

    for(const auto &error : errors) {
        std::printf("error on line: %d, col: %d: %s\n", error.line, error.col, error.error.c_str());
    }

    return 0;
}
