#pragma once

#include <expressions/expression.hpp>
#include <statements/statement.hpp>
#include <data_type.hpp>

#include <vector>

#include <vector>

namespace piper {
    class IfStatement : public Statement {
        public:
            IfStatement();

            void set_condition(Expression *condition);
            void set_then(Statement *then);

            std::string toString();
        protected:
            Expression *condition;
            Statement *then;
    };
}
