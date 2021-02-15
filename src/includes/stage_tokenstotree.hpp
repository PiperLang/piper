#pragma once

#include <stage.hpp>
#include <statements/statement.hpp>

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
            void term_expression(parse_result results);
            void factor_expression(parse_result results);
            void unary_expression(parse_result results);
            void call_expression(parse_result results);
            void primary_expression(parse_result results);

            // General
            void parse_function_arguments(parse_result results);
            void parse_expression(parse_result results);
            void parse_statement(parse_result results);
            void parse_block(parse_result results);
            void parse_strings(parse_result results);
            Statement *parse_top_level_statement(parse_result results);
        private:
    };
}
