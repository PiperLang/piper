#pragma once

#include <expressions/expression.hpp>

namespace piper {
    class GreaterThanExpression : public Expression {
        public:
            GreaterThanExpression(Expression *left, Expression *right);

            std::string toString();
        protected:
            Expression *left;
            Expression *right;
    };
}
