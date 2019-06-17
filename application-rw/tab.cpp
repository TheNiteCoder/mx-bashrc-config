#include "tab.h"

Tab::Tab(const QString name, const QIcon icon, QObject *parent)
    : QObject(parent)
{
    DEBUG_ENTER(Tab::Tab);
    setName(name);
    setIcon(icon);
    DEBUG_EXIT(Tab::Tab);
}

Tab::Tab(Tab &copy)
    : QObject(copy.parent())
{
    DEBUG_ENTER(Tab::Tab);
    *this = copy;
    DEBUG_EXIT(Tab::Tab);
}

Tab::Tab(Tab &&move)
    : QObject(move.parent())
{
    DEBUG_ENTER(Tab::Tab);
    *this = move;
    DEBUG_EXIT(Tab::Tab);
}

Tab& Tab::operator =(Tab &copy)
{
    DEBUG_ENTER(Tab::operator=);
    setName(copy.name());
    setIcon(copy.icon());
    DEBUG_EXIT(Tab::operator=);
    return *this;
}

Tab& Tab::operator =(Tab &&move)
{
    DEBUG_ENTER(Tab::operator=);
    setName(move.name());
    setIcon(move.icon());
    move.setIcon(QIcon());
    move.setName(QString());
    DEBUG_EXIT(Tab::operator=);
    return *this;
}

Tab::~Tab()
{
    DEBUG_ENTER(Tab::~Tab);
    DEBUG_EXIT(Tab::~Tab);
}

QString Tab::name()
{
    DEBUG_ENTER(Tab::name);
    DEBUG_EXIT(Tab::name);
    return m_name;
}

QIcon Tab::icon()
{
    DEBUG_ENTER(Tab::icon);
    DEBUG_EXIT(Tab::icon);
    return m_icon;
}

Tab &Tab::setWidget(QWidget *widget)
{
    DEBUG_ENTER(Tab::setWidget);
    m_widget = widget;
    DEBUG_EXIT(Tab::setWidget);
    return *this;
}

QWidget *Tab::widget()
{
    DEBUG_ENTER(Tab::widget);
    DEBUG_EXIT(Tab::widget);
    return m_widget;
}

Tab& Tab::setIcon(const QIcon icon)
{
    DEBUG_ENTER(Tab::setIcon);
    m_icon = icon;
    DEBUG_EXIT(Tab::setIcon);
    return *this;
}

Tab& Tab::setName(const QString name)
{
    DEBUG_ENTER(Tab::setName);
    m_name = name;
    DEBUG_EXIT(Tab::setName);
    return *this;
}
