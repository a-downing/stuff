#include <cctype>
#include <string>
#include <cstdio>
#include <vector>
#include <fstream>
#include <concepts>

#include "lexer.h"

using namespace std::literals;

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

int main(int, char **argv) {
    Lexer lexer;
    std::vector<Token> tokens;
    std::vector<Lexer::Error> errors;
    std::string code = readFile(argv[1]);

    lexer.lex(code, tokens, errors);

    for(const auto &token : tokens) {
        std::printf("token %s: \"%s\"\n    line: %d\n    col: %d\n\n", Token::name(token.value), token.text.c_str(), token.line, token.col);
    }

    for(const auto &error : errors) {
        std::printf("error on line: %d, col: %d: %s (%s)\n", error.line, error.col, error.error.c_str(), error.text.c_str());
    }

    return 0;
}
