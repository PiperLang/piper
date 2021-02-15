#pragma once

#include <string>

namespace piper {
    class Statement {
        public:
            virtual std::string toString() = 0;
    };
}
