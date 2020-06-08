#include <cstddef>
#include <vector>
#include <memory>
#include <array>

#ifdef __cpp_lib_concepts
#include <concepts>
#endif

using namespace std::literals;

#ifdef __cpp_concepts
template<typename T>
concept IndexableToken = requires(T a) {
    { a.value } -> std::convertible_to<typename T::value_type>;
    { T::index(a.value) } -> std::same_as<std::size_t>;
    { T::max_index() } -> std::same_as<std::size_t>;
};

template<typename T>
concept MovableExpression = std::move_constructible<T>;
#else
#define IndexableToken typename
#define MovableExpression typename
#endif

template<IndexableToken Token_T, MovableExpression Expression_T>
class Parser {
public:
    using PrefixParselet_t = Expression_T (*)(int precedence, const Token_T &, Parser &parser);
    using InfixParselet_t = Expression_T (*)(int precedence, Expression_T, const Token_T &, Parser &parser);

private:
    struct PrefixParselet {
        int precedence = 0;
        PrefixParselet_t func = nullptr;
    };

    struct InfixParselet {
        int precedence = 0;
        InfixParselet_t func = nullptr;
    };
    
    std::array<PrefixParselet, Token_T::max_index() + 1> m_prefixParselets;
    std::array<InfixParselet, Token_T::max_index() + 1> m_infixParselets;
    const std::vector<Token_T> *m_tokens{};
    std::size_t *m_index = nullptr;

    int getPrecedence(typename Token_T::value_type value) const {
        if(Token_T::index(value) < m_infixParselets.size()) {
            return m_infixParselets[Token_T::index(value)].precedence;
        } else {
            return 0;
        }
    }

public:
    void init(const std::vector<Token_T> &tokens, std::size_t &index) {
        m_index = &index;
        m_tokens = &tokens;
    }

    void addPrefixParselet(typename Token_T::value_type value, int precedence, PrefixParselet_t prefixParselet) {
        m_prefixParselets[Token_T::index(value)] = {precedence, prefixParselet};
    }

    void addInfixParselet(typename Token_T::value_type value, int precedence, InfixParselet_t infixParselet) {
        m_infixParselets[Token_T::index(value)] = {precedence, infixParselet};
    }

    Expression_T parse(int precedence = 0) {
        if(end()) {
            return nullptr;
        }

        const Token_T &token_l = consume();
        PrefixParselet prefixParselet = m_prefixParselets[Token_T::index(token_l.value)];

        if(prefixParselet.func == nullptr) {
            return nullptr;
        }

        Expression_T left = prefixParselet.func(prefixParselet.precedence, token_l, *this);

        if(!left) {
            return nullptr;
        }

        while(!end() && precedence < getPrecedence(peek().value)) {
            const Token_T &token_r = consume();
            InfixParselet infixParselet = m_infixParselets[Token_T::index(token_r.value)];

            if(infixParselet.func == nullptr) {
                break;
            }

            left = infixParselet.func(infixParselet.precedence, std::move(left), token_r, *this);
        }

        return left;
    }

    const Token_T &consume() {
        return m_tokens->at((*m_index)++);
    }

    [[nodiscard]] const Token_T &peek() const {
        return m_tokens->at(*m_index);
    }

    bool end() const {
        return *m_index == m_tokens->size();
    }
};
