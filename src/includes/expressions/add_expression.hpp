#pragma once

#include <expressions/expression.hpp>

namespace piper {
    class AddExpression : public Expression {
        public:
            AddExpression(Expression *left, Expression *right);

            std::string toString();
        protected:
            Expression *left;
            Expression *right;
    };
}
