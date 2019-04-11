#ifndef RESTOREPLUGIN_H
#define RESTOREPLUGIN_H

#include "plugin.h"

#include <QPushButton>

class RestorePlugin : public Plugin
{
//Q_OBJECT //is this neccary?
public:
    explicit RestorePlugin();
    virtual ~RestorePlugin();
    virtual QString exec(const QString source, const QString bashrcSource) override;
public slots:
    void restore();
protected:
    QPushButton* button;
};

#endif // RESTOREPLUGIN_H
