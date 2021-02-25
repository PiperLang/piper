#pragma once

#include <expressions/expression.hpp>
#include <statements/statement.hpp>

namespace piper {
    class ExpressionStatement : public Statement {
        public:
            ExpressionStatement(Expression *expression);

            std::string toString();
        protected:
            Expression *expression;
    };
}
