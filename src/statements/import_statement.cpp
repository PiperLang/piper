#include <statements/import_statement.hpp>

namespace piper {
    ImportStatement::ImportStatement(std::string import_name) {
        this->import_name = import_name;
    }

    std::string ImportStatement::toString() {
        std::string buffer;
        buffer += "ImportStatement:";
        buffer += this->import_name;
        return buffer;
    }
}