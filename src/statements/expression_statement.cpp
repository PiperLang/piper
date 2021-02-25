#include <statements/expression_statement.hpp>

#include <iomanip>
#include <sstream>

namespace piper {
    ExpressionStatement::ExpressionStatement(Expression *expression) {
        this->expression = expression;
    }

    std::string ExpressionStatement::toString() {
        std::stringstream stream;
        
        stream << "\"" << std::hex << this << "\"";
        stream << " [label=\"Expression Statement\"];\n";
        stream << "\"" << std::hex << this << "\"";
        stream << " -> ";
        stream << "\"" << std::hex << this->expression << "\"";
        stream << ";\n";
        stream << this->expression->toString();

        return stream.str();
    }
}