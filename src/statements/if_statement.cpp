#include <statements/if_statement.hpp>

#include <iostream>

#include <iomanip>
#include <sstream>

namespace piper {
    IfStatement::IfStatement() {
    }

    void IfStatement::set_condition(Expression *condition) {
        this->condition = condition;
    }

    void IfStatement::set_then(Statement *then) {
        this->then = then;
    }

    std::string IfStatement::toString() {
        std::stringstream stream;

        stream << "\"" << std::hex << this << "\"";
        stream << " [label = \"If\"];\n";

        stream << "\"" << std::hex << this << "\"";
        stream << " -> ";
        stream << "\"" << std::hex << this->condition << "\"";
        stream << " [ label = \"Condition\" ];\n";

        stream << "\"" << std::hex << this << "\"";
        stream << " -> ";
        stream << "\"" << std::hex << this->then << "\"";
        stream << " [ label = \"Then\" ];\n";

        stream << this->condition->toString();
        stream << this->then->toString();

        return stream.str();
    }
}