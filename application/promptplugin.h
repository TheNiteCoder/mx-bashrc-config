#ifndef PROMPTPLUGIN_H
#define PROMPTPLUGIN_H

#include "plugin.h"
#include "ui_promptplugin.h"

class PromptPlugin : public Plugin
{
public:
    PromptPlugin();
    ~PromptPlugin();
    QString exec(const QString source, const QString bashrcSource) override;
    void setup(const QString source, const QString bashrcSource) override;
protected slots:

private:
    Ui::PromptPlugin* ui;


};

#endif // PROMPTPLUGIN_H
