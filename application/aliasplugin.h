#ifndef ALIASPLUGIN_H
#define ALIASPLUGIN_H

#include "plugin.h"
#include "ui_aliasplugin.h"

#include <QTextStream>

class AliasPlugin : public Plugin
{
public:
    struct Alias
    {
    public:
        Alias(QString keyword, QString command)
            : aliasKeyword(keyword), aliasCommand(command)
        {
        }
        Alias()
        {
        }
        QString keyword() const{return aliasKeyword;}
        QString command() const{return aliasCommand;}
        void setKeyword(QString keyword){aliasKeyword = keyword;}
        void setCommand(QString command){aliasCommand = command;}
    protected:
        QString aliasKeyword;
        QString aliasCommand;
    };
    AliasPlugin();
    ~AliasPlugin();
    virtual void setup(const QString source, const QString bashrcSource) override;
    virtual QString exec(const QString source, const QString bashrcSource) override;
    typedef QList<Alias> AliasList;
protected:
    AliasList aliases(QString source);
private:
    Ui::AliasPlugin* ui;
};

#endif // ALIASPLUGIN_H
