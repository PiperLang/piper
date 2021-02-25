#include <expressions/call_expression.hpp>

#include <iomanip>
#include <sstream>

namespace piper {
    CallExpression::CallExpression(Expression *left) {
        this->left = left;
    }

    void CallExpression::add_argument(Expression *argument) {
        this->arguments.push_back(argument);
    }

    std::string CallExpression::toString() {
        std::stringstream stream;
        stream << "\"" << std::hex << this << "\"";
        stream << " [label=\"Call Expression\"];\n";

        stream << "\"" << std::hex << this << "\"";
        stream << " -> ";
        stream << "\"" << std::hex << this->left << "\"";
        stream << " [ label = \"Base\" ];\n";
        stream << this->left->toString();

        for (int i = 0; i < this->arguments.size(); i++) {
            auto argument = this->arguments.at(i);

            stream << "\"" << std::hex << this << "\"";
            stream << " -> ";
            stream << "\"" << std::hex << argument << "\"";
            stream << " [ label = \"";
            stream << std::dec << i;
            stream << "\"];\n";
            stream << argument->toString();
        }

        return stream.str();
    }
}
