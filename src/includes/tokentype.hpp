#pragma once

#include <map>

namespace piper {
    /**
     * Token types that piper recognizes. Note that the names here are
     * purposefully not indicative of *use*. Some of these tokens change
     * meaning based on context. Rather than having a two-tier system where
     * some tokens are use-named and some are appearance-named, I am choosing
     * to have everything appearance-named at this stage. Later stages, where
     * symbols have been disambiguated can be more specific.
     */
    enum TokenType {
        ASTERISK,                       // *
        ASTERISK_ASTERISK,              // **
        ASTERISK_EQUALS,                // *=
        BANG,                           // !
        BANG_EQUALS,                    // !=
        BREAK,                          // break
        CLASS,                          // class
        COLON,                          // :
        COLON_COLON,                    // ::
        COMMA,                          // ,
        CONTINUE,                       // continue
        DO,                             // do
        DOT,                            // .
        EQUALS,                         // =
        EQUALS_EQUALS,                  // ==
        FALSE,                          // false
        FAT_ARROW,                      // =>
        FOR,                            // for
        FORMAT_STRING,                  // `some string`
        FORWARD_SLASH,                  // /
        FORWARD_SLASH_EQUALS,           // /=
        FUNCTION,                       // function
        GREATER_EQUALS,                 // >=
        HASH,                           // #
        I64,                            // i64
        IDENTIFIER,                     // something
        IF,                             // if
        IMPORT,                         // import
        INTERROBANG,                    // ?!
        LEFT_ANGLE,                     // <
        LEFT_CURLY,                     // {
        LEFT_PARENTHESIS,               // (
        LEFT_SQUARE,                    // [
        LESS_EQUALS,                    // <=
        LET,                            // let
        MINUS,                          // -
        MINUS_EQUALS,                   // -=
        MINUS_MINUS,                    // --
        MODULE,                         // module
        NUMBER,                         // 6
        PERCENT,                        // %
        PERCENT_EQUALS,                 // %=
        PIPE,                           // |
        PLUS,                           // +
        PLUS_PLUS,                      // ++
        PLUS_EQUALS,                    // +=
        QUESTION,                       // ?
        QUESTION_COLON,                 // ?:
        RETURN,                         // return
        RIGHT_ANGLE,                    // >
        RIGHT_CURLY,                    // }
        RIGHT_PARENTHESIS,              // )
        RIGHT_SQUARE,                   // ]
        SEMICOLON,                      // ;
        SKINNY_ARROW,                   // ->
        STRING,                         // "string" or 'string'
        STRUCT,                         // struct
        TILDE,                          // ~
        TOKEN_EOF,                      // <No Visual Representation>
        TRUE,                           // true
        U64,                            // u64
        VAR,                            // var
        VOID,                           // void
        WHILE,                          // while
    };
}
