#ifndef PLUGIN_H
#define PLUGIN_H

#include <QObject>
#include <QWidget>
#include <QString>
#include <QIcon>
#include "config.h"
class Plugin : public QObject
{
    Q_OBJECT
public:
    enum DisplayFormat
    {
        DisplayFormat_Tab = 1,
        DisplayFormat_Section = 2,
        DisplayFormat_Single = 3,
        DisplayFormat_None = 4,
    };
    virtual QString exec(const QString source, const QString bashrcSource){return source;}
    virtual void setup(const QString source, const QString bashrcSource){Q_UNUSED(source);Q_UNUSED(bashrcSource);}
    explicit Plugin(QString pluginName, DisplayFormat format = DisplayFormat_Section, QStringList tabNames = QStringList(), QObject *parent = nullptr);
    virtual ~Plugin();
    QString name()const{return __pluginName;}
    QWidget* widget()const{return __widget;}
    void setWidget(QWidget* widget){__widget = widget;}
    DisplayFormat displayFormat()const{return __displayFormat;}
    void setDisplayFormat(DisplayFormat format){__displayFormat = format;}
    QStringList requestedTabNames(){return __tabNames;}
protected:
    QString __pluginName;
    QWidget* __widget;
    DisplayFormat __displayFormat;
    QStringList __tabNames;
signals:
    void requestReloadWidgets();
public slots:
};

#endif // PLUGIN_H
