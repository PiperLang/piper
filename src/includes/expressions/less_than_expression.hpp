#pragma once

#include <expressions/expression.hpp>

namespace piper {
    class LessThanExpression : public Expression {
        public:
            LessThanExpression(Expression *left, Expression *right);

            std::string toString();
        protected:
            Expression *left;
            Expression *right;
    };
}
