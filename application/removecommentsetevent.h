#ifndef REMOVECOMMENTSETEVENT_H
#define REMOVECOMMENTSETEVENT_H

#include "event.h"
#include "buffer.h"

class RemoveCommentSetEvent : public SetEvent
{
public:
    RemoveCommentSetEvent();
    virtual QString process(QString source);
};

#endif // REMOVECOMMENTSETEVENT_H
