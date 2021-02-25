#include <expressions/double_colon_expression.hpp>

#include <iomanip>
#include <sstream>


namespace piper {
    DoubleColonExpression::DoubleColonExpression(Expression *left, Expression *right) {
        this->left = left;
        this->right = right;
    }

    std::string DoubleColonExpression::toString() {
        std::stringstream stream;
        stream << "\"" << std::hex << this << "\"";
        stream << " [label=\"Double Colon\"];\n";
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
