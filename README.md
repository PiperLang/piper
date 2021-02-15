# piper
The main repo for piper. Potentially pulls in many other repositories. Start here.

## checking out

    git clone https://github.com/PiperLang/piper.git

## building

    mkdir build
    cd build
    cmake ..
    make
    cp ../example.piper ./
    ./piper example.piper
    xxd ./example.piper.fft

## stages

The compiler works in small logical stages. This is mostly for my own sanity, but should also make each stage easier to work on. It may also make the compiler run more slowly, but frankly I don't care. During development / when flags are given, each stage will also output its intermediary files. Sometimes these files wil be easy to read, sometimes you will need to know their binary structure. xxd comes in handy, as it will show you the hex and also strings if those are embedded in the binary.

### Stage 0: tokenization : ftt (file to tokens)

This stage takes textual code in and outputs a stream of tokens, mostly. It does do a few special things with interning strings and string-like things.

The output is a binary file, with the following structure. Every component is 8 bits.

#### File Structure

##### MAGIC NUMBER

The magic number is ASCII for PPRALCFTT. In hex that is 50 50 52 41 4c 43 46 54 54.

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

### Stage 1: tree : ttt (tokens to tree)

This stage takes a list of tokens and constructs a tree. Most of our syntax errors will occur here, as this is the first stage that will know that variable names must be valid identifiers, etc. There's a LOT of invalid programs that will pass the tokenization stage.

The output is again a binary file, this time though it's fundamentally a tree structure. The top level structure is a file, and there's a branch node for every top level statement in the file itself.

#### File Structure

##### Magic Number

The magic number is ASCII for PPRALCTTT. In hex that is 50 50 52 41 9c 43 54 54 54.

##### VERSION ID

This is always zero for now. I will start updating this field when things are more stable and they do not shift literally every commit.

