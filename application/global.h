#ifndef GLOBAL_H
#define GLOBAL_H

#include <QDebug>
#include "searcher.h"

struct BashrcSource
{
    QString bashrc;
    QString program;
};

#define NAME QString("MX Bashrc Config")
#define NAME_BIN QString("mx-bashrc-config")
#define ORG QString("MX-Linux")

#define DEBUG qDebug()
#define DEBUG_VAR(x) qDebug() << #x << " = " << x
#define DEBUG_POS qDebug() << __FILE__ << ":" << __LINE__
#define CHECK_SEARCH(x) ((x != Searcher::ReturnValueSearchStatesFailed) && (x != Searcher::ReturnValueSearchStringNotFound))
#define DEBUG_ENTER(x) //qDebug() << "+++ " << #x << " +++"
#define DEBUG_EXIT(x) //qDebug() << "--- " << #x << " ---"
#endif // GLOBAL_H
