#pragma once

#include <expressions/expression.hpp>

namespace piper {
    class DoubleColonExpression : public Expression {
        public:
            DoubleColonExpression(Expression *left, Expression *right);

            std::string toString();
        protected:
            Expression *left;
            Expression *right;
    };
}
