#pragma once

#include <expressions/expression.hpp>

namespace piper {
    class NumberExpression : public Expression {
        public:
            NumberExpression(std::string number);

            std::string toString();
        protected:
            std::string number;
    };
}
