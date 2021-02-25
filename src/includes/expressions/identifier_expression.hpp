#pragma once

#include <expressions/expression.hpp>

namespace piper {
    class IdentifierExpression : public Expression {
        public:
            IdentifierExpression(std::string identifier_name);

            std::string toString();
        protected:
            std::string identifier_name;
    };
}
