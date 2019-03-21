#ifndef FANCYPROMPTPLUGIN_H
#define FANCYPROMPTPLUGIN_H

#include "plugin.h"
#include <QCheckBox>

class FancyPromptPlugin : public Plugin
{
public:
    FancyPromptPlugin();
    ~FancyPromptPlugin();

protected:
    virtual QString exec(const QString source);
    QCheckBox* test;
};

#endif // FANCYPROMPTPLUGIN_H
