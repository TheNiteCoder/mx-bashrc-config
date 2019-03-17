#ifndef PARSER_H
#define PARSER_H

#include <QObject>
#include "buffer.h"
#include "event.h"
#include <algorithm>

class Parser : public QObject
{
       Q_OBJECT
public:
    Parser(QString source, QObject *parent = 0);
    void doSetEvent(SetEvent* e);
    void doSetEvents(QList<SetEvent*> le);
    void* doGetEvent(GetEvent* e);
    QString source(){return __source;}
protected:
    QString __source;
signals:

public slots:
};

#endif // PARSER_H
