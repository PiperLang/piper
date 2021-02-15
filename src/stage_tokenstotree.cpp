#include <stage_tokenstotree.hpp>

#include <scanner.hpp>

#include <statements/import_statement.hpp>

#include <cassert>
#include <cstring>
#include <iostream>

#define BYTES_PER_INSTRUCTION 3

namespace piper {
    TokensToTree::TokensToTree(std::string filename) {
        this->filename = filename;
        this->debug = true;
        this->idx = 0;
    }

    bool TokensToTree::shouldRun() {
        return true;
    }

    void TokensToTree::parse_strings(parse_result results) {
        char num_strings = results.bytes[this->idx++];

        for (int i = 0; i < num_strings; i++) {
            char str_len = results.bytes[this->idx++];
            std::string str(&(results.bytes[this->idx]), str_len);
            if (this->debug) {
                std::cout << "String: " << str << std::endl;
            }
            this->idx += str_len;

            this->embedded_strings.push_back(str);
        }
    }

    void TokensToTree::term_expression(parse_result tokens) {
        this->factor_expression(tokens);

        while (true) {
            TokenType tt = static_cast<TokenType>(tokens.bytes[this->idx]);
            switch (tt) {
                case TokenType::MINUS:
                    {
                        this->idx += BYTES_PER_INSTRUCTION;
                        this->factor_expression(tokens);
                    }
                    break;
                case TokenType::PLUS:
                    {
                        this->idx += BYTES_PER_INSTRUCTION;
                        this->factor_expression(tokens);
                        break;
                    }
                default:
                    return;
            }
        }
    }

    void TokensToTree::factor_expression(parse_result tokens) {
        this->unary_expression(tokens);
    }

    void TokensToTree::unary_expression(parse_result tokens) {
        this->call_expression(tokens);
    }

    void TokensToTree::call_expression(parse_result tokens) {
        this->primary_expression(tokens);

        TokenType tmp = static_cast<TokenType>(tokens.bytes[this->idx]);
        if (tmp == TokenType::LEFT_PARENTHESIS) {
            this->idx += BYTES_PER_INSTRUCTION;

            while (true) {
                tmp = static_cast<TokenType>(tokens.bytes[this->idx]);
                if (tmp == TokenType::RIGHT_PARENTHESIS) {
                    this->idx += BYTES_PER_INSTRUCTION;
                    break;
                }

                this->parse_expression(tokens);
            }
        }
    }

    void TokensToTree::primary_expression(parse_result tokens) {
        TokenType tmp = static_cast<TokenType>(tokens.bytes[this->idx]);
        this->idx += BYTES_PER_INSTRUCTION;

        switch (tmp) {
            case TokenType::LEFT_PARENTHESIS:
                {
                    this->primary_expression(tokens);
                    tmp = static_cast<TokenType>(tokens.bytes[this->idx]);
                    this->idx += BYTES_PER_INSTRUCTION;

                    assert(tmp == TokenType::RIGHT_PARENTHESIS);
                }
                break;
            case TokenType::IDENTIFIER:
                {
                    char str_id = tokens.bytes[this->idx++];
                    // return in;
                }
                break;
            case TokenType::NUMBER:
                {
                    char str_id = tokens.bytes[this->idx++];
                }
                break;
            default:
                std::cout << "Failed at primary: " << Scanner::getTokenName(tmp) << std::endl;
                exit(-1);
        }
    }

    void TokensToTree::parse_expression(parse_result tokens) {
        this->term_expression(tokens);
    }

