#pragma once

#include <string>

namespace piper {
    class Expression {
        public:
            virtual std::string toString() = 0;
    };
}
