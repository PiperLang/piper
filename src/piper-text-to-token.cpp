#include <fstream>
#include <iostream>
#include <string>

#include <string.h>

#include <scanner.hpp>
#include <stage_filetotokens.hpp>

void parse_file(std::string filename) {
    std::map<std::string /* pass_name */, piper::parse_result> results;

    auto stage = new piper::FileToTokens(filename);
    stage->run(&results);

    auto r = results["tokens"];
    for (int i = 0; i < r.output_size; i++) {
        std::cout << r.bytes[i];
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
}