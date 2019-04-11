#ifndef ALIASPLUGIN_H
#define ALIASPLUGIN_H

#include "plugin.h"
#include "ui_aliasplugin.h"

class AliasPlugin : public Plugin
{
public:
    AliasPlugin();
    ~AliasPlugin();
protected:
    virtual void setup(const QString source, const QString bashrcSource) override;
    virtual QString exec(const QString source, const QString bashrcSource) override;
private:
    QString bashrcSource;
    Ui::AliasPlugin* ui;
};

#endif // ALIASPLUGIN_H
