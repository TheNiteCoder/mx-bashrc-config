#include "removecommentsetevent.h"

RemoveCommentSetEvent::RemoveCommentSetEvent()
{

}

QString RemoveCommentSetEvent::process(QString source)
{
    QStringList lines = source.split('\n');
    foreach(QString line, lines)
    {
        Buffer buf(line);
        bool inStringDouble = false;
        bool inStringSingle = false;
        int commentPos = -1;
        while(buf.iterator() != buf.source().end())
        {
            QChar c = buf.moveForwardAndGetChar();
            if(c == "\"")
            {
                if(inStringDouble && !inStringSingle)
                {
                    inStringDouble = false;
                }
            }
            else if(c == "'")
            {
                if(inStringSingle && !inStringDouble)
                {
                    inStringSingle = false;
                }
            }
            else if(c == "#")
            {
                if(!inStringDouble && !inStringSingle)
                {
                    commentPos = buf.iterator() - buf.source().begin();
                    break;
                }
            }
            else if(c == "\\")
            {
                buf.move(1);
            }
        }
        if(commentPos != -1)
        {
            line.remove(commentPos, line.length() - commentPos+1)
        }
    }

}
