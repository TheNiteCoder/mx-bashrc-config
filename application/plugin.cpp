#include "plugin.h"

Plugin::Plugin(QString pluginName, QObject *parent)
    : QObject(parent), __pluginName(pluginName)//, __widget(new QWidget)
{

}
