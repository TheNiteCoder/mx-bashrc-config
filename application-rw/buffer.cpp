#include "buffer.h"

Buffer::Buffer()
    : QObject(nullptr)
{
}

Buffer::Buffer(const QString source)
    : QObject(nullptr)
{
    setSource(source);
}

Buffer::Buffer(Buffer &copy)
    : QObject(copy.parent())
{
    *this = copy;
}

Buffer::Buffer(Buffer &&move)
    : QObject(move.parent())
{
    *this = move;
}

Buffer &Buffer::operator =(Buffer &copy)
{
    if(this == &copy) return *this;
    setSource(copy.source());
    return *this;
}

Buffer &Buffer::operator =(Buffer &&move)
{
    if(this == &move) return *this;
    setSource(move.source());
    move.setSource(QString());
    return *this;
}

Buffer &Buffer::setSource(const QString source)
{
    m_source = source;
    return *this;
}

QString Buffer::source()
{
    return m_source;
}

Buffer &Buffer::addState(Buffer::State *state)
{
    m_states.push_back(state);
    return *this;
}

Buffer &Buffer::addStates(QList<Buffer::State *> states)
{
    for(State* state : states)
    {
        m_states.push_back(state);
    }
    return *this;
}

Buffer &Buffer::move(int times)
{
    if(times > 0)
    {
        for(int i = 0; i < times; i++)
        {
            m_sourceiter++;
            if(m_sourceiter == m_source.end()) return *this;
            for(State* state : m_states)
            {
                if(buffer().indexOf(state->searchString()) == 0)
                {

                }
            }
        }
    }
    else
    {
        for(int i = 0; i > times; i--)
        {
            m_sourceiter--;
            if(m_sourceiter == m_source.end()) return *this;
        }
    }
    return *this;
}

QString Buffer::buffer()
{
    return m_source.mid(m_sourceiter - m_source.begin());
}

Buffer::State::State()
{
}

Buffer::State::State(const QString search, int state)
{
    setSearchString(search);
    setState(state);
}

Buffer::State::State(State& copy)
{
    *this = copy;
}

Buffer::State::State(State&& move)
{
    *this = move;
}

Buffer::State::~State()
{
}

Buffer::State& Buffer::State::operator =(Buffer::State &copy)
{
    if(this == &copy) return *this;
    setSearchString(copy.searchString());
    setState(copy.state());
    return *this;
}

Buffer::State& Buffer::State::operator =(Buffer::State &&move)
{
    if(this == &move) return *this;
    setSearchString(move.searchString());
    move.setSearchString(QString());
    setState(move.state());
    move.setState(0);
    return *this;
}

QString Buffer::State::searchString()
{
    return m_search;
}

Buffer::State &Buffer::State::setSearchString(const QString search)
{
    m_search = search;
    return *this;
}

int Buffer::State::state()
{
    return m_state;
}

Buffer::State &Buffer::State::setState(int state)
{
    m_state = state;
    return *this;
}
