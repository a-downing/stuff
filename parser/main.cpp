#include <cstdio>
#include <cstddef>
#include <string>
#include <utility>
#include <vector>
#include <memory>
#include <array>

#include "parser.h"

using namespace std::literals;

enum class TokenType {
    PLUS,
    MINUS,
    MUL,
    BANG,
    LPAREN,
    RPAREN,
    NUMBER,
    SEMICOLON,

    MAX_VALUE
};

struct Token {
    using value_type = TokenType;
    value_type value;
    std::string text;
};

struct Expression {
    virtual ~Expression() = default;
    [[nodiscard]] virtual std::string toString() const = 0;
    [[nodiscard]] virtual std::string_view name() const = 0;
};

struct PrefixExpression : public Expression {
    Token oper;
    std::unique_ptr<Expression> right;

    explicit PrefixExpression(Token oper, std::unique_ptr<Expression> right) : oper(std::move(oper)), right(std::move(right)) {
        std::puts(toString().c_str());
    }

    [[nodiscard]] std::string_view name() const override {
        return "PrefixExpression";
    };

    [[nodiscard]] std::string toString() const override {
        return "("s + std::string(name()) + " "s + oper.text + right->toString() + ")"s;
    }
};

struct PostfixExpression : public Expression {
    std::unique_ptr<Expression> left;
    Token oper;

    explicit PostfixExpression(std::unique_ptr<Expression> left, Token oper) : left(std::move(left)), oper(std::move(oper)) {
        std::puts(toString().c_str());
    }

    [[nodiscard]] std::string_view name() const override {
        return "PostfixExpression";
    };

    [[nodiscard]] std::string toString() const override {
        return "("s + std::string(name()) + " "s + left->toString() + oper.text + ")"s;
    }
};

struct InfixExpression : public Expression {
    std::unique_ptr<Expression> left;
    Token oper;
    std::unique_ptr<Expression> right;

    explicit InfixExpression(std::unique_ptr<Expression> left, Token oper, std::unique_ptr<Expression> right) : left(std::move(left)), oper(std::move(oper)), right(std::move(right)) {
        std::puts(toString().c_str());
    }

    [[nodiscard]] std::string_view name() const override {
        return "InfixExpression";
    };

    [[nodiscard]] std::string toString() const override {
        return "("s + std::string(name()) + " " + left->toString() + " " + oper.text + " " + right->toString() + ")";
    }
};

struct NumberExpression : public Expression {
    Token token;

    explicit NumberExpression(Token token) : token(std::move(token)) {
        std::puts(toString().c_str());
    }

    [[nodiscard]] std::string_view name() const override {
        return "NumberExpression";
    };

    [[nodiscard]] std::string toString() const override {
        return token.text;
    }
};

int main() {
    std::vector<Token> tokens = {
        {TokenType::MINUS, "-"},
        {TokenType::LPAREN, "("},
        {TokenType::NUMBER, "1"},
        {TokenType::PLUS, "+"},
        {TokenType::NUMBER, "2"},
        {TokenType::RPAREN, ")"},
        {TokenType::MUL, "*"},
        {TokenType::NUMBER, "3"},
        {TokenType::PLUS, "+"},
        {TokenType::NUMBER, "4"},
        {TokenType::BANG, "!"},
        {TokenType::SEMICOLON, ";"},
    };

    using Parser = Parser<Token, Expression, static_cast<std::size_t>(TokenType::MAX_VALUE)>;

    Parser parser;
    std::size_t index = 0;
    parser.init(tokens, index);

    auto primaryParselet =  [](int precedence, const Token &token, Parser &parser) -> std::unique_ptr<Expression> {
        return std::make_unique<NumberExpression>(token);
    };

    auto prefixParselet =  [](int precedence, const Token &token, Parser &parser) -> std::unique_ptr<Expression> {
        auto right = parser.parse(precedence);

        if (!right) {
            std::puts("prefixParselet: error, expected operand");
            return nullptr;
        }

        auto ret = std::make_unique<PrefixExpression>(token, std::move(right));
        std::puts(token.text.c_str());
        return ret;
    };

    auto groupingParselet =  [](int precedence, const Token &token, Parser &parser) -> std::unique_ptr<Expression> {
        auto right = parser.parse();

        if (!right) {
            std::puts("groupingParselet: error, expected operand");
            return nullptr;
        }

        auto rparen = parser.peek();
        if (rparen.value != TokenType::RPAREN) {
            std::puts("groupingParselet: error, expected ')'");
            return nullptr;
        }

        parser.consume();
        return std::make_unique<PrefixExpression>(token, std::move(right));
    };

    auto binaryParselet =  [](int precedence, std::unique_ptr<Expression> left, const Token &token, Parser &parser) -> std::unique_ptr<Expression> {
        auto right = parser.parse(precedence);

        if(!right) {
            std::puts("binaryParselet: error, expected right-hand operand");
            return nullptr;
        }

        return std::make_unique<InfixExpression>(std::move(left), token, std::move(right));
    };

    auto postfixParselet =  [](int precedence, std::unique_ptr<Expression> left, const Token &token, Parser &parser) -> std::unique_ptr<Expression> {
        return std::make_unique<PostfixExpression>(std::move(left), token);
    };

    parser.addInfixParselet(TokenType::PLUS, 1, binaryParselet);
    parser.addInfixParselet(TokenType::MINUS, 1, binaryParselet);

    parser.addInfixParselet(TokenType::MUL, 2, binaryParselet);
    parser.addInfixParselet(TokenType::BANG, 2, postfixParselet);

    parser.addPrefixParselet(TokenType::PLUS, 3, prefixParselet);
    parser.addPrefixParselet(TokenType::MINUS, 3, prefixParselet);
    parser.addPrefixParselet(TokenType::BANG, 3, prefixParselet);

    parser.addPrefixParselet(TokenType::NUMBER, 0, primaryParselet);
    parser.addPrefixParselet(TokenType::LPAREN, 0, groupingParselet);

    auto expr = parser.parse();

    return 0;
}
