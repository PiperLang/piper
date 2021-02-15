#include <fstream>
#include <iostream>
#include <string>

#include <string.h>

#include <scanner.hpp>

void output_bin0(std::string out_filename, std::vector<piper::Token *> *tokens) {
    std::ofstream fout;
    std::streambuf *buf;

    if (out_filename.compare("-") == 0) {
        buf = std::cout.rdbuf();
    } else {
        fout.open(out_filename, std::ios::binary | std::ios::out);
        buf = fout.rdbuf();
    }

    std::ostream out(buf);

    std::vector<std::string> static_strings;

    for (auto token : *tokens) {
        switch (token->type) {
            case piper::TokenType::FORMAT_STRING:
            case piper::TokenType::IDENTIFIER:
            case piper::TokenType::NUMBER:
            case piper::TokenType::STRING:
                {
                    bool found = false;
                    for (auto s: static_strings) {
                        if (s.compare(token->str) == 0) {
                            found = true;
                            break;
                        }
                    }
                    if (!found) {
                        static_strings.push_back(token->str);
                    }
                }
                break;
        }
    }

    char file_version = 0;
    char num_strings = static_strings.size();

    out.write(&file_version, sizeof(char));
    out.write(&num_strings, sizeof(char));

    for (auto s: static_strings) {
        char len = s.length();
        out.write(&len, sizeof(len));
        for (int i = 0; i < len; i++) {
            out.write(&s.at(i), sizeof(char));
        }
    }

    for (auto token : *tokens) {
        char c = static_cast<char>(token->type);
        out.write(&c, sizeof(c));

        switch (token->type) {
            case piper::TokenType::FORMAT_STRING:
            case piper::TokenType::IDENTIFIER:
            case piper::TokenType::NUMBER:
            case piper::TokenType::STRING:
                {
                    for (int i = 0; i < static_strings.size(); i++) {
                        auto s = static_strings.at(i);

                        if (s.compare(token->str) == 0) {
                            c = static_cast<char>(i);
                            out.write(&c, sizeof(c));
                            break;
                        }
                    }
                }
                break;
        }
    }

    if (fout) {
        fout.close();
    }
}

int main(int argc, char *argv[]) {
    std::string my_name = argv[0];

    if (argc <= 1) {
        std::cout << "Usage: " << my_name << " filename" << std::endl;
        return 0;
    }

    std::cout << "Max Tokens: " << piper::TokenType::TOKEN_MAX << std::endl;

    bool dump_tokens = false;
    std::string output_file = "-";

    for (int i = 2; i < argc; i++) {
        if (strcmp(argv[i], "--dump-tokens") == 0) {
            dump_tokens = true;
        } else if (strcmp(argv[i], "--out-tokens") == 0) {
            if (i + 1 < argc) {
                i++;
                output_file = argv[i];
            } else {
                std::cout << "Provide filename with --out-tokens" << std::endl;
                return -3;
            }
        } else {
            std::cout << "Unknown argument: " << argv[i] << std::endl;
            return -2;
        }
    }

    std::string filename = argv[1];
    piper::Scanner *scanner = new piper::Scanner(filename);
    auto tokens = scanner->getTokens();

    if (dump_tokens) {
        for (auto token : *tokens) {
            std::cout << scanner->getTokenName(token->type) << std::endl;
        }
    }

    output_bin0(output_file, tokens);
}