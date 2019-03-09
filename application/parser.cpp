#include "parser.h"
#include "config.h"
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QMessageBox>
Parser::Parser()
{
    QFile bashrc(QDir::homePath() + "/.bashrc");
    if(!bashrc.open(QFile::ReadOnly))
    {

    }
    QTextStream bashrcStream(&bashrc);
    this->bashrc = bashrcStream.readAll();
    bashrc.close();
}

QString Parser::removeComments(QString str)
{
    str = toStr(removeComments(toList(str)));
    return str;
}

QStringList Parser::removeComments(QStringList list)
{
    foreach (QString str, list)
    {
        str.remove(QRegExp("#[\\s\\d\\w]{0,}"));
    }
    return list;
}

QList<Alias> Parser::getAliases()
{
    QList<Alias> rtn;
    foreach(QString str, toList(bashrc))
    {
        while(true)
        {
            QRegExp aliasRegex("[\\s]{0,}alias[\\s]+[\\w]+=\"[\\d\\D\\w\\W\\S\\s]{0,}\"[\\s]{0,}");
            if(str.contains(aliasRegex))
            {
                Alias alias;
                int start = str.indexOf(aliasRegex);
                start += 6; //get rid of alias_
                alias.alias = str.section('=', 0);
                alias.alias.remove(QRegExp("[\\s]+"));
                alias.command = str.section("=\"", 1);
                while(true)
                {
                    if(alias.command.endsWith('\"'))
                    {
                        alias.command.chop(1);
                        break;
                    }
                    else
                    {
                        alias.command.chop(1);
                    }
                }
                rtn << alias;
            }else break;
        }
    }
    return rtn;
}

QStringList Parser::toList(QString str)
{
    return str.split('\n');
}

QString Parser::toStr(QStringList list)
{
    QString str;
    foreach (QString i, list) {
        str += i + "\n";
    }
    return str;
}
