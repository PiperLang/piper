#pragma once

#include <statements/statement.hpp>
#include <data_type.hpp>

#include <vector>

#include <vector>

namespace piper {
    class BlockStatement : public Statement {
        public:
            BlockStatement();

            void append(Statement *statement);

            std::string toString();
        protected:
            std::vector<Statement *> statements;
    };
}
