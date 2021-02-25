#include <stage_tokenstotree.hpp>

#include <scanner.hpp>

#include <expressions/add_expression.hpp>
#include <expressions/call_expression.hpp>
#include <expressions/double_colon_expression.hpp>
#include <expressions/format_string_expression.hpp>
#include <expressions/greater_than_expression.hpp>
#include <expressions/identifier_expression.hpp>
#include <expressions/less_than_expression.hpp>
#include <expressions/number_expression.hpp>
#include <expressions/sub_expression.hpp>

#include <statements/block_statement.hpp>
#include <statements/expression_statement.hpp>
#include <statements/function_statement.hpp>
#include <statements/if_statement.hpp>
#include <statements/import_statement.hpp>
#include <statements/return_statement.hpp>

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
            this->idx += str_len;

            this->embedded_strings.push_back(str);
        }
    }

    Expression *TokensToTree::parse_expression(parse_result tokens) {
        auto expression = this->comparison_expression(tokens);

        return expression;
    }

    Expression *TokensToTree::comparison_expression(parse_result tokens) {
        auto left = this->term_expression(tokens);
        if (left == NULL) {
            std::cout << "In comparison_expression, got no left." << std::endl;
            return NULL;
        }

        TokenType tt = static_cast<TokenType>(tokens.bytes[this->idx]);
        switch (tt) {
            case TokenType::LEFT_ANGLE:
                {
                    this->idx += BYTES_PER_INSTRUCTION;

                    auto right = this->term_expression(tokens);
                    if (right == NULL) {
                        return NULL;
                    }

                    return new LessThanExpression(left, right);
                }
                break;
            case TokenType::RIGHT_ANGLE:
                {
                    this->idx += BYTES_PER_INSTRUCTION;
                    auto right = this->term_expression(tokens);
                    if (right == NULL) return NULL;

                    return new GreaterThanExpression(left, right);
                }
                break;
            default:
                return left;
        }
    }

    Expression *TokensToTree::term_expression(parse_result tokens) {
        auto expression = this->factor_expression(tokens);
        if (expression == NULL) return NULL;

        while (true) {
            TokenType tt = static_cast<TokenType>(tokens.bytes[this->idx]);
            switch (tt) {
                case TokenType::MINUS:
                    {
                        this->idx += BYTES_PER_INSTRUCTION;
                        auto right = this->factor_expression(tokens);
                        return new SubExpression(expression, right);
                    }
                    break;
                case TokenType::PLUS:
                    {
                        this->idx += BYTES_PER_INSTRUCTION;
                        auto right = this->factor_expression(tokens);
                        return new AddExpression(expression, right);
                        break;
                    }
                default:
                    return expression;
            }
        }

        return expression;
    }

    Expression *TokensToTree::factor_expression(parse_result tokens) {
        auto expression = this->unary_expression(tokens);

        return expression;
    }

    Expression *TokensToTree::unary_expression(parse_result tokens) {
        auto expression = this->call_expression(tokens);

        return expression;
    }

    Expression *TokensToTree::call_expression(parse_result tokens) {
        auto left = this->primary_expression(tokens);

        TokenType tmp = static_cast<TokenType>(tokens.bytes[this->idx]);
        if (tmp == TokenType::LEFT_PARENTHESIS) {
            this->idx += BYTES_PER_INSTRUCTION;

            auto call_expression = new CallExpression(left);

            while (true) {
                tmp = static_cast<TokenType>(tokens.bytes[this->idx]);
                if (tmp == TokenType::RIGHT_PARENTHESIS) {
                    this->idx += BYTES_PER_INSTRUCTION;
                    break;
                }

                auto argument_expression = this->parse_expression(tokens);

                tmp = static_cast<TokenType>(tokens.bytes[this->idx]);
                if (tmp == TokenType::COLON) {
                    // argument_expression is actually the name
                    this->idx += BYTES_PER_INSTRUCTION;
                    auto actual_argument_expression = this->parse_expression(tokens);
                    // TODO: We are ignoring the argument name here. Bad.
                    call_expression->add_argument(actual_argument_expression);
                } else if (tmp == TokenType::COMMA) {
                    this->idx += BYTES_PER_INSTRUCTION;
                    call_expression->add_argument(argument_expression);
                    continue;
                } else if (tmp == TokenType::RIGHT_PARENTHESIS) {
                    // this->idx += BYTES_PER_INSTRUCTION;
                    call_expression->add_argument(argument_expression);
                    continue;
                }
            }

            return call_expression;
        }

        return left;
    }

    Expression *TokensToTree::primary_expression(parse_result tokens) {
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

                    auto identifier_expression = new IdentifierExpression(this->embedded_strings.at(str_id));

                    tmp = static_cast<TokenType>(tokens.bytes[this->idx]);
                    switch (tmp) {
                        case TokenType::COLON_COLON:
                            {
                                this->idx += BYTES_PER_INSTRUCTION;

                                auto right = this->primary_expression(tokens);
                                return new DoubleColonExpression(identifier_expression, right);
                            }
                            break;
                        default:
                            return identifier_expression;
                    }
                }
                break;
            case TokenType::FORMAT_STRING:
                {
                    char str_id = tokens.bytes[this->idx++];

                    auto format_string_expression = new FormatStringExpression(this->embedded_strings.at(str_id));
                    return format_string_expression;
                }
                break;
            case TokenType::NUMBER:
                {
                    char str_id = tokens.bytes[this->idx++];
                    auto number_expression = new NumberExpression(this->embedded_strings.at(str_id));
                    return number_expression;
                }
                break;
            default:
                std::cout << "Failed at primary: " << Scanner::getTokenName(tmp) << std::endl;
                exit(-1);
        }

        return NULL;
    }

    Statement *TokensToTree::parse_statement(parse_result tokens) {
        TokenType tmp = static_cast<TokenType>(tokens.bytes[this->idx]);
        this->idx += BYTES_PER_INSTRUCTION;

        switch (tmp) {
            case TokenType::IF:
                {
                    auto if_statement = new IfStatement();

                    tmp = static_cast<TokenType>(tokens.bytes[this->idx]);
                    this->idx += BYTES_PER_INSTRUCTION;
                    assert(tmp == TokenType::LEFT_PARENTHESIS);

                    auto condition = this->parse_expression(tokens);
                    if_statement->set_condition(condition);

                    tmp = static_cast<TokenType>(tokens.bytes[this->idx]);
                    this->idx += BYTES_PER_INSTRUCTION;
                    assert(tmp == TokenType::RIGHT_PARENTHESIS);

                    if_statement->set_then(
                        this->parse_block(tokens)
                    );

                    return if_statement;
                }
                break;
            case TokenType::RETURN:
                {
                    auto expression = this->parse_expression(tokens);

                    tmp = static_cast<TokenType>(tokens.bytes[this->idx]);
                    this->idx += BYTES_PER_INSTRUCTION;
                    assert(tmp == TokenType::SEMICOLON);

                    return new ReturnStatement(expression);
                }
                break;
            case TokenType::IDENTIFIER:
                {
                    // This is dumb.
                    this->idx -= BYTES_PER_INSTRUCTION;
                    auto expression = this->parse_expression(tokens);

                    TokenType tmp = static_cast<TokenType>(tokens.bytes[this->idx]);
                    this->idx += BYTES_PER_INSTRUCTION;
                    assert(tmp == TokenType::SEMICOLON);

                    return new ExpressionStatement(expression);
                }
                break;
            default:
                {
                    std::cout << "Bad statement start" << std::endl;
                    this->idx += BYTES_PER_INSTRUCTION;
                }
                break;
        }

        return NULL;
    }

    Statement *TokensToTree::parse_block(parse_result tokens) {
        TokenType tmp = static_cast<TokenType>(tokens.bytes[this->idx]);
        if (tmp != TokenType::LEFT_CURLY) {
            return this->parse_statement(tokens);
        }
        this->idx += BYTES_PER_INSTRUCTION;;

        auto block_statement = new BlockStatement();

        while (true) {
            // Note: We do not advance here.
            tmp = static_cast<TokenType>(tokens.bytes[this->idx]);
            if (tmp == TokenType::RIGHT_CURLY) {
                this->idx += BYTES_PER_INSTRUCTION;
                return block_statement;
            }

            block_statement->append(
                this->parse_statement(tokens)
            ); 
        }
    }

    void TokensToTree::parse_function_arguments(parse_result tokens, FunctionStatement *function) {
        while (true) {
            TokenType tmp = static_cast<TokenType>(tokens.bytes[this->idx]);
            if (tmp == TokenType::RIGHT_PARENTHESIS) break;
            this->idx += BYTES_PER_INSTRUCTION;

            assert(tmp == TokenType::U64);

            DataType dt;
            dt.type = MetaType::MT_U64;

            tmp = static_cast<TokenType>(tokens.bytes[this->idx]);
            this->idx += BYTES_PER_INSTRUCTION;
            assert(tmp == TokenType::COLON);

            tmp = static_cast<TokenType>(tokens.bytes[this->idx]);
            this->idx += BYTES_PER_INSTRUCTION;
            assert(tmp == TokenType::IDENTIFIER);

            char str_id = tokens.bytes[this->idx++];
            std::string arg_name = this->embedded_strings.at(str_id);

            function->add_arg(arg_name, dt);
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
                    std::string function_name = this->embedded_strings.at(str_id);

                    auto *function = new FunctionStatement(function_name);

                    tmp = static_cast<TokenType>(tokens.bytes[this->idx]);
                    this->idx += BYTES_PER_INSTRUCTION;
                    assert(tmp == TokenType::LEFT_PARENTHESIS);

                    this->parse_function_arguments(tokens, function);

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

                    switch (tmp) {
                        case TokenType::U64:
                            {
                                DataType dt;
                                dt.type = MT_U64;
                                function->set_return_type(dt);
                            }
                            break;
                        case TokenType::I64:
                            {
                                DataType dt;
                                dt.type = MT_I64;
                                function->set_return_type(dt);
                            }
                            break;
                    }

                    function->set_body(this->parse_block(tokens));

                    return function;
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

        std::cout << "digraph {\n" << std::endl;

        while (true) {
            TokenType tokenType = static_cast<TokenType>(tokens.bytes[this->idx]);
            if (tokenType == TokenType::TOKEN_EOF) break;

            Statement *statement = this->parse_top_level_statement(tokens);
            if (statement == NULL) {
                std::cout << "Top level statement was NULL" << std::endl;
            } else {
                std::cout << statement->toString() << std::endl;
            }
        }

        std::cout << "}\n" << std::endl;
    }
}
