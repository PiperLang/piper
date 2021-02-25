#include <expressions/sub_expression.hpp>

#include <iomanip>
#include <sstream>

namespace piper {
    SubExpression::SubExpression(Expression *left, Expression *right) {
        this->left = left;
        this->right = right;
    }

    std::string SubExpression::toString() {
        std::stringstream stream;

        stream << "\"" << std::hex << this << "\"";
        stream << " [label=\"Sub\"];\n";
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
