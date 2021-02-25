#pragma once

#include <expressions/expression.hpp>

namespace piper {
    class SubExpression : public Expression {
        public:
            SubExpression(Expression *left, Expression *right);

            std::string toString();
        protected:
            Expression *left;
            Expression *right;
    };
}
