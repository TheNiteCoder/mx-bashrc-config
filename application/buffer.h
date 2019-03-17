#ifndef BUFFER_H
#define BUFFER_H

#include <QObject>

class Buffer : public QObject
{
    Q_OBJECT
public:
    Buffer(QString source, QObject *parent = 0);
    Buffer(){}
    void setSource(QString source){__source = source;}
    QString source(){return __source;}
    void move(int t){__iter+=t;}
    QString::iterator iterator(){return __iter;}
    void setIterator(QString::iterator iter){__iter = iter;}

    QChar moveForwardAndGetChar(){move(1);return *iterator();}
    QChar moveBackwardAndGetChar(){move(-1);return *iterator();}
    QChar currentChar(){return *__iter;}
    QString buffer(){return __source.mid(iterator()-__source.begin());}
    QChar moveUntilFindingAndGetChar(QChar c);
    QChar moveUntilFindingAndGetChar(QString mc);

protected:
    QString __source;
    QString::iterator __iter;
signals:

public slots:
};

#endif // BUFFER_H
