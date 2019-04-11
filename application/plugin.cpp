#include "plugin.h"

Plugin::Plugin(QString pluginName, DisplayFormat format, QStringList tabNames, QObject *parent)
    : QObject(parent), __pluginName(pluginName), __displayFormat(format), __tabNames(tabNames)
{
    if(format != DisplayFormat_None) __widget = new QWidget;
    else{if(__widget)delete __widget;__widget = new QWidget;}
}

Plugin::~Plugin()
{
    delete __widget;
}
