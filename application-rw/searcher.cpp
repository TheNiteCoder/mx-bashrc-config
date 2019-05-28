#include "searcher.h"

#include <QRegExp>
#include <QRegularExpression>

#include "buffer.h"

Searcher::Searcher()
{

}

Searcher::Searcher(const QString source, const int states)
{
    setSource(source);
    setStates((SearchStates)states);
}

Searcher::Searcher(Searcher &copy)
    : QObject(copy.parent())
{
    *this = copy;
}

Searcher::Searcher(Searcher &&move)
    : QObject(move.parent())
{
    *this = move;
}

Searcher::~Searcher()
{
}

Searcher& Searcher::operator = (Searcher& copy)
{
    setSource(copy.source());
    return *this;
}

Searcher& Searcher::operator = (Searcher&& move)
{
    setSource(move.source());
    move.setSource(QString());
    return *this;
}

int Searcher::search(const QRegExp search, int from)
{
    return templateSearch(search, from);
}

int Searcher::search(const QRegularExpression search, int from)
{
    return templateSearch(search, from);
}

int Searcher::search(const QChar search, int from)
{
    return templateSearch(search, from);
}

int Searcher::search(const QString search, int from)
{
    return templateSearch(search, from);
}

Searcher &Searcher::setSource(const QString source)
{
    m_source = source;
    return *this;
}

QString Searcher::source()
{
    return m_source;
}

Searcher &Searcher::setStates(const int states)
{
    m_states = (Searcher::SearchStates)states;
    return *this;
}

Searcher::SearchStates Searcher::states()
{
    return m_states;
}
template<class Type>
int Searcher::templateSearch(Type search, int from)
{
    if(!source().contains(search))
    {
        return Searcher::ReturnValueSearchStringNotFound;
    }
    Buffer::State *q1, *q2, *q3;
    Buffer buffer = Buffer(source());
    if(states() & SearchStates::StateCheckDoubleQuotations)
    {
        q1 = new Buffer::State("\"");
        buffer.addState(q1);
    }
    if(states() & SearchStates::StateCheckSingleQuotations)
    {
        q2 = new Buffer::State("\'");
        buffer.addState(q2);
    }
    if(states() & SearchStates::StateCheckSpecialQuotations)
    {
        q3 = new Buffer::State("`");
        buffer.addState(q3);
    }
    buffer.move(source().indexOf(search, from));
    if(states() & SearchStates::StateCheckDoubleQuotations)
    {
        if(!q1->state() % 2) return ReturnValueSearchStatesFailed;
    }
    if(states() & SearchStates::StateCheckSingleQuotations)
    {
        if(!q2->state() % 2) return ReturnValueSearchStatesFailed;
    }
    if(states() & SearchStates::StateCheckSpecialQuotations)
    {
        if(!q3->state() % 2) return ReturnValueSearchStatesFailed;
    }
    return source().indexOf(search, from);
}
