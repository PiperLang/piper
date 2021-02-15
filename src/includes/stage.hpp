#pragma once

#include <fstream>
#include <map>
#include <string>

namespace piper {
    typedef struct parse_result_t {
        uint16_t output_size;
        char *bytes;
    } parse_result;

    class Stage {
        public:
            // TODO: I need to pass in some things so that the stages can make
            //       a good decision. Flags and previous stage outputs,
            //       probably.
            virtual bool shouldRun() = 0;

            // TODO: Need to pass in compiler flags and the output of previous
            //       stages. Writable so I can write the output of my stage.
            virtual void run(
                std::map<std::string, parse_result> *previous_results
            ) = 0;

            void write_file(parse_result result, std::string out_filename) {
                std::ofstream fout;
                fout.open(out_filename, std::ios::binary | std::ios::out);
                fout.write(result.bytes, sizeof(char) * result.output_size);
                fout.close();
            }
        protected:
        private:
    };
}
