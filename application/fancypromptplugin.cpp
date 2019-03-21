#include "fancypromptplugin.h"
#include <QVBoxLayout>

FancyPromptPlugin::FancyPromptPlugin()
    : Plugin("Fancy Prompt")
{
    widget()->setLayout(new QVBoxLayout);
    test = new QCheckBox("Test");
    widget()->layout()->addWidget(test);
}

FancyPromptPlugin::~FancyPromptPlugin()
{
    delete test;
}

QString FancyPromptPlugin::exec(const QString source)
{
    return source;
}
