#include <cstdio>
#include <cstddef>
#include <string>
#include <utility>
#include <vector>
#include <memory>
#include <array>

using namespace std::literals;

template<typename Token_T, typename Expression_T, std::size_t MAX_TOKEN_VALUE>
class Parser {
public:
    using PrefixParselet_t = std::unique_ptr<Expression_T> (*)(int precedence, const Token_T &, Parser &parser);
    using InfixParselet_t = std::unique_ptr<Expression_T> (*)(int precedence, std::unique_ptr<Expression_T>, const Token_T &, Parser &parser);

    struct PrefixParselet {
        int precedence = 0;
        PrefixParselet_t func = nullptr;
    };

    struct InfixParselet {
        int precedence = 0;
        InfixParselet_t func = nullptr;
    };

private:
    std::array<PrefixParselet, MAX_TOKEN_VALUE> m_prefixParselets;
    std::array<InfixParselet, MAX_TOKEN_VALUE> m_infixParselets;
    const std::vector<Token_T> *m_tokens{};
    std::size_t *m_index = nullptr;

public:
    void init(const std::vector<Token_T> &tokens, std::size_t &index) {
        m_index = &index;
        m_tokens = &tokens;
    }

    void addPrefixParselet(typename Token_T::value_type type, int precedence, PrefixParselet_t prefixParselet) {
        m_prefixParselets[static_cast<std::size_t>(type)] = {precedence, prefixParselet};
    }

    void addInfixParselet(typename Token_T::value_type type, int precedence, InfixParselet_t infixParselet) {
        m_infixParselets[static_cast<std::size_t>(type)] = {precedence, infixParselet};
    }

    int getPrecedence(typename Token_T::value_type type) {
        return m_infixParselets[static_cast<std::size_t>(type)].precedence;
    }

    std::unique_ptr<Expression_T> parse(int precedence = 0) {
        if(end()) {
            return nullptr;
        }

        const Token_T *token = &consume();
        PrefixParselet prefixParselet = m_prefixParselets[static_cast<std::size_t>(token->value)];

        if(prefixParselet.func == nullptr) {
            return nullptr;
        }

        std::unique_ptr<Expression_T> left = prefixParselet.func(prefixParselet.precedence, *token, *this);

        if(!left) {
            return nullptr;
        }

        while(!end() && precedence < getPrecedence(peek().value)) {
            token = &consume();
            InfixParselet infixParselet = m_infixParselets[static_cast<std::size_t>(token->value)];

            if(infixParselet.func == nullptr) {
                break;
            }

            left = infixParselet.func(infixParselet.precedence, std::move(left), *token, *this);
        }

        return left;
    }

    const Token_T &consume() {
        return m_tokens->at((*m_index)++);
    }

    [[nodiscard]] const Token_T &peek() const {
        return m_tokens->at(*m_index);
    }

    bool end() {
        return *m_index == m_tokens->size();
    }
};
