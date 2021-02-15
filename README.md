# piper
The main repo for piper. Pulls in many other repositories. Start here.

## building

    mkdir build
    cd build
    cmake ..
    make
    ./piper ../example.piper --dump-tokens --out-tokens example.piper.bin0

## stages

The compiler works in small logical stages. This is mostly for my own sanity, but should also make each stage easier to work on. It may also make the compiler run more slowly, but frankly I don't care.

### stage 0: tokenization

This stage takes textual code in and outputs a stream of tokens, mostly. It does do a few special things with interning strings and string-like things.

The output is a binary file, with the following structure. Every component is 8 bits.

#### File Structure

##### VERSION ID

This is always zero for now. I will start updating this field when things are more stable and they do not shift literally every commit.

##### NUMBER OF STRINGS

The number of embedded strings. All strings and string-like things are interned and from here on in will be referred to as just their string ID (the index into this quasi-array, starting at zero)

##### FOR EACH STRING: LENGTH OF STRING

The length of the following string. It is NOT null terminated.

##### FOR EACH STRING: BYTES

For now this is only guaranteed to work with ASCII. UTF-8 encoding is on the horizon. Again, this is not null terminated.

##### LIST OF TOKENS

Each token is its own byte. There are (currently) four tokens that are followed by a string id to represent the additional info: FORMAT_STRING, STRING, IDENTIFIER, NUMBER.

Note that format strings at this point are not internally parsed and are just strings. Also note that numbers themselves are not parsed and are just the string version of themselves.