#include <statements/import_statement.hpp>

#include <iomanip>
#include <sstream>

namespace piper {
    ImportStatement::ImportStatement(std::string import_name) {
        this->import_name = import_name;
    }

    std::string ImportStatement::toString() {
        std::stringstream stream;

        stream << "\"" << std::hex << this << "\"";
        stream << " [label = \"ImportStatement\"];";

        return stream.str();
    }
}