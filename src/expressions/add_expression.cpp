#include <expressions/add_expression.hpp>

#include <iomanip>
#include <sstream>


namespace piper {
    AddExpression::AddExpression(Expression *left, Expression *right) {
        this->left = left;
        this->right = right;
    }

    std::string AddExpression::toString() {
        std::stringstream stream;
        stream << "\"" << std::hex << this << "\"";
        stream << " [label=\"Add\"];\n";
        stream << "\"" << std::hex << this << "\"";
        stream << " -> ";
        stream << "\"" << std::hex << this->left << "\"";
        stream << ";\n";
        stream << this->left->toString();
        stream << "\"" << std::hex << this << "\"";
        stream << " -> ";
        stream << "\"" << std::hex << this->right << "\"";
        stream << ";\n";
        stream << this->right->toString();

        return stream.str();
    }
}
