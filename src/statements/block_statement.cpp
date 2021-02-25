#include <statements/block_statement.hpp>

#include <iostream>

#include <iomanip>
#include <sstream>

namespace piper {
    BlockStatement::BlockStatement() {
    }

    void BlockStatement::append(Statement *statement) {
        if (statement != NULL) {
            this->statements.push_back(statement);
        }
    }

    std::string BlockStatement::toString() {
        std::stringstream stream;

        stream << "\"" << std::hex << this << "\"";
        stream << " [label=\"Block\"];\n";

        for (int i = 0; i < this->statements.size(); i++) {
            auto statement = this->statements.at(i);

            stream << "\"" << std::hex << this << "\"";
            stream << " -> ";
            stream << "\"" << std::hex << statement << "\"";
            stream << " [ label = \"";
            stream << std::dec << i;
            stream << "\" ];\n";

            stream << statement->toString();
        }

        return stream.str();
    }
}