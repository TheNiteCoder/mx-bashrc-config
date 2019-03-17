#include "bashrcparser.h"
#include "config.h"
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QMessageBox>
#include <algorithm>
BashrcParser::BashrcParser(QWidget *win)
    : window(win)
{
    QFile bashrc(QDir::homePath() + "/.bashrc");
    if(!bashrc.open(QFile::ReadOnly))
    {
        QMessageBox::warning(window, tr("Warning - ") + tr(APPNAME), "Unable to open " + bashrc.fileName() + "for reading", QMessageBox::Ok);
    }
    QTextStream bashrcStream(&bashrc);
    this->bashrc = bashrcStream.readAll();
    bashrc.close();
}

QString BashrcParser::removeComments(QString str)
{
    str = toStr(removeComments(toList(str)));
    return str;
}

QStringList BashrcParser::removeComments(QStringList list)
{
    foreach (QString str, list)
    {
        str.remove(QRegExp("#[\\s\\d\\w]{0,}"));
    }
    return list;
}

QList<Alias> BashrcParser::getAliases()
{
    QList<Alias> rtn;
    foreach(QString str, toList(removeComments(bashrc)))
    {
        while(true)
        {
            if(str.contains("alias"))
            {
                //this breaks the loop that checks for aliases on this line
                //this is because if something doesn't work after the alias that fails
                //there is a high chance that things afterward will not work
#define TEST_FOR_BAD(i) ((i == -1) || !(i < str.length()))
                int aliasBegin = str.indexOf("alias");
                int aliasEnd = aliasBegin+5; //go to the end of alias
                if(TEST_FOR_BAD(aliasEnd)) break;
                QString::iterator iter = str.begin()+(aliasEnd);
                int keywordBegin = -1;
                for(int j = 0; iter < str.end(); j++)
                {
                    if(*iter == ' ' || *iter == '\n' || *iter == '\t') keywordBegin = aliasEnd+j;
                }
                if(TEST_FOR_BAD(keywordBegin)) break;
                int keywordEnd = str.indexOf('=',keywordBegin);
                keywordEnd -= 1; // so it is not on = but the char before
                if(TEST_FOR_BAD(keywordEnd)) break;
                int commandBegin = keywordEnd + 2; //goes to =["]
                if(!(str.at(commandBegin) == '\"')) break;
                commandBegin += 1; //move it again forward so it isn't on a "
                int commandEnd = str.indexOf('\"', commandBegin + 1);
                if(TEST_FOR_BAD(commandEnd)) break;
                commandEnd -= 1; //to move the position off the "
                Alias a(str.mid(keywordBegin, keywordEnd-keywordBegin), str.mid(commandBegin, commandEnd-commandBegin), aliasBegin);
                rtn << a;
                DEBUG << "Alias: " << a.alias;
                DEBUG << "Command: " << a.command;
#undef TEST_FOR_BAD
            }else break;
        }
    }
    return rtn;
}

int BashrcParser::setAlias(Alias a)
{
    QList<Alias> la = getAliases();
    foreach (Alias i, la)
    {
        if(i.alias == a.alias)
        {
            if(i.pos < 0)
            {
                //TODO error
                continue;
            }
            QRegExp aliasRegex("[\\s]{0,}alias[\\s]+[\\w]+=\"[\\d\\D\\w\\W\\S\\s]{0,}\"[\\s]{0,}");
            int start = i.pos;
            int end = bashrc.indexOf('\"', bashrc.indexOf('\"', i.pos));
            if(end == -1)
            {
                continue;
            }
            bashrc.replace(start, end-start, tr("alias ") + a.alias + tr("=\"") + a.command + "\"");
        }
    }

    return 0;
}

int BashrcParser::setAliases(QList<Alias> la)
{
    foreach (Alias a, la) {
        setAlias(a);
    }
    return 0;
}

Alias BashrcParser::getAlias(QString str, bool command)
{
    QList<Alias> la = getAliases();
    foreach (Alias a, la) {
        if(command)
        {
            if(a.command == str)
            {
                return a;
            }
        }
        else
        {
            if(a.alias == str)
            {
                return a;
            }
        }
    }
    return Alias("", "", -2);
}

QStringList BashrcParser::toList(QString str)
{
    return str.split('\n');
}

QString BashrcParser::toStr(QStringList list)
{
    QString str;
    foreach (QString i, list) {
        str += i + "\n";
    }
    return str;
}
