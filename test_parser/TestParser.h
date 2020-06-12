#ifndef TEST_PARSER_H
#define TEST_PARSER_H

#include <cstddef>
#include <vector>
#include <memory>
#include <array>
#include <concepts>

#include <cstdio>

#include "cpp_lexer/cpp_lexer.h"
#include "pratt_parser/PrattParser.h"
#include "BaseParser.h"
#include "Expression.h"

class TestParser : public BaseParser<cpp_lexer::Token> {
    using Token = cpp_lexer::Token;
    using ExpressionParser = PrattParser<Token, std::unique_ptr<Expression>>;
    ExpressionParser m_parser;

public:
    TestParser() {
        m_parser.addInfixParselet(Token::value_type::equal, 1, binaryParselet);

        m_parser.addInfixParselet(Token::value_type::plus, 2, binaryParselet);
        m_parser.addInfixParselet(Token::value_type::plus, 2, binaryParselet);

        m_parser.addInfixParselet(Token::value_type::star, 3, binaryParselet);
        m_parser.addInfixParselet(Token::value_type::slash, 3, binaryParselet);
        m_parser.addInfixParselet(Token::value_type::bang, 3, postfixParselet);

        m_parser.addPrefixParselet(Token::value_type::plus, 4, unaryParselet);
        m_parser.addPrefixParselet(Token::value_type::minus, 4, unaryParselet);
        m_parser.addPrefixParselet(Token::value_type::bang, 4, unaryParselet);

        m_parser.addPrefixParselet(Token::value_type::identifier, 0, primaryParselet);
        m_parser.addPrefixParselet(Token::value_type::number, 0, primaryParselet);
        m_parser.addPrefixParselet(Token::value_type::lparen, 0, groupingParselet);
    }

    void parse(const std::vector<Token> &tokens) {
        BaseParser::parse(tokens);

        while(!end()) {
            auto expr = m_parser.parseExpression(tokens, m_index);

            if(!match(Token::value_type::semicolon)) {
                std::printf("error: expected ;\n");
                return;
            }
        }
    }

private:
    static std::unique_ptr<Expression> primaryParselet(int, const Token &token, ExpressionParser &) {
        if(token.value == Token::value_type::identifier) {
            return std::make_unique<NameExpression>(token);
        } else {
            return std::make_unique<NumberExpression>(token);
        }
    }

    static std::unique_ptr<Expression> unaryParselet(int precedence, const Token &token, ExpressionParser &parser) {
        auto right = parser.parse(precedence);

        if (!right) {
            std::puts("prefixParselet: error, expected operand");
            return nullptr;
        }

        auto ret = std::make_unique<UnaryExpression>(token, std::move(right));
        std::puts(token.text.c_str());
        return ret;
    }

    static std::unique_ptr<Expression> groupingParselet(int, const Token &token, ExpressionParser &parser) {
        auto right = parser.parse();

        if (!right) {
            std::puts("groupingParselet: error, expected operand");
            return nullptr;
        }

        auto rparen = parser.peek();
        if (rparen.value != Token::Kind::rparen) {
            std::puts("groupingParselet: error, expected ')'");
            return nullptr;
        }

        parser.consume();
        return std::make_unique<GroupExpression>(token, std::move(right));
    };

    static std::unique_ptr<Expression> binaryParselet(int precedence, std::unique_ptr<Expression> left, const Token &token, ExpressionParser &parser) {
        auto right = parser.parse(precedence);

        if(!right) {
            std::puts("binaryParselet: error, expected right-hand operand");
            return nullptr;
        }

        return std::make_unique<BinaryExpression>(std::move(left), token, std::move(right));
    };

    static std::unique_ptr<Expression> postfixParselet(int, std::unique_ptr<Expression> left, const Token &token, ExpressionParser &) {
        return std::make_unique<PostfixExpression>(std::move(left), token);
    };


    template<typename ...Args> requires ((std::convertible_to<Args, std::string_view>) && ...)
    bool check_identifier(Args ...c) {
        return !end() && peek()->value == Token::value_type::identifier && ((c == peek()->text) || ...);
    }
};

#endif
