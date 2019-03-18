#ifndef PLUGIN_H
#define PLUGIN_H

#include <QObject>
#include <QWidget>
#include <QVector>
#include <QMap>
class Plugin : public QObject
{
    Q_OBJECT
public:
    class GuiOptionData
    {
    public:
        void* retrieveData(QString idStr)
        {
            int id = __idMap[idStr];
            foreach (GuiOption* opt, __guiOptions) {
                if(opt->id() == id)
                {
                    return opt->value();
                }
            }
            return nullptr;
        }
        void addOption(QString idStr, GuiOption* opt)
        {
            __idMap[idStr] = opt->id();
        }

    protected:
        QMap<QString/*Id str*/, int/*id*/> __idMap;
        QVector<GuiOption*> __guiOptions;
    };

    class GuiOption : public QObject
    {
        Q_OBJECT
    public:
        virtual void* value();
        QWidget* widget(){return __widget;}
        int id(){return __id;}
    protected:
        GuiOption(QWidget* widget, QObject* parent = nullptr)
            : __widget(widget), QObject(parent)
        {
        }
        QWidget* __widget;
        int __id;
        friend class GuiOptionData;
    };

    explicit Plugin(QObject *parent = nullptr);
protected:

signals:

public slots:
};

#endif // PLUGIN_H