    void TokensToTree::parse_statement(parse_result tokens) {
        TokenType tmp = static_cast<TokenType>(tokens.bytes[this->idx]);
        this->idx += BYTES_PER_INSTRUCTION;

        switch (tmp) {
            case TokenType::IF:
                {
                    tmp = static_cast<TokenType>(tokens.bytes[this->idx]);
                    this->idx += BYTES_PER_INSTRUCTION;
                    assert(tmp == TokenType::LEFT_PARENTHESIS);

                    tmp = static_cast<TokenType>(tokens.bytes[this->idx]);
                    this->idx += BYTES_PER_INSTRUCTION;
                    assert(tmp == TokenType::IDENTIFIER);

                    char str_id = tokens.bytes[this->idx++];

                    tmp = static_cast<TokenType>(tokens.bytes[this->idx]);
                    this->idx += BYTES_PER_INSTRUCTION;
                    assert(tmp == TokenType::LEFT_ANGLE);

                    tmp = static_cast<TokenType>(tokens.bytes[this->idx]);
                    this->idx += BYTES_PER_INSTRUCTION;
                    assert(tmp == TokenType::NUMBER);

                    str_id = tokens.bytes[this->idx++];

                    tmp = static_cast<TokenType>(tokens.bytes[this->idx]);
                    this->idx += BYTES_PER_INSTRUCTION;
                    assert(tmp == TokenType::RIGHT_PARENTHESIS);

                    this->parse_block(tokens);
                }
                break;
            case TokenType::IDENTIFIER:
                {
                    char str_id = tokens.bytes[this->idx++];

                    tmp = static_cast<TokenType>(tokens.bytes[this->idx]);
                    if (tmp != TokenType::LEFT_PARENTHESIS) {
                        this->idx++;
                        std::cout << "Expected (, found " << Scanner::getTokenName(tmp) << " at " << tokens.bytes[this->idx++] << ":" << tokens.bytes[this->idx++] << std::endl;
                        exit(-1);
                    }
                    this->idx += BYTES_PER_INSTRUCTION;
                    assert(tmp == TokenType::LEFT_PARENTHESIS);

                    tmp = static_cast<TokenType>(tokens.bytes[this->idx]);
                    this->idx += BYTES_PER_INSTRUCTION;
                    assert(tmp == TokenType::FORMAT_STRING);

                    str_id = tokens.bytes[this->idx++];

                    tmp = static_cast<TokenType>(tokens.bytes[this->idx]);
                    this->idx += BYTES_PER_INSTRUCTION;
                    assert(tmp == TokenType::COMMA);

                    tmp = static_cast<TokenType>(tokens.bytes[this->idx]);
                    this->idx += BYTES_PER_INSTRUCTION;
                    assert(tmp == TokenType::IDENTIFIER);

                    str_id = tokens.bytes[this->idx++];

                    tmp = static_cast<TokenType>(tokens.bytes[this->idx]);
                    this->idx += BYTES_PER_INSTRUCTION;
                    assert(tmp == TokenType::COLON);

                    tmp = static_cast<TokenType>(tokens.bytes[this->idx]);
                    this->idx += BYTES_PER_INSTRUCTION;
                    assert(tmp == TokenType::IDENTIFIER);

                    str_id = tokens.bytes[this->idx++];

                    tmp = static_cast<TokenType>(tokens.bytes[this->idx]);
                    this->idx += BYTES_PER_INSTRUCTION;
                    assert(tmp == TokenType::LEFT_PARENTHESIS);

                    tmp = static_cast<TokenType>(tokens.bytes[this->idx]);
                    this->idx += BYTES_PER_INSTRUCTION;
                    assert(tmp == TokenType::NUMBER);

                    str_id = tokens.bytes[this->idx++];

                    tmp = static_cast<TokenType>(tokens.bytes[this->idx]);
                    this->idx += BYTES_PER_INSTRUCTION;
                    assert(tmp == TokenType::RIGHT_PARENTHESIS);

                    tmp = static_cast<TokenType>(tokens.bytes[this->idx]);
                    this->idx += BYTES_PER_INSTRUCTION;
                    assert(tmp == TokenType::RIGHT_PARENTHESIS);

                    tmp = static_cast<TokenType>(tokens.bytes[this->idx]);
                    this->idx += BYTES_PER_INSTRUCTION;
                    assert(tmp == TokenType::SEMICOLON);
                }
                break;
            case TokenType::RETURN:
                {
                    this->parse_expression(tokens);

                    tmp = static_cast<TokenType>(tokens.bytes[this->idx]);
                    this->idx += BYTES_PER_INSTRUCTION;
                    assert(tmp == TokenType::SEMICOLON);
                }
                break;
            default:
                std::cout << "Cannot start statement with " << Scanner::getTokenName(tmp) << std::endl;
                break;
        }
    }

    void TokensToTree::parse_block(parse_result tokens) {
        TokenType tmp = static_cast<TokenType>(tokens.bytes[this->idx]);
        if (tmp != TokenType::LEFT_CURLY) {
            this->parse_statement(tokens);
            return;
        }
        this->idx += BYTES_PER_INSTRUCTION;;

        while (true) {
            // Note: We do not advance here.
            tmp = static_cast<TokenType>(tokens.bytes[this->idx]);
            if (tmp == TokenType::RIGHT_CURLY) {
                this->idx += BYTES_PER_INSTRUCTION;
                return;
            }

            this->parse_statement(tokens); 
        }
    }

