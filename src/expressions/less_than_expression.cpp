#include <expressions/less_than_expression.hpp>

#include <iomanip>
#include <sstream>

namespace piper {
    LessThanExpression::LessThanExpression(Expression *left, Expression *right) {
        this->left = left;
        this->right = right;
    }

    std::string LessThanExpression::toString() {
        std::stringstream stream;
        stream << "\"" << std::hex << this << "\"";
        stream << " [label=\"Less Than\"];\n";
        stream << "\"" << std::hex << this << "\"";
        stream << " -> ";
        stream << "\"" << std::hex << this->left << "\"";
        stream << ";\n";
        stream << "\"" << std::hex << this << "\"";
        stream << " -> ";
        stream << "\"" << std::hex << this->right << "\"";
        stream << ";\n";
        stream << this->left->toString();
        stream << this->right->toString();
        return stream.str();
    }
}
