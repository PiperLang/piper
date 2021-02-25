#include <statements/return_statement.hpp>

#include <iomanip>
#include <sstream>

namespace piper {
    ReturnStatement::ReturnStatement(Expression *expression) {
        this->expression = expression;
    }

    std::string ReturnStatement::toString() {
        std::stringstream stream;

        stream << "\"" << std::hex << this << "\"";
        stream << " [label=\"Return Statement\"];\n";
        stream << "\"" << std::hex << this << "\"";
        stream << " -> ";
        stream << "\"" << std::hex << this->expression << "\"";
        stream << ";\n";

        stream << this->expression->toString();
        
        return stream.str();
    }
}