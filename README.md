# piper
The main repo for piper. Pulls in many other repositories. Start here.

## stages

The compiler works in small logical stages. This is mostly for my own sanity, but should also make each stage easier to work on. It may also make the compiler run more slowly, but frankly I don't care.

### stage 0: tokenization

This stage takes textual code in and outputs a stream of tokens, mostly. It does do a few special things with interning strings and string-like things.

The output is a binary file, with the following structure. Every component is 8 bits.

Version ID: This is always zero for now. I will likely start incrementing this when I start claiming things are ready to use.
Number of Strings: These are interned strings that will be used once here in the header, and thereafter referred to by id.

For each string:
Length of string.
Bytes (this does NOT include a null byte)

List of tokens. Most tokens are just the byte that represents the token, but a few are special:

FORMAT_STRING is followed by the string id of the format string itself. At this point the format string is not internally parsed, it is just a string.
STRING is followed by the string id of the string itself.
IDENTIFIER is followed by the string id of the name of the identifier.
NUMBER is followed by the string id of the stringified number. At this point we are treating numbers as strings. We do not know if they are floating point, integers, or what. That comes later.

The last token SHOULD be a TOKEN_EOF.
