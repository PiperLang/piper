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
     *
     * Many things were stolen from https://llvm.org/docs/LangRef.html#module-structure
     * Not all of these things will be used, probably, this list stands as a note to myself to some degree.
     */
    enum TokenType {
        ALIGN,                          // align
        ASTERISK,                       // *
        ASTERISK_ASTERISK,              // **
        ASTERISK_EQUALS,                // *=
        BANG,                           // !
        BANG_EQUALS,                    // !=
        BREAK,                          // break
        BYREF,                          // byref
        BYVAL,                          // byval
        CLASS,                          // class
        COLD,                           // cold
        COLON,                          // :
        COLON_COLON,                    // ::
        COMMA,                          // ,
        CONTINUE,                       // continue
        DO,                             // do
        DOT,                            // .
        EQUALS,                         // =
        EQUALS_EQUALS,                  // ==
        EXTERNAL,                       // external
        FALSE,                          // false
        FAT_ARROW,                      // =>
        FOR,                            // for
        FORMAT_STRING,                  // `some string`
        FORWARD_SLASH,                  // /
        FORWARD_SLASH_EQUALS,           // /=
        FUNCTION,                       // function
        GLOBAL,                         // global
        GREATER_EQUALS,                 // >=
        HASH,                           // #
        HIDDEN,                         // hidden
        HOT,                            // hot
        I64,                            // i64
        IDENTIFIER,                     // something
        IF,                             // if
        IMPORT,                         // import
        INTERNAL,                       // internal
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
        MONOTONIC,                      // monotonic
        NOCAPTURE,                      // nocapture
        NOFREE,                         // nofree
        NONNULL,                        // nonnull
        NORECURSE,                      // norecurse
        NUMBER,                         // 6
        PERCENT,                        // %
        PERCENT_EQUALS,                 // %=
        PIPE,                           // |
        PLUS,                           // +
        PLUS_PLUS,                      // ++
        PLUS_EQUALS,                    // +=
        PREALLOCATED,                   // preallocated
        PRIVATE,                        // private
        PROTECTED,                      // protected
        PUBLIC,                         // public
        QUESTION,                       // ?
        QUESTION_COLON,                 // ?:
        READONLY,                       // readonly
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
        UNORDERED,                      // unordered
        VAR,                            // var
        VOID,                           // void
        WEAK,                           // weak
        WHILE,                          // while
        WRITEONLY,                      // writeonly
        TOKEN_MAX
    };
}
