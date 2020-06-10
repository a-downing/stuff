#ifndef LEXER_H
#define LEXER_H

#include "base_lexer.h"

using namespace std::literals;

struct Token {
    enum class Kind {
        identifier,
        number,
        string,
        character,
        equal,
        equal_equal,
        minus,
        minus_minus,
        minus_equal,
        plus,
        plus_plus,
        plus_equal,
        star,
        start_equal,
        slash,
        slash_equal,
        caret,
        caret_equal,
        lparen, rparen,
        lbrace, rbrace,
        lbracket, rbracket,
        semicolon,
        colon,
        scope_resolution,
        bang,
        bang_equal,
        comma,
        lt, gt,
        lte, gte,
        lshift, rshift,
        ampersand,
        ampersand_ampersand,
        ampersand_equal,
        dot,
        dot_star,
        arrow,
        comment,
        macro,
        question,
        percent,
        percent_equal,
        tilde,
        tilde_equal,
        pipe,
        pipe_pipe,
        pipe_equal
    };

    using value_type = Kind;

    Kind value;
    std::string text;
    int line;
    int col;

    static constexpr std::size_t index(Kind k) {
        return static_cast<std::size_t>(k);
    }

    static constexpr const char *tokenKinds[] = {"identifier", "number", "string", "character", "equal", "equal_equal", "minus", "minus_minus", "minus_equal", "plus", "plus_plus", "plus_equal", "star", "start_equal", "slash", "slash_equal", "caret", "caret_equal", "lparen", "rparen", "lbrace", "rbrace", "lbracket", "rbracket", "semicolon", "colon", "scope_resolution", "bang", "bang_equal", "comma", "lt", "gt", "lte", "gte", "lshift", "rshift", "ampersand", "ampersand_ampersand", "ampersand_equal", "dot", "dot_star", "arrow", "comment", "macro", "question", "percent", "percent_equal", "tilde", "tilde_equal", "pipe", "pipe_pipe", "pipe_equal"};

    static constexpr const char *name(Kind k) {
        return tokenKinds[index(k)];
    }
};

class Lexer : public BaseLexer<Token> {
public:
    void lex(const std::string &str, std::vector<Token> &tokens, std::vector<Error> &errors) {
        BaseLexer::lex(str, tokens, errors);

        while(!end() && ok()) {
            reset();

            const char c = consume();
            bool handled = true;

            switch(c) {
            case '=':
                if(match('=')) {
                    make_token(Token::Kind::equal_equal);
                } else {
                    make_token(Token::Kind::equal);
                }

                break;
            case '+':
                if(match('+')) {
                    make_token(Token::Kind::plus_plus);
                } else if(match('=')) {
                    make_token(Token::Kind::plus_equal);
                } else {
                    make_token(Token::Kind::plus);
                }

                break;
            case '^':
                if(match('=')) {
                    make_token(Token::Kind::caret_equal);
                } else {
                    make_token(Token::Kind::caret);
                }

                break;
            case '(':
                make_token(Token::Kind::lparen);
                break;
            case ')':
                make_token(Token::Kind::rparen);
                break;
            case '[':
                make_token(Token::Kind::lbrace);
                break;
            case ']':
                make_token(Token::Kind::rbrace);
                break;
            case '{':
                make_token(Token::Kind::lbracket);
                break;
            case '}':
                make_token(Token::Kind::rbracket);
                break;
            case ';':
                make_token(Token::Kind::semicolon);
                break;
            case '!':
                if(match('=')) {
                    make_token(Token::Kind::bang_equal);
                } else {
                    make_token(Token::Kind::bang);
                }

                break;
            case ',':
                make_token(Token::Kind::comma);
                break;
            case '&':
                if(match('=')) {
                    make_token(Token::Kind::ampersand_equal);
                } else if(match('&')) {
                    make_token(Token::Kind::ampersand_ampersand);
                } else {
                    make_token(Token::Kind::ampersand);
                }

                break;
            case '?':
                make_token(Token::Kind::question);
                break;
            case '%':
                if(match('=')) {
                    make_token(Token::Kind::percent_equal);
                } else {
                    make_token(Token::Kind::percent);
                }

                break;
            case '~':
                if(match('=')) {
                    make_token(Token::Kind::tilde_equal);
                } else {
                    make_token(Token::Kind::tilde);
                }

                break;
            case '|':
                if(match('=')) {
                    make_token(Token::Kind::pipe_equal);
                } else if(match('|')) {
                    make_token(Token::Kind::pipe_pipe);
                } else {
                    make_token(Token::Kind::pipe);
                }

                break;
            case '\\':
                break;
            case '/':
                if(match('/')) {
                    if(eat_comment()) {
                        make_token(Token::Kind::comment);
                    } else {
                        handled = false;
                    }
                } else if(match('*')) {
                    if(eat_multiline_comment()) {
                        make_token(Token::Kind::comment);
                    } else {
                        handled = false;
                    }
                } else {
                    make_token(Token::Kind::slash);
                }

                break;
            case '-':
                if(match('-')) {
                    make_token(Token::Kind::minus_minus);
                } else if(match('=')) {
                    make_token(Token::Kind::minus_equal);
                } else if(match('>')) {
                    make_token(Token::Kind::arrow);
                } else {
                    make_token(Token::Kind::minus);
                }

                break;
            case '.':
                if(match('*')) {
                    make_token(Token::Kind::dot_star);
                } else if(!std::isdigit(peek())) {
                    make_token(Token::Kind::dot);
                }

                break;
            case '<':
                if(match('=')) {
                    make_token(Token::Kind::lte);
                } else {
                    make_token(Token::Kind::lt);
                }

                break;
            case '>':
                if(match('=')) {
                    make_token(Token::Kind::gte);
                } else {
                    make_token(Token::Kind::gt);
                }

                break;
            case ':':
                if(match(':')) {
                    make_token(Token::Kind::scope_resolution);
                } else {
                    make_token(Token::Kind::colon);
                }

                break;
            case '#':
                if(eat_macro()) {
                    make_token(Token::Kind::macro);
                } else {
                    handled = false;
                }

                break;
            case '*':
                if(match('=')) {
                    make_token(Token::Kind::start_equal);
                } else {
                    make_token(Token::Kind::star);
                }

                break;
            default:
                handled = false;
            }

            if(!handled) {
                if(std::isspace(c)) {

                } else if(c == '_' || c == '$' || std::isalpha(c)) {
                    if(eat_identifier()) {
                        make_token(Token::Kind::identifier);
                    }
                } else if(c == '.' || std::isdigit(c)) {
                    if(eat_number()) {
                        make_token(Token::Kind::number);
                    }
                } else if(c == '"') {
                    if(eat_string(c)) {
                        make_token(Token::Kind::string);
                    }
                } else if(c == '\'') {
                    if(eat_string(c)) {
                        make_token(Token::Kind::character);
                    }
                } else {
                    add_error("unhandled character \""s + c + "\"\n", line(), col());
                    fail(true);
                }
            }
        }
    }

    bool eat_identifier() {
        while(check('_') || check('$') || std::isalnum(peek())) {
            advance();
        }

        return true;
    }

    bool eat_number() {
        while(!end() && std::isdigit(peek())) {
            advance();
        }

        bool fp = false;

        if(match('.')) {
           fp = true;
        }

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

        if(fp) {
            match('f', 'l', 'F', 'L');
        }

        return ok();
    }

    bool eat_string(char quote) {
        while(!end() && !check(quote)) {
            if(peek() == '\\') {
                advance();
            }

            advance();
        }

        if(end()) {
            add_error("unterminated string\n", line(), col());
            fail(true);
        } else {
            advance();
        }

        return ok();
    }

    bool eat_macro() {
        while(!end() && !check('\n')) {
            if(match('\\')) {
                match('\n');
            } else {
                advance();
            }
        }

        return ok();
    }


    bool eat_comment() {
        while(!end() && !check('\n')) {
            if(match('\\')) {
                match('\n');
            } else {
                advance();
            }
        }

        return ok();
    }

    bool eat_multiline_comment() {
        while(!end()) {
            if(match('*')) {
                if(check('/')) {
                    advance();
                    break;
                }
            } else {
                advance();
            }
        }

        if(end()) {
            add_error("unterminated comment\n", line(), col());
            fail(true);
        }

        return ok();
    }
};

#endif
