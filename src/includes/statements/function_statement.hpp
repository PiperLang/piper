#pragma once

#include <statements/statement.hpp>
#include <data_type.hpp>

#include <vector>

namespace piper {
    class FunctionStatement : public Statement {
        public:
            FunctionStatement(std::string function_name);

            void set_return_type(DataType return_type);
            void add_arg(std::string arg_name, DataType data_type);
            void set_body(Statement *body);

            std::string toString();
        protected:
            std::string function_name;
            DataType return_type;
            Statement *body;
            std::vector<std::pair<std::string, DataType>> args;
    };
}
