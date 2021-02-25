#include <stage_filetotokens.hpp>

#include <scanner.hpp>

#include <cstring>
#include <iostream>

namespace piper {
    FileToTokens::FileToTokens(std::string filename) {
        this->filename = filename;
        this->debug = true;
    }

    bool FileToTokens::shouldRun() {
        return true;
    }

    void FileToTokens::run(
        std::map<std::string, parse_result> *results
    ) {
        auto *scanner = new Scanner(this->filename);

        parse_result pr;
        // 9 -- magic number
        // 1 -- file version
        // 1 -- # of strings
        pr.output_size = 9 + 1 + 1;

        auto tokens = scanner->getTokens();
        
        std::vector<std::string> static_strings;
        for (auto token : *tokens) {
            // one byte for the opcode itself
            // one for the line number
            // one for the column number
            pr.output_size += 3;

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
                            // 1 -- size
                            pr.output_size += 1 + token->str.size();
                            static_strings.push_back(token->str);
                        }

                        // Storing the str_id regardless of if this string is "new"
                        pr.output_size += 1;
                    }
                    break;
            }
        }
        
        char file_version = 0;        
        const char *magic = "PPRALCFTT";

        pr.bytes = (char *)malloc(sizeof(char) * pr.output_size);

        int pr_idx = 0;

        for (int i = 0; i < strlen(magic); i++) {
            pr.bytes[pr_idx++] = magic[i];
        }
        pr.bytes[pr_idx++] = file_version;

        pr.bytes[pr_idx++] = static_strings.size();
        for (int i = 0; i < static_strings.size(); i++) {
            auto s = static_strings.at(i);
            char len = s.length();
            pr.bytes[pr_idx++] = len;
            for (int i = 0; i < len; i++) {
                pr.bytes[pr_idx++] = s.at(i);
            }
        }

        for (auto token : *tokens) {
            char c = static_cast<char>(token->type);

            pr.bytes[pr_idx++] = c;
            pr.bytes[pr_idx++] = token->start.line;
            pr.bytes[pr_idx++] = token->start.column;

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
                                pr.bytes[pr_idx++] = c;
                                break;
                            }
                        }
                    }
                    break;
            }
        }

        std::string out_filename = this->filename + ".ftt";
        this->write_file(pr, out_filename);

        (*results)["tokens"] = pr;
    }
}
