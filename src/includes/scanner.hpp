#pragma once

#include <string>
#include <vector>

#include <tokentype.hpp>

namespace piper {
    struct Location {
        int line;
        int column;
        std::string filename;
    };

    struct Token {
        TokenType type;
        Location start;
        Location end;
        std::string str;
    };

    class Scanner {
        public:
            Scanner(std::string filename);

            std::vector<Token *> *getTokens();
            static std::string getTokenName(TokenType tokenType);
        protected:
        private:
            std::string filename;

    };
}
