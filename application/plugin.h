#ifndef PLUGIN_H
#define PLUGIN_H

#include <QObject>
#include <QWidget>
#include <QString>
#include <QIcon>
class Plugin : public QObject
{
    Q_OBJECT
public:
    virtual QString exec(const QString source)=0;
    explicit Plugin(QString pluginName = QString(), QObject *parent = nullptr);
    QString name(){return __pluginName;}
    QWidget* widget(){return __widget;}
    void setWidget(QWidget* widget){__widget = widget;}
protected:
    QString __pluginName;
    QWidget* __widget;
signals:

public slots:
};

#endif // PLUGIN_H
