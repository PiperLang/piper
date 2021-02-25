#include <expressions/number_expression.hpp>

#include <iomanip>
#include <sstream>

namespace piper {
    NumberExpression::NumberExpression(std::string number) {
        this->number = number;
    }

    std::string NumberExpression::toString() {
        std::stringstream stream;
        stream << "\"" << std::hex << this << "\"";
        stream << " [label=\"Number: ";
        stream << this->number;
        stream << "\"];\n";
        return stream.str();
    }
}
