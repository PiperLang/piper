#pragma once

#include <expressions/expression.hpp>

namespace piper {
    class FormatStringExpression : public Expression {
        public:
            FormatStringExpression(std::string format_string);

            std::string toString();
        protected:
            std::string format_string;
    };
}
