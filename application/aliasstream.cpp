#include "aliasstream.h"

#include <QRegularExpression>
#include <algorithm>
#include "global.h"


AliasStream::AliasStream(QString *str, bool isOfBashrc)
{
    SCOPE_TRACKER;
    m_source = str;
    m_isOfBashrc = isOfBashrc;
}

AliasStream &AliasStream::operator <<(const Alias &alias)
{
    SCOPE_TRACKER;
    if(m_source == nullptr)
        return *this;
    QString templateAliasRegex("(alias) (%1)=(\"|')(%2)(\\3)");
    QString templateAliasText("alias %1='%2'");
    QList<Alias> aliases;
    *this >> aliases;
    bool resolved = false;
    for(Alias a : aliases)
    {
        if(a.alias() != alias.alias())
            continue;
        if(a.command() == alias.command())
        {
            resolved = true;
            break;
        }
        m_source->replace(QRegularExpression(templateAliasRegex.arg(a.alias()).arg(a.command())), templateAliasText.arg(alias.alias()).arg(alias.command()));
        resolved = true;
        break;
    }
    if(!resolved)
    {
        m_source->append(QObject::tr("\n") + templateAliasText.arg(alias.alias()).arg(alias.command()));
    }
    return *this;
}

AliasStream &AliasStream::operator <<(const QList<Alias> &aliases)
{
    SCOPE_TRACKER;
    if(m_source == nullptr)
        return *this;
    for(Alias alias : aliases)
    {
        *this << alias;
    }
    return *this;
}

AliasStream &AliasStream::operator >>(QList<Alias> &aliases)
{
    SCOPE_TRACKER;
    if(m_source == nullptr)
        return *this;
    QList<Alias> rtn;
    QRegularExpression regex("(alias) ([\\w-]+)=(\"|')([^\\n]{0,})(\\3)");
    auto iter = regex.globalMatch(*m_source);
    while(iter.hasNext())
    {
        auto match = iter.next();
        Alias alias(match.captured(2), match.captured(4));
        alias.setStart(match.capturedStart());
        alias.setEnd(match.capturedEnd());
        alias.setLength(match.capturedLength());
        alias.setInBashrc(m_isOfBashrc);
        /*
        if(match.captured(3).contains('"'))
            alias.surrondedInDoubleQuotes = true;
        else
            alias.surrondedInDoubleQuotes = false;
         */
        rtn.append(alias);
    }
    aliases = rtn;
    return *this;
}

QList<Alias> AliasStream::get()
{
    SCOPE_TRACKER;
    QList<Alias> rtn;
    *this >> rtn;
    return rtn;
}

void AliasStream::set(const Alias &alias)
{
    SCOPE_TRACKER;
    *this << alias;
}

void AliasStream::set(const QList<Alias> &alias)
{
    SCOPE_TRACKER;
    *this << alias;
}

void AliasStream::remove(const Alias &alias)
{
    SCOPE_TRACKER;
    QString templateAliasRegex("(alias) (%1)=(\"|')(%2)(\\3)");
    QString templateAliasText("alias %1='%2'");
#define E(i) escapeRegexCharacters(i)
    if(m_source->contains(QRegularExpression(templateAliasRegex.arg(E(alias.alias())).arg(E(alias.command())))))
    {
        m_source->remove(QRegularExpression(templateAliasRegex.arg(E(alias.alias())).arg(E(alias.command()))));
        DEBUG << "Did detect and remove alias" << alias.alias();
    }
#undef E
}

void AliasStream::remove(const QList<Alias> &alias)
{
    SCOPE_TRACKER;
    for(auto a : alias)
        remove(a);
}


QString *AliasStream::source() const
{
    SCOPE_TRACKER;
    return m_source;
}

void AliasStream::setSource(QString *source)
{
    SCOPE_TRACKER;
    m_source = source;
}

bool AliasStream::getIsOfBashrc() const
{
    SCOPE_TRACKER;
    return m_isOfBashrc;
}

void AliasStream::setIsOfBashrc(bool isOfBashrc)
{
    SCOPE_TRACKER;
    m_isOfBashrc = isOfBashrc;
}

Alias::Alias()
{
    SCOPE_TRACKER;
}

Alias::Alias(QString alias, QString command)
{
    SCOPE_TRACKER;
    m_alias = alias;
    m_command = command;
}

bool Alias::operator ==(const Alias &other)
{
    SCOPE_TRACKER;
    return m_alias == other.m_alias && m_command == other.m_command;
}

bool Alias::operator !=(const Alias &other)
{
    SCOPE_TRACKER;
    return !(*this == other);
}

QString Alias::command() const
{
    SCOPE_TRACKER;
    return m_command;
}

void Alias::setCommand(const QString &command)
{
    SCOPE_TRACKER;
    m_command = command;
}

QString Alias::alias() const
{
    SCOPE_TRACKER;
    return m_alias;
}

void Alias::setAlias(const QString &alias)
{
    SCOPE_TRACKER;
    m_alias = alias;
}

int Alias::start() const
{
    SCOPE_TRACKER;
    return m_start;
}

void Alias::setStart(int start)
{
    SCOPE_TRACKER;
    m_start = start;
}

int Alias::end() const
{
    SCOPE_TRACKER;
    return m_end;
}

void Alias::setEnd(int end)
{
    SCOPE_TRACKER;
    m_end = end;
}

int Alias::length() const
{
    SCOPE_TRACKER;
    return m_length;
}

void Alias::setLength(int length)
{
    SCOPE_TRACKER;
    m_length = length;
}

bool Alias::inBashrc() const
{
    SCOPE_TRACKER;
    return m_inBashrc;
}

void Alias::setInBashrc(bool inBashrc)
{
    SCOPE_TRACKER;
    m_inBashrc = inBashrc;
}
