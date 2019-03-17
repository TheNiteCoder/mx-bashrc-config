#include "buffer.h"

Buffer::Buffer(QString source, QObject *parent)
    : QObject(parent), __source(source)
{

}

QChar Buffer::moveUntilFindingAndGetChar(QChar c)
{
    while(iterator() != source().end())
    {
        if(currentChar() == c) return currentChar();
        move(1);
    }
    return QChar('\0');
}

QChar Buffer::moveUntilFindingAndGetChar(QString mc)
{
    while(iterator() != source().end())
    {
        if(std::count(mc.begin(), mc.end(), currentChar()) > 0) return currentChar();
        move(1);
    }
    return QChar('\0');
}
