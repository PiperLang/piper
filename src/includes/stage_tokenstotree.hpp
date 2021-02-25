#pragma once

#include <expressions/expression.hpp>
#include <stage.hpp>
#include <statements/statement.hpp>

#include <statements/function_statement.hpp>

#include <vector>

namespace piper {
    class TokensToTree : public Stage {
        public:
            TokensToTree(std::string filename);

            bool shouldRun();
            void run(
                std::map<std::string, parse_result> *results
            );
        protected:
            bool debug;
            std::string filename;
            int idx;
            std::vector<std::string> embedded_strings;

            // Expressions
            Expression *comparison_expression(parse_result results);
            Expression *term_expression(parse_result results);
            Expression *factor_expression(parse_result results);
            Expression *unary_expression(parse_result results);
            Expression *call_expression(parse_result results);
            Expression *primary_expression(parse_result results);

            // General
            void parse_function_arguments(parse_result results, FunctionStatement *function);
            Expression *parse_expression(parse_result results);
            Statement *parse_statement(parse_result results);
            Statement *parse_block(parse_result results);
            void parse_strings(parse_result results);
            Statement *parse_top_level_statement(parse_result results);
        private:
    };
}
