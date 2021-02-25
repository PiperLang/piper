#pragma once

#include <expressions/expression.hpp>

#include <vector>

namespace piper {
    class CallExpression : public Expression {
        public:
            CallExpression(Expression *left);

            void add_argument(Expression *argument);

            std::string toString();
        protected:
            Expression *left;
            std::vector<Expression *> arguments;
    };
}
