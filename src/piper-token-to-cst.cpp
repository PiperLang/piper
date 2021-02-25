#include <array>
#include <cassert>
#include <cerrno>
#include <cstring>
#include <vector>

#include <iostream>

#include <scanner.hpp>
#include <tokentype.hpp>

#define INIT_BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    std::freopen(nullptr, "rb", stdin);

    if (std::ferror(stdin)) {
        throw std::runtime_error(std::strerror(errno));
    }

    std::size_t len;
    std::array<char, INIT_BUFFER_SIZE> buf;

    std::vector<char> input;

    while ((len = std::fread(buf.data(), sizeof(buf[0]), buf.size(), stdin)) > 0) {
        if (std::ferror(stdin) && !std::feof(stdin)) {
            throw std::runtime_error(std::strerror(errno));
        }

        input.insert(input.end(), buf.data(), buf.data() + len);
    }

    int idx = 0;

    assert(input.at(idx++) == 'P');
    assert(input.at(idx++) == 'P');
    assert(input.at(idx++) == 'R');
    assert(input.at(idx++) == 'A');
    assert(input.at(idx++) == 'L');
    assert(input.at(idx++) == 'C');
    assert(input.at(idx++) == 'F');
    assert(input.at(idx++) == 'T');
    assert(input.at(idx++) == 'T');

    assert(input.at(idx++) == 0);

    int string_count = (int)input.at(idx++);

    std::cout << "String Count: " << string_count << std::endl;
    for (int i = 0; i < string_count; i++) {
        int single_string_length = (int)input.at(idx++);
        std::cout << "    Single string (" << single_string_length << "): ";
        for (int p = 0; p < single_string_length; p++) {
            std::cout << input.at(idx++);
        }
        std::cout << std::endl;
    }

    while (true) {
        bool is_at_end = false;

        uint8_t token_type = input.at(idx++);
        uint8_t token_line = input.at(idx++);
        uint8_t token_column = input.at(idx++);
        
        std::cout
            << "(" << std::hex << (int)token_type << "): "
            << piper::Scanner::getTokenName(static_cast<piper::TokenType>(token_type))
            << std::endl;

        switch (token_type) {
            case piper::TokenType::TOKEN_EOF:
                is_at_end = true;
                break;
            case piper::TokenType::FORMAT_STRING:
            case piper::TokenType::IDENTIFIER:
            case piper::TokenType::NUMBER:
            case piper::TokenType::STRING:
                idx++;
                break;
        }

        if (is_at_end) break;
    }

    // use input vector here
}