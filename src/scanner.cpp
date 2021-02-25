#include <scanner.hpp>

#include <fstream>
#include <iostream>
#include <map>
#include <vector>

using namespace piper;

struct SimpleTokenConfig {
    TokenType type;
};

std::map<TokenType, std::string> tokenToString = {
    { ASTERISK,             "ASTERISK (*)"              },
    { ASTERISK_ASTERISK,    "ASTERISK_ASTERISK (**)"    },
    { ASTERISK_EQUALS,      "ASTERISK_EQUALS (*=)"      },
    { BANG,                 "BANG"                      },
    { BANG_EQUALS,          "BANG_EQUALS"               },
    { BREAK,                "BREAK"                     },
    { CLASS,                "CLASS"                     },
    { COLON,                "COLON (:)"                 },
    { COLON_COLON,          "COLON_COLON (::)"          },
    { COMMA,                "COMMA (,)"                 },
    { CONTINUE,             "CONTINUE (continue)"       },
    { DO,                   "DO (do)"                   },
    { DOT,                  "DOT (.)"                   },
    { EQUALS,               "EQUALS (=)"                },
    { EQUALS_EQUALS,        "EQUALS_EQUALS (==)"        },
    { FALSE,                "FALSE (false)"             },
    { FAT_ARROW,            "FAT_ARROW (=>)"            },
    { FOR,                  "FOR (for)"                 },
    { FORMAT_STRING,        "FORMAT_STRING ()"          },
    { FORWARD_SLASH,        "FORWARD_SLASH (/)"         },
    { FORWARD_SLASH_EQUALS, "FORWARD_SLASH_EQUALS (/=)" },
    { FUNCTION,             "FUNCTION (function)"       },
    { GLOBAL,               "GLOBAL (global)"           },
    { GREATER_EQUALS,       "GREATER_EQUALS (>=)"       },
    { HASH,                 "HASH (#)"                  },
    { HIDDEN,               "HIDDEN (hidden)"           },
    { I64,                  "I64 (i64)"                 },
    { IDENTIFIER,           "IDENTIFIER ()"             },
    { IF,                   "IF (if)"                   },
    { IMPORT,               "IMPORT (import)"           },
    { INTERROBANG,          "INTERROBANG (?!)"          },
    { LEFT_ANGLE,           "LEFT_ANGLE (<)"            },
    { LEFT_CURLY,           "LEFT_CURLY ({)"            },
    { LEFT_PARENTHESIS,     "LEFT_PARENTHESIS (()"      },
    { LEFT_SQUARE,          "LEFT_SQUARE ([)"           },
    { LESS_EQUALS,          "LESS_EQUALS (<=)"          },
    { LET,                  "LET (let)"                 },
    { MINUS,                "MINUS (-)"                 },
    { MINUS_EQUALS,         "MINUS_EQUALS (-=)"         },
    { MINUS_MINUS,          "MINUS_MINUS (--)"          },
    { MODULE,               "MODULE (module)"           },
    { NUMBER,               "NUMBER ()"                 },
    { PERCENT,              "PERCENT (%)"               },
    { PERCENT_EQUALS,       "PERCENT_EQUALS (%=)"       },
    { PIPE,                 "PIPE (|)"                  },
    { PLUS,                 "PLUS (+)"                  },
    { PLUS_PLUS,            "PLUS_PLUS (++)"            },
    { PLUS_EQUALS,          "PLUS_EQUALS (+=)"          },
    { PROTECTED,            "PROTECTED (protected)"     },
    { PRIVATE,              "PRIVATE (private)"         },
    { PUBLIC,               "PUBLIC (public)"           },
    { QUESTION,             "QUESTION (?)"              },
    { QUESTION_COLON,       "QUESTIN_COLON (?:)"        },
    { RETURN,               "RETURN (return)"           },
    { RIGHT_ANGLE,          "RIGHT_ANGLE (>)"           },
    { RIGHT_CURLY,          "RIGHT_CURLY (})"           },
    { RIGHT_PARENTHESIS,    "RIGHT_PARENTHESIS ())"     },
    { RIGHT_SQUARE,         "RIGHT_SQUARE (])"          },
    { SEMICOLON,            "SEMICOLON (;)"             },
    { SKINNY_ARROW,         "SKINNY_ARROW (->)"         },
    { STRING,               "STRING ()"                 },
    { STRUCT,               "STRUCT (struct)"           },
    { TILDE,                "TILDE (~)"                 },
    { TOKEN_EOF,            "EOF ()"                    },
    { TRUE,                 "TRUE (true)"               },
    { U64,                  "U64 (u64)"                 },
    { VAR,                  "VAR (var)"                 },
    { VOID,                 "VOID (void)"               },
    { WHILE,                "WHILE (while)"             },
};

std::map<std::string, SimpleTokenConfig> keywordTokens = {
    { "false",     { TokenType::FALSE } },
    { "function",  { TokenType::FUNCTION } },
    { "global",    { TokenType::GLOBAL } },
    { "hidden",    { TokenType::HIDDEN} },
    { "i64",       { TokenType::I64 } },
    { "if",        { TokenType::IF } },
    { "import",    { TokenType::IMPORT } },
    { "let",       { TokenType::LET } },
    { "module",    { TokenType::MODULE } },
    { "private",   { TokenType::PRIVATE } },
    { "protected", { TokenType::PROTECTED } },
    { "public",    { TokenType::PUBLIC} },
    { "return",    { TokenType::RETURN } },
    { "struct",    { TokenType::STRUCT } },
    { "true",      { TokenType::TRUE } },
    { "u64",       { TokenType::U64 } },
    { "var",       { TokenType::VAR } },
    { "void",      { TokenType::VOID } },
};

std::map<char, SimpleTokenConfig> simpleTokens = {
    { '{', { TokenType::LEFT_CURLY } },
    { '}', { TokenType::RIGHT_CURLY } },
    { '(', { TokenType::LEFT_PARENTHESIS } },
    { ')', { TokenType::RIGHT_PARENTHESIS } },
    { '.', { TokenType::DOT } }, // This is very likely to become more in the future.
    { ';', { TokenType::SEMICOLON } },
    { '[', { TokenType::LEFT_SQUARE } },
    { ']', { TokenType::RIGHT_SQUARE } },
    { ',', { TokenType::COMMA } },
    { '~', { TokenType::TILDE } },
    { '|', { TokenType::PIPE } },
    { '#', { TokenType::HASH } },
};

std::map<char, std::pair<SimpleTokenConfig, std::map<char, SimpleTokenConfig>>> doubleTokens = {
    {
        '+', { { TokenType::PLUS }, {
            { '+', { TokenType::PLUS_PLUS } },
            { '=', { TokenType::PLUS_EQUALS } },
        }}
    },
    {
        '*', { { TokenType::ASTERISK }, {
            { '*', { TokenType::ASTERISK_ASTERISK } },
            { '=', { TokenType::ASTERISK_EQUALS } },
        }}
    },
    {
        '-', { { TokenType::MINUS }, {
            { '>', { TokenType::SKINNY_ARROW } },
            { '-', { TokenType::MINUS_MINUS } },
            { '=', { TokenType::MINUS_EQUALS } },
        }}
    },
    {
        '=', { { TokenType::EQUALS }, {
            { '>', { TokenType::FAT_ARROW } },
            { '=', { TokenType::EQUALS_EQUALS } },
        }}
    },
    {
        ':', { { TokenType::COLON }, {
            { ':', { TokenType::COLON_COLON } },
        }}
    },
    {
        '/', { { TokenType::FORWARD_SLASH }, {
            { '=', { TokenType::FORWARD_SLASH_EQUALS } },
        }}
    },
    {
        '!', { { TokenType::BANG }, {
            { '=', { TokenType::BANG_EQUALS } },
        }}
    },
    {
        '%', { { TokenType::PERCENT }, {
            { '=', { TokenType::PERCENT_EQUALS } },
        }}
    },
    {
        '<', { { TokenType::LEFT_ANGLE }, {
            { '=', { TokenType::LESS_EQUALS } },
        }}
    },
    {
        '>', { { TokenType::RIGHT_ANGLE }, {
            { '=', { TokenType::GREATER_EQUALS } },
        }}
    },
    {
        '?', { { TokenType::QUESTION }, {
            { ':', { TokenType::QUESTION_COLON } },
            { '!', { TokenType::INTERROBANG } },
        }}
    }
};

namespace piper {
    Scanner::Scanner(std::string filename) {
        this->filename = filename;
    }

    std::string Scanner::getTokenName(TokenType tokenType) {
        if (tokenToString.find(tokenType) != tokenToString.end()) {
            return tokenToString[tokenType];
        }

        return "UNKNOWN";
    }

    std::vector<Token *> *Scanner::getTokens() {
        std::ifstream file;
        file.open(filename);

        int line = 1;
        int column = 1;

        auto tokens = new std::vector<Token *>();

        Location start_location;
        start_location.filename = filename;
        Location end_location;
        end_location.filename = filename;

        while (true) {
            if (file.eof()) {
                break;
            }

            char c = file.peek();
            if (c == EOF) {
                break;
            }

            // Whitespace outside of a token. Adjust some internal counters,
            // but don't REALLY do anything with them.
            if (c == '\n') {
                line++;
                column = 1;
                file.get();
                continue;
            }
            
            if (isspace(c) != 0) {
                column++;
                file.get();
                continue;
            }

            if (isalpha(c) != 0) {
                std::string buffer;

                start_location.line = line;
                start_location.column = column;

                while (true) {
                    buffer += c;
                    file.get();
                    column++;

                    c = file.peek();
                    if (isalnum(c) == 0 && c != '_') {
                        end_location.line = line;
                        end_location.column = column;
                        break;
                    }
                }

                auto t = new Token();
                t->start = start_location;
                t->end = end_location;
                t->str = buffer;

                if (keywordTokens.find(buffer) != keywordTokens.end()) {
                    t->type = keywordTokens.find(buffer)->second.type;
                } else {
                    t->type = TokenType::IDENTIFIER;
                }
                tokens->push_back(t);
                continue;
            }

            // TODO: Support floating points.
            if (isdigit(c) != 0) {
                std::string buffer;

                start_location.line = line;
                start_location.column = column;

                while (true) {
                    buffer += c;
                    file.get();
                    column++;

                    c = file.peek();
                    if (isdigit(c) == 0) {
                        end_location.line = line;
                        end_location.column = column;
                        break;
                    }
                }

                auto t = new Token();
                t->start = start_location;
                t->end = end_location;
                t->str = buffer;
                t->type = TokenType::NUMBER;
                tokens->push_back(t);
                continue;
            }

            // TODO: For the time being this is 100% the same as a double-quoted string. I need to do something special for this.
            if (c == '`') {
                std::string buffer;

                start_location.line = line;
                start_location.column = column;

                file.get(); column++;
                c = file.peek();

                while (c != '`') {
                    if (c == EOF) {
                        // This should never happen with good code.
                        break;
                    }

                    if (c == '\\') {
                        file.get(); column++;
                        c = file.peek();
                        buffer += c;
                        file.get(); column++;
                        c = file.peek();
                        continue;
                    } else {
                        buffer += c;
                        file.get(); column++;
                        c = file.peek();
                    }
                }

                end_location.line = line;
                end_location.column = column;

                file.get(); column++;
                c = file.peek();

                Token *t = new Token();
                t->start = start_location;
                t->end = end_location;
                t->type = TokenType::FORMAT_STRING;
                t->str = buffer;
                tokens->push_back(t);
                continue;
            }

            if (c == '"') {
                std::string buffer;

                start_location.line = line;
                start_location.column = column;

                file.get(); column++;
                c = file.peek();

                while (c != '"') {
                    if (c == EOF) {
                        // This should never happen with good code.
                        break;
                    }

                    if (c == '\\') {
                        file.get(); column++;
                        c = file.peek();
                        buffer += c;
                        file.get(); column++;
                        c = file.peek();
                        continue;
                    } else {
                        buffer += c;
                        file.get(); column++;
                        c = file.peek();
                    }
                }

                end_location.line = line;
                end_location.column = column;

                file.get(); column++;
                c = file.peek();

                Token *t = new Token();
                t->start = start_location;
                t->end = end_location;
                t->type = TokenType::STRING;
                t->str = buffer;
                tokens->push_back(t);
                continue;
            }

            if (c == '\'') {
                std::string buffer;

                start_location.line = line;
                start_location.column = column;

                file.get(); column++;
                c = file.peek();

                while (c != '\'') {
                    if (c == EOF) {
                        // This should never happen with good code.
                        break;
                    }

                    if (c == '\\') {
                        file.get(); column++;
                        c = file.peek();
                        buffer += c;
                        file.get(); column++;
                        c = file.peek();
                        continue;
                    } else {
                        buffer += c;
                        file.get(); column++;
                        c = file.peek();
                    }
                }

                end_location.line = line;
                end_location.column = column;

                file.get(); column++;
                c = file.peek();

                Token *t = new Token();
                t->start = start_location;
                t->end = end_location;
                t->type = TokenType::STRING;
                t->str = buffer;
                tokens->push_back(t);
                continue;
            }

            if (doubleTokens.find(c) != doubleTokens.end()) {
                start_location.line = line;
                start_location.column = column;

                std::string buffer;
                buffer += c;
                column++;
                file.get();

                auto def = doubleTokens.find(c)->second.first;
                auto possibilities = doubleTokens.find(c)->second.second;

                c = file.peek();

                if (possibilities.find(c) != possibilities.end()) {
                    column++;
                    file.get();

                    end_location.line = line;
                    end_location.column = column;

                    buffer += c;

                    auto t = new Token();
                    t->start = start_location;
                    t->end = end_location;
                    t->type = possibilities.find(c)->second.type;
                    t->str = buffer;
                    tokens->push_back(t);
                    continue;
                } else {
                    end_location.line = line;
                    end_location.column = column;

                    auto t = new Token();
                    t->start = start_location;
                    t->end = end_location;
                    t->type = def.type;
                    t->str = buffer;
                    tokens->push_back(t);
                    continue;
                }
            }

            if (simpleTokens.find(c) != simpleTokens.end()) {
                start_location.line = line;
                start_location.column = column;

                std::string buffer;
                buffer += c;
                column++;
                file.get();

                end_location.line = line;
                end_location.column = column;

                auto t = new Token();
                t->start = start_location;
                t->end = end_location;
                t->type = simpleTokens.find(c)->second.type;
                t->str = buffer;
                tokens->push_back(t);
                continue;
            }

            std::cout << "UNKNOWN: " << c << std::endl;
            file.get();
            column++;
        }

        auto t = new Token();
        t->start = start_location;
        t->end = end_location;
        t->type = TokenType::TOKEN_EOF;
        t->str = "<EOF>";
        tokens->push_back(t);

        return tokens;
    }
}