#ifndef TOKENIZED_HPP
#define TOKENIZED_HPP

#include "Utility/includes.hpp"
#include "Utility/filetokenizer.hpp"

class Tokenized
{
public:
    virtual bool fillFromTokenizer ( FileTokenizer& theFile ) = 0;
};

#endif // TOKENIZED_HPP

