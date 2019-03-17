#ifndef EVENT_H
#define EVENT_H
#include <QString>

struct SetEvent
{
    virtual QString process(QString source);
    virtual ~SetEvent();
};
struct GetEvent
{
    virtual void* process(QString source);
    virtual ~GetEvent();
};

#endif // EVENT_H
