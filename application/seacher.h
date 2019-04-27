#ifndef SEACHER_H
#define SEACHER_H

#include "buffer.h"

class Searcher
{
public:

    typedef unsigned char byte_t;
    static const byte_t CheckForDoubleQuotes = 1 << 0;
    static const byte_t CheckForSingleQuotes = 1 << 1;
    static const byte_t CheckForSpecialQuotes = 1 << 2;

    typedef int return_t;
    static const return_t ReturnOptionFailed = -1;
    static const return_t ReturnSubstringNotFound = -2;

    Searcher();
    Searcher(byte_t bitmask);
    int test(const QString source, const QString lookFor);
    byte_t options(){return m_bitmask;}
    Searcher& setOptions(byte_t bitmask){m_bitmask = bitmask;}
protected:
    byte_t m_bitmask;
};




#endif // SEACHER_H
