#include "tab.h"

Tab::Tab(const QString name, const QIcon icon, QObject *parent)
    : QObject(parent)
{
    setName(name);
    setIcon(icon);
}

Tab::Tab(Tab &copy)
    : QObject(copy.parent())
{
    *this = copy;
}

Tab::Tab(Tab &&move)
    : QObject(move.parent())
{
    *this = move;
}

Tab& Tab::operator =(Tab &copy)
{
    setName(copy.name());
    setIcon(copy.icon());
    return *this;
}

Tab& Tab::operator =(Tab &&move)
{
    setName(move.name());
    setIcon(move.icon());
    move.setIcon(QIcon());
    move.setName(QString());
    return *this;
}

Tab::~Tab()
{

}

QString Tab::name()
{
    return m_name;
}

QIcon Tab::icon()
{
    return m_icon;
}

Tab &Tab::setWidget(QWidget *widget)
{
    m_widget = widget;
    return *this;
}

QWidget *Tab::widget()
{
    return m_widget;
}

Tab& Tab::setIcon(const QIcon icon)
{
    m_icon = icon;
    return *this;
}

Tab& Tab::setName(const QString name)
{
    m_name = name;
    return *this;
}
