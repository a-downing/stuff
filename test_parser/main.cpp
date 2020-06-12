#include <cctype>
#include <string>
#include <cstdio>
#include <vector>
#include <fstream>
#include <concepts>

#include "TestParser.h"

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
    cpp_lexer::Lexer lexer;
    std::vector<cpp_lexer::Token> tokens;
    std::vector<cpp_lexer::Lexer::Error> errors;
    std::string code = readFile(argv[1]);

    lexer.lex(code, tokens, errors);

    if(!errors.empty()) {
        for(const auto &error : errors) {
            std::printf("error on line: %d, col: %d: %s (%s)\n", error.line, error.col, error.error.c_str(), error.text.c_str());
        }

        return 1;
    }

    for(const auto &token : tokens) {
        std::printf("token: %s (%s)\n", cpp_lexer::Token::name(token.value), token.text.c_str());
    }

    TestParser parser;
    parser.parse(tokens);

    return 0;
}
