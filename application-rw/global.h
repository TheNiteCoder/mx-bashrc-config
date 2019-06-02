#ifndef GLOBAL_H
#define GLOBAL_H

#include <QDebug>
#include "searcher.h"

struct BashrcSource
{
    QString bashrc;
    QString program;
};

#define DEBUG qDebug()
#define DEBUG_VAR(x) qDebug() << #x << " = " << x
#define DEBUG_POS qDebug() << __FILE__ << ":" << __LINE__
#define CHECK_SEARCH(x) ((x != Searcher::ReturnValueSearchStatesFailed) && (x != Searcher::ReturnValueSearchStringNotFound))

#endif // GLOBAL_H
