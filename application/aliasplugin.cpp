#include "aliasplugin.h"

AliasPlugin::AliasPlugin()
    : Plugin("Aliases", Plugin::DisplayFormat_Tab)
{
    ui->setupUi(widget());
}

AliasPlugin::~AliasPlugin()
{

}

void AliasPlugin::setup(const QString source, const QString bashrcSource)
{
    this->bashrcSource = bashrcSource;
}

QString AliasPlugin::exec(const QString source, const QString bashrcSource)
{
}
