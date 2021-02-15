#pragma once

#include <statements/statement.hpp>

namespace piper {
    class ImportStatement : public Statement {
        public:
            ImportStatement(std::string import_name);

            std::string toString();
        protected:
            std::string import_name;
    };
}
