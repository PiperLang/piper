#include <statements/function_statement.hpp>

#include <iomanip>
#include <sstream>

namespace piper {
    FunctionStatement::FunctionStatement(std::string function_name) {
        this->function_name = function_name;
    }

    void FunctionStatement::set_return_type(DataType return_type) {
        this->return_type = return_type;
    }

    void FunctionStatement::set_body(Statement *body) {
        this->body = body;
    }

    void FunctionStatement::add_arg(std::string arg_name, DataType data_type) {
        this->args.push_back({arg_name, data_type});
    }

    std::string FunctionStatement::toString() {
        std::stringstream stream;

        stream << "subgraph \"cluster_" << std::hex << this << "\" {\n";
            stream << "label = \"" << this->function_name << "\";\n";

            stream << "\"" << std::hex << this << "\" [ label = \"Start\" ];\n";

            stream << "\"" << std::hex << this << "\"";
            stream << " -> ";
            stream << "\"" << std::hex << this->body << "\"";
            stream << " [label=\"Body\"];\n";

            stream << "subgraph \"cluster_" << std::hex << this << "_args\" {\n";
            stream << "label = \"Args\";\n";

            for (int i = 0; i < this->args.size(); i++) {
                auto arg = this->args.at(i);

                stream << "\"" << std::hex << this << "_arg" << std::dec << i << "\"";
                stream << " [ label = \"{";
                stream << " <f0> " << arg.first;
                stream << " | <f1> ";

                switch (arg.second.type) {
                    case MT_I64:
                        stream << "i64";
                        break;
                    case MT_U64:
                        stream << "u64";
                        break;
                    case MT_CUSTOM:
                        stream << arg.second.extra_data;
                        break;
                    case MT_UNKNOWN:
                    default:
                        stream << "UNKNOWN";
                        break;
                }
                arg.second;
                stream << " }\"; shape = \"record\"; ];\n";
            }

            stream << "}\n";

            stream << this->body->toString();
        
        stream << "}\n";

        return stream.str();
    }
}