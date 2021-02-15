#include <fstream>
#include <iostream>
#include <string>

#include <string.h>

#include <scanner.hpp>
#include <stage_filetotokens.hpp>
#include <stage_tokenstotree.hpp>

// For each stage:
//     a) Do we run this stage?
//     b) Do we write this out to a file at all?
//     c) What stages are we passing this info on to?
//         *) We could just store the output and let the future stages grab.

void parse_file(std::string filename) {
    std::map<std::string /* pass_name */, piper::parse_result> results;

    std::vector<piper::Stage *> stages = {
        new piper::FileToTokens(filename),
        new piper::TokensToTree(filename)
    };

    for (auto stage: stages) {
        if (stage->shouldRun()) {
            stage->run(&results);
        }
    }
}

int main(int argc, char *argv[]) {
    std::string my_name = argv[0];

    if (argc <= 1) {
        std::cout << "Usage: " << my_name << " filename" << std::endl;
        return 0;
    }

    std::string filename = argv[1];
    parse_file(filename);

    std::cout << "Max Tokens: " << piper::TokenType::TOKEN_MAX << std::endl;
}