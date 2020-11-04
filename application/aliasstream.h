#ifndef ALIASSTREAM_H
#define ALIASSTREAM_H

#include <QString>

class AliasStream;
class AliasTab;

class Alias
{
public:
    Alias();
    Alias(QString alias, QString command);
    bool operator == (const Alias& other);
    bool operator != (const Alias& other);

    QString command() const;
    void setCommand(const QString &command);

    QString alias() const;
    void setAlias(const QString &alias);
    int start() const;
    void setStart(int start);
    int end() const;
    void setEnd(int end);
    int length() const;
    void setLength(int length);
    bool inBashrc() const;
    void setInBashrc(bool inBashrc);
protected:
    QString m_command, m_alias;
    int m_start = 0, m_end = 0, m_length = 0;
    bool m_inBashrc = false;
};

class AliasStream
{
public:
    AliasStream(QString* str = nullptr, bool isOfBashrc = false);
    AliasStream& operator << (const Alias& alias); //DEPRECATED
    AliasStream& operator << (const QList<Alias> &aliases); //DEPRECATED
    AliasStream& operator >> (QList<Alias> &aliases); //DEPRECATED
    QList<Alias> get();
    void set(const Alias& alias);
    void set(const QList<Alias>& alias);
    void remove(const Alias& alias);
    void remove(const QList<Alias>& alias);
    QString *source() const;
    void setSource(QString *source);
    bool getIsOfBashrc() const;
    void setIsOfBashrc(bool isOfBashrc);
protected:
    QString* m_source;
    bool m_isOfBashrc;
};

#endif // ALIASSTREAM_H
