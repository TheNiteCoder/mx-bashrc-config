#ifndef SAVEPLUGIN_H
#define SAVEPLUGIN_H

#include <QWidget>
#include "plugin.h"

namespace Ui {
class SavePlugin;
}

class SavePlugin : public Plugin
{
    Q_OBJECT

public:
    explicit SavePlugin();
    virtual QString exec(const QString source){return source;}
    ~SavePlugin();
private:
    Ui::SavePlugin *ui;
protected slots:
    void restore();
};

#endif // SAVEPLUGIN_H
