#include <expressions/format_string_expression.hpp>

#include <iomanip>
#include <sstream>


namespace piper {
    FormatStringExpression::FormatStringExpression(std::string format_string) {
        this->format_string = format_string;
    }

    std::string FormatStringExpression::toString() {
        std::stringstream stream;
        stream << "\"" << std::hex << this << "\"";
        stream << " [label=\"Format String: ";
        stream << this->format_string;
        stream << "\"];\n";
        return stream.str();
    }
}
