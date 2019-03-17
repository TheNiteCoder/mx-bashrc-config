#include "parser.h"

Parser::Parser(QString source, QObject *parent)
    : QObject(parent), __source(source)
{

}

void Parser::doSetEvent(Parser::SetEvent *e)
{
    __source = e->process(source());
}

void Parser::doSetEvents(QList<Parser::SetEvent *> le)
{
    foreach (SetEvent* e, le) {
        __source  = e->process(source());
    }
}

void *Parser::doGetEvent(Parser::GetEvent *e)
{
    return e->process(source());
}
