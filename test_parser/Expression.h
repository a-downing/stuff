#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <cstddef>
#include <memory>
#include <cstdio>
#include <string>

#include "cpp_lexer/cpp_lexer.h"

using namespace std::literals;

using Token = cpp_lexer::Token;

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

struct NameExpression : public Expression {
    Token token;

    explicit NameExpression(Token token) : token(std::move(token)) {
        std::puts(toString().c_str());
    }

    [[nodiscard]] std::string_view name() const override {
        return "NameExpression";
    };

    [[nodiscard]] std::string toString() const override {
        return token.text;
    }
};

#endif
