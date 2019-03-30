#include "saveplugin.h"
#include "ui_saveplugin.h"

SavePlugin::SavePlugin() :
    Plugin("Restore")
{
    widget() S= new Ui::SavePlugin;
    ui->setupUi(__widget);
}

SavePlugin::~SavePlugin()
{
    delete ui;
}

void SavePlugin::restore()
{
//TODO
}
