#ifndef GLOBAL_H
#define GLOBAL_H

#include <QDebug>

struct BashrcSource
{
    QString bashrc;
    QString program;
};

#define DEBUG qDebug()
#define DEBUG_VAR(x) qDebug() << #x << " = " << x

#endif // GLOBAL_H
