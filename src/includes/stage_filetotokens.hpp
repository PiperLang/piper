#pragma once

#include <stage.hpp>

namespace piper {
    class FileToTokens : public Stage {
        public:
            FileToTokens(std::string filename);

            bool shouldRun();
            void run(
                std::map<std::string, parse_result> *results
            );
        protected:
            bool debug;
            std::string filename;
        private:
    };
}
