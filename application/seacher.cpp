#include "seacher.h"

Searcher::Searcher()
{
}

Searcher::Searcher(Searcher::byte_t bitmask)
    : m_bitmask(bitmask)
{

}

int Searcher::test(const QString source, const QString lookFor)
{
    if(!source.contains(lookFor)) return ReturnSubstringNotFound;
    int start = source.indexOf(lookFor);
    int end = start + lookFor.length();
    Buffer buf(source);
    buf.move(start);
    if(m_bitmask & CheckForDoubleQuotes)
        buf.addState(Buffer::State("q1", {"\""},  0));
    if(m_bitmask & CheckForSingleQuotes)
        buf.addState(Buffer::State("q2", {"\'"}, 0));
    if(m_bitmask & CheckForSpecialQuotes)
        buf.addState(Buffer::State("q3", {"`"}, 0));
    for(int i = start; i <= end; i++)
    {
        buf.move(i);
    }
    if(m_bitmask & CheckForDoubleQuotes)
        if(!(buf.state("q1").state() % 2)) return ReturnOptionFailed;
    if(m_bitmask & CheckForSingleQuotes)
        if(!(buf.state("q2").state() % 2)) return ReturnOptionFailed;
    if(m_bitmask & CheckForSpecialQuotes)
        if(!(buf.state("q3").state() % 2)) return ReturnOptionFailed;
    else
        return start;
}

