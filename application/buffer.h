#ifndef BUFFER_H
#define BUFFER_H

#include <QObject>
#include <QVector>

class Buffer : public QObject
{
    Q_OBJECT
public:
    class State
    {
    public:
        State(){}
        State(QString name, QStringList triggers, int startState)
            : __triggers(triggers), __state(startState), __name(name)
        {
        }
        void test(QString test)
        {
            for(QString trigger : __triggers)
            {
                if(test.indexOf(trigger) == 0) setState(state()+1);
            }
        }
        int state(){return __state;}
        void setState(int newState){__state = newState;}
        QString name(){return __name;}
    protected:
        QStringList __triggers;
        int __state;
        QString __name;
    };

    Buffer(QString source, QObject *parent = 0);
    Buffer(){}
    void setSource(QString source){__source = source;}
    QString source(){return __source;}
    void move(int t){
        __iter+=t;
        for(State state : states())
        {
            state.test(buffer());
        }
    }
    QString::iterator iterator(){return __iter;}
    void setIterator(QString::iterator iter){__iter = iter;}

    QChar moveForwardAndGetChar(){move(1);return *iterator();}
    QChar moveBackwardAndGetChar(){move(-1);return *iterator();}
    QChar currentChar(){if(iterator()->isNull())return *iterator();}
    QString buffer(){return __source.mid(iterator()-__source.begin());}
    int pos(){return iterator()-__source.begin();}
    QChar moveUntilFindingAndGetChar(QChar c);
    QChar moveUntilFindingAndGetChar(QString mc);
    QVector<State> states(){return __states;}
    void addState(State state){__states.append(state);}
    State state(QString name)
    {
        for(State state : __states)
        {
            if(state.name() == name) return state;
        }
        return State();
    }

protected:
    QString __source;
    QString::iterator __iter;
    QVector<State> __states;
signals:

public slots:
};

#endif // BUFFER_H
