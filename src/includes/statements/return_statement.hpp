#pragma once

#include <expressions/expression.hpp>
#include <statements/statement.hpp>

namespace piper {
    class ReturnStatement : public Statement {
        public:
            ReturnStatement(Expression *expression);

            std::string toString();
        protected:
            Expression *expression;
    };
}
