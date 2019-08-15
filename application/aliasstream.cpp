#include "aliasstream.h"

#include <QRegularExpression>
#include <algorithm>
#include "global.h"


AliasStream::AliasStream(QString *str, bool isOfBashrc)
{
    m_source = str;
    m_isOfBashrc = isOfBashrc;
}

AliasStream &AliasStream::operator <<(const Alias &alias)
{
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
    QList<Alias> rtn;
    *this >> rtn;
    return rtn;
}

void AliasStream::set(const Alias &alias)
{
    *this << alias;
}

void AliasStream::set(const QList<Alias> &alias)
{
    *this << alias;
}

void AliasStream::remove(const Alias &alias)
{
    QString templateAliasRegex("(alias) (%1)=(\"|')(%2)(\\3)");
    QString templateAliasText("alias %1='%2'");
    if(m_source->contains(QRegularExpression(templateAliasRegex.arg(alias.alias()).arg(alias.command().replace('\\', "\\\\")))))
    {
        m_source->remove(QRegularExpression(templateAliasRegex.arg(alias.alias()).arg(alias.command().replace('\\', "\\\\"))));
        DEBUG << "Did detect and remove alias";
    }
}

void AliasStream::remove(const QList<Alias> &alias)
{
    for(auto a : alias)
        remove(a);
}


QString *AliasStream::source() const
{
    return m_source;
}

void AliasStream::setSource(QString *source)
{
    m_source = source;
}

bool AliasStream::getIsOfBashrc() const
{
    return m_isOfBashrc;
}

void AliasStream::setIsOfBashrc(bool isOfBashrc)
{
    m_isOfBashrc = isOfBashrc;
}

Alias::Alias()
{
}

Alias::Alias(const QString &alias, const QString &command)
{
    m_alias = alias;
    m_command = command;
}

Alias::Alias(const Alias &other)
{
    *this = other;
}

Alias &Alias::operator=(const Alias &other)
{
    m_alias = other.m_alias;
    m_command = other.m_command;
    m_start = other.m_start;
    m_end = other.m_end;
    m_inBashrc = other.m_inBashrc;
    m_length = other.m_length;
    return *this;
}

bool Alias::operator ==(const Alias &other)
{
    return m_alias == other.m_alias && m_command == other.m_command;
}

bool Alias::operator !=(const Alias &other)
{
    return !(*this == other);
}

QString Alias::command() const
{
    return m_command;
}

void Alias::setCommand(const QString &command)
{
    m_command = command;
}

QString Alias::alias() const
{
    return m_alias;
}

void Alias::setAlias(const QString &alias)
{
    m_alias = alias;
}

int Alias::start() const
{
    return m_start;
}

void Alias::setStart(int start)
{
    m_start = start;
}

int Alias::end() const
{
    return m_end;
}

void Alias::setEnd(int end)
{
    m_end = end;
}

int Alias::length() const
{
    return m_length;
}

void Alias::setLength(int length)
{
    m_length = length;
}

bool Alias::inBashrc() const
{
    return m_inBashrc;
}

void Alias::setInBashrc(bool inBashrc)
{
    m_inBashrc = inBashrc;
}
