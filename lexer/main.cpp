#include <cctype>
#include <string>
#include <cstdio>
#include <vector>
#include <fstream>
#include <concepts>

#include "base_lexer.h"

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
        semicolon,
    };

    using value_type = Kind;

    Kind value;
    std::string text;
    int line;
    int col;

    static constexpr std::size_t index(Kind k) {
        return static_cast<std::size_t>(k);
    }

    static constexpr const char *tokenKinds[] = {"identifier", "number", "string", "equals", "minus", "plus", "star", "star_star", "slash", "caret", "lparen", "rparen", "semicolon"};

    static constexpr const char *name(Kind k) {
        return tokenKinds[index(k)];
    }
};

class TestLexer : public BaseLexer<Token> {
public:
    void lex(const std::string &str, std::vector<Token> &tokens, std::vector<Error> &errors) {
        BaseLexer::lex(str, tokens, errors);

        while(!end() && ok()) {
            reset();

            const char c = consume();
            bool handled = true;

            switch(c) {
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

            if(!handled) {
                if(std::isspace(c)) {
                    if(c == '\n') {
                        newline();
                    }
                } else if(c == '_' || std::isalpha(c)) {
                    if(eat_identifier()) {
                        make_token(Token::Kind::identifier);
                    }
                } else if(c == '.' || std::isdigit(c)) {
                    if(eat_number()) {
                        make_token(Token::Kind::number);
                    }
                } else if(c == '"') {
                    if(eat_string()) {
                        make_token(Token::Kind::string);
                    }
                } else {
                    add_error("unhandled character \""s + c + "\"\n", line(), col());
                    fail(true);
                }
            }
        }
    }

    bool eat_identifier() {
        while(check('_') || std::isalnum(peek())) {
            advance();
        }

        return true;
    }

    bool eat_number() {
        while(!end() && std::isdigit(peek())) {
            advance();
        }

        match('.');

        while(!end() && std::isdigit(peek())) {
            advance();
        }

        if(check('e')) {
            advance();
            match('+', '-');

            if(!std::isdigit(peek())) {
                add_error("invalid number", line(), col());
                fail(true);
            } else {
                while(!end() && std::isdigit(peek())) {
                    advance();
                }
            }
        }

        return ok();
    }

    bool eat_string() {
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
            add_error("unterminated string\n", line(), col());
            fail(true);
        } else {
            advance();
        }

        return ok();
    }
};

std::string readFile(const char *filename) {
    std::ifstream f(filename);

    if(!f.good()) {
        return "";
    }

    f.seekg(0, std::ios::end);
    std::size_t size = f.tellg();
    f.seekg(0, std::ios::beg);
    std::string str(size, '\0');
    f.read(str.data(), size);
    return str;
}

int main() {
    TestLexer lexer;
    std::vector<Token> tokens;
    std::vector<TestLexer::Error> errors;
    std::string code = readFile("../code.txt");

    //int total = 0;
    //for(int i = 0; i < 10000000; i++) {
    //    tokens.clear();
    //    errors.clear();
        lexer.lex(code, tokens, errors);
    //    total += tokens.size();
    //}

    //std::printf("total tokens processed: %d\n", total);

    for(const auto &token : tokens) {
        std::printf("token %s: \"%s\"\n    line: %d\n    col: %d\n\n", Token::name(token.value), token.text.c_str(), token.line, token.col);
    }

    for(const auto &error : errors) {
        std::printf("error on line: %d, col: %d: %s (%s)\n", error.line, error.col, error.error.c_str(), error.text.c_str());
    }

    return 0;
}