    void TokensToTree::parse_function_arguments(parse_result tokens) {
        while (true) {
            TokenType tmp = static_cast<TokenType>(tokens.bytes[this->idx]);
            if (tmp == TokenType::RIGHT_PARENTHESIS) break;
            this->idx += BYTES_PER_INSTRUCTION;

            assert(tmp == TokenType::U64);

            tmp = static_cast<TokenType>(tokens.bytes[this->idx]);
            this->idx += BYTES_PER_INSTRUCTION;
            assert(tmp == TokenType::COLON);

            tmp = static_cast<TokenType>(tokens.bytes[this->idx]);
            this->idx += BYTES_PER_INSTRUCTION;
            assert(tmp == TokenType::IDENTIFIER);

            char str_id = tokens.bytes[this->idx++];
        }
    }

    Statement *TokensToTree::parse_top_level_statement(parse_result tokens) {
        TokenType tokenType = static_cast<TokenType>(tokens.bytes[this->idx]);
        this->idx += BYTES_PER_INSTRUCTION;

        switch (tokenType) {
            case TokenType::IMPORT:
                {
                    TokenType tmp = static_cast<TokenType>(tokens.bytes[this->idx]);
                    this->idx += BYTES_PER_INSTRUCTION;
                    assert(tmp == TokenType::MODULE);

                    tmp = static_cast<TokenType>(tokens.bytes[this->idx]);
                    this->idx += BYTES_PER_INSTRUCTION;
                    assert(tmp == TokenType::STRING);

                    char str_id = tokens.bytes[this->idx++];

                    tmp = static_cast<TokenType>(tokens.bytes[this->idx]);
                    this->idx += BYTES_PER_INSTRUCTION;
                    assert(tmp == TokenType::SEMICOLON);

                    return new ImportStatement(this->embedded_strings.at(str_id));
                }
                break;
            case TokenType::FUNCTION:
                {
                    TokenType tmp = static_cast<TokenType>(tokens.bytes[this->idx]);
                    this->idx += BYTES_PER_INSTRUCTION;
                    assert(tmp == TokenType::IDENTIFIER);

                    char str_id = tokens.bytes[this->idx++];

                    tmp = static_cast<TokenType>(tokens.bytes[this->idx]);
                    this->idx += BYTES_PER_INSTRUCTION;
                    assert(tmp == TokenType::LEFT_PARENTHESIS);

                    this->parse_function_arguments(tokens);

                    tmp = static_cast<TokenType>(tokens.bytes[this->idx]);
                    this->idx += BYTES_PER_INSTRUCTION;
                    assert(tmp == TokenType::RIGHT_PARENTHESIS);

                    // We're done with the argument list. Return value!

                    tmp = static_cast<TokenType>(tokens.bytes[this->idx]);
                    this->idx += BYTES_PER_INSTRUCTION;
                    assert(tmp == TokenType::COLON);

                    tmp = static_cast<TokenType>(tokens.bytes[this->idx]);
                    this->idx += BYTES_PER_INSTRUCTION;
                    assert(
                        (tmp == TokenType::U64) ||
                        (tmp == TokenType::I64)
                    );

                    this->parse_block(tokens);
                }
                break;
            default:
                std::cout << "ERROR: Cannot start top-level statement with " << Scanner::getTokenName(tokenType) << std::endl;
                break;
        }

        std::cout << "parse_top_level_statement:" << Scanner::getTokenName(tokenType) << std::endl;
        return NULL;
    }

    void TokensToTree::run(
        std::map<std::string, parse_result> *results
    ) {
        parse_result tokens = results->at("tokens");
        if (strncmp("PPRALCFTT", tokens.bytes, 9) != 0) {
            std::cout << "ERROR: Invalid magic signature for tokens: %.*s" << std::endl;
            return;
        }

        // Skip magic bytes and (ignored, for now) version
        this->idx = 10;

        this->parse_strings(tokens);

        while (true) {
            TokenType tokenType = static_cast<TokenType>(tokens.bytes[this->idx]);
            if (tokenType == TokenType::TOKEN_EOF) break;

            Statement *statement = this->parse_top_level_statement(tokens);
            if (statement == NULL) {
                std::cout << "Top level statement was NULL" << std::endl;
            } else {
                std::cout << "Got top level statement: " << statement->toString() << std::endl;
            }
        }
    }
}
