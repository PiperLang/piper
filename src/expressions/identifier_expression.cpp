#include <expressions/identifier_expression.hpp>

#include <iomanip>
#include <sstream>

namespace piper {
    IdentifierExpression::IdentifierExpression(std::string identifier_name) {
        this->identifier_name = identifier_name;
    }

    std::string IdentifierExpression::toString() {
        std::stringstream stream;
        stream << "\"" << std::hex << this << "\"";
        stream << " [ label = \"{ <f0> Identifier: ";
        stream << this->identifier_name;
        stream << "| <f1> Data Type: UNKNOWN }\"; shape = \"record\" ];\n";
        return stream.str();
    }
}
