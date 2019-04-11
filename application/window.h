#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include "plugin.h"
#include "restoreplugin.h"
#include "promptplugin.h"

namespace Ui {
class Window;
}

class Window : public QWidget
{
    Q_OBJECT

public:
    explicit Window(QWidget *parent = 0);
    ~Window();
    class PluginManager
    {
    public:
        PluginManager(){}
        ~PluginManager()
        {
            foreach(Plugin* p, __plugins)
            {
                if(p != nullptr) delete p;
            }
        }

        void addPlugin(Plugin* plugin){__plugins.push_back(plugin);}
        QString execPlugins(const QString source, const QString bashrcSource)
        {
            QString copy = source;
            foreach(Plugin* p, __plugins)
            {
                copy = p->exec(copy, bashrcSource);
            }
            return copy;
        }
        QVector<Plugin*> plugins(){return __plugins;}
    protected:
        QVector<Plugin*> __plugins;
        QWidget __window;
    };
public slots:
    void apply();
private:
    Ui::Window *ui;
    PluginManager pluginManager;

};

#endif // WINDOW_H
