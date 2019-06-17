#include "buffer.h"
#include "global.h"

Buffer::Buffer()
    : QObject(nullptr)
{
    DEBUG_ENTER(Buffer::Buffer);
    DEBUG_EXIT(Buffer::Buffer);
}

Buffer::Buffer(const QString source)
    : QObject(nullptr)
{
    DEBUG_ENTER(Buffer::Buffer);
    setSource(source);
    DEBUG_EXIT(Buffer::Buffer);
}

Buffer::Buffer(Buffer &copy)
    : QObject(copy.parent())
{
    DEBUG_ENTER(Buffer::Buffer);
    *this = copy;
    DEBUG_EXIT(Buffer::Buffer);
}

Buffer::Buffer(Buffer &&move)
    : QObject(move.parent())
{
    DEBUG_ENTER(Buffer::Buffer);
    *this = move;
    DEBUG_EXIT(Buffer::Buffer);
}

Buffer &Buffer::operator =(Buffer &copy)
{
    DEBUG_ENTER(Buffer::operator=);
    if(this == &copy)
    {
        DEBUG_EXIT(Buffer::operator=);
        return *this;
    }
    setSource(copy.source());
    DEBUG_EXIT(Buffer::operator=);
    return *this;
}

Buffer &Buffer::operator =(Buffer &&move)
{
    DEBUG_ENTER(Buffer::operator=);
    if(this == &move)
    {
        DEBUG_EXIT(Buffer::operator=);
        return *this;
    }
    setSource(move.source());
    move.setSource(QString());
    DEBUG_EXIT(Buffer::operator=);
    return *this;
}

Buffer &Buffer::setSource(const QString source)
{
    DEBUG_ENTER(Buffer::setSource);
    m_source = source;
    DEBUG_EXIT(Buffer::setSource);
    return *this;
}

QString Buffer::source()
{
    DEBUG_ENTER(Buffer::source);
    DEBUG_EXIT(Buffer::source);
    return m_source;
}

Buffer &Buffer::addState(Buffer::State *state)
{
    DEBUG_ENTER(Buffer::addState);
    m_states.push_back(state);
    DEBUG_EXIT(Buffer::addState);
    return *this;
}

Buffer &Buffer::addStates(QList<Buffer::State *> states)
{
    DEBUG_ENTER(Buffer::addStates);
    for(State* state : states)
    {
        m_states.push_back(state);
    }
    DEBUG_EXIT(Buffer::addStates);
    return *this;
}

Buffer &Buffer::move(int times)
{
    DEBUG_ENTER(Buffer::move);
    if(times > 0)
    {
        for(int i = 0; i < times; i++)
        {
            m_sourceiter++;
            if(m_sourceiter == m_source.end())
            {
                DEBUG_EXIT(Buffer::move);
                return *this;
            }
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
            if(m_sourceiter == m_source.end())
            {
                DEBUG_EXIT(Buffer::move);
                return *this;
            }
        }
    }
    DEBUG_EXIT(Buffer::move);
    return *this;
}

QString Buffer::buffer()
{
    DEBUG_ENTER(Buffer::buffer);
    DEBUG_EXIT(Buffer::buffer);
    return m_source.mid(m_sourceiter - m_source.begin());
}

Buffer::State::State()
{
    DEBUG_ENTER(State::State);
    DEBUG_EXIT(State::State);
}

Buffer::State::State(const QString search, int state)
{
    DEBUG_ENTER(State::State);
    setSearchString(search);
    setState(state);
    DEBUG_EXIT(State::State);
}

Buffer::State::State(State& copy)
{
    DEBUG_ENTER(State::State);
    *this = copy;
    DEBUG_EXIT(State::State);
}

Buffer::State::State(State&& move)
{
    DEBUG_ENTER(State::State);
    *this = move;
    DEBUG_EXIT(State::State);
}

Buffer::State::~State()
{
    DEBUG_ENTER(State::~State);
    DEBUG_EXIT(State::~State);
}

Buffer::State& Buffer::State::operator =(Buffer::State &copy)
{
    DEBUG_ENTER(State::operator=);
    if(this == &copy)
    {
        DEBUG_EXIT(State::operator=);
        return *this;
    }
    setSearchString(copy.searchString());
    setState(copy.state());
    DEBUG_EXIT(State::operator=);
    return *this;
}

Buffer::State& Buffer::State::operator =(Buffer::State &&move)
{
    DEBUG_ENTER(State::operator=);
    if(this == &move)
    {
        DEBUG_EXIT(State::operator=);
        return *this;
    }
    setSearchString(move.searchString());
    move.setSearchString(QString());
    setState(move.state());
    move.setState(0);
    DEBUG_EXIT(State::operator=);
    return *this;
}

QString Buffer::State::searchString()
{
    DEBUG_ENTER(State::searchString);
    DEBUG_EXIT(State::searchString);
    return m_search;
}

Buffer::State &Buffer::State::setSearchString(const QString search)
{
    DEBUG_ENTER(State::setSearchString);
    m_search = search;
    DEBUG_EXIT(State::setSearchString);
    return *this;
}

int Buffer::State::state()
{
    DEBUG_ENTER(State::state);
    DEBUG_EXIT(State::state);
    return m_state;
}

Buffer::State &Buffer::State::setState(int state)
{
    DEBUG_ENTER(State::setState);
    m_state = state;
    DEBUG_EXIT(State::setState);
    return *this;
}
