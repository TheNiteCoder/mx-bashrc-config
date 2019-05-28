#include "window.h"

//import all tab implementations
#include "tab.h"
#include "prompttab.h"
#include "aliastab.h"

#include <QDir>
#include <QFile>
#include <QTextStream>

Window::Window(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Window),
    m_manager(ui)
{
    ui->setupUi(this);

    QFile bashrc(QDir::homePath() + "/.bashrc");
    if(!bashrc.open(QFile::Text | QFile::ReadOnly))
    {
        DEBUG_VAR(bashrc.fileName());
    }
    QTextStream bashrcStream(&bashrc);
    QString bashrcSource = bashrcStream.readAll();
    DEBUG_VAR(bashrcSource);
    bashrc.close();

    QFile program(QDir::homePath() + "/.bashrc");
    if(!program.open(QFile::Text | QFile::ReadOnly))
    {
        DEBUG_VAR(program.fileName());
    }
    QTextStream programStream(&program);
    QString programSource = programStream.readAll();
    DEBUG_VAR(programSource);
    program.close();

    BashrcSource data;
    data.program = programSource;
    data.bashrc = bashrcSource;

    m_manager.addTab(new PromptTab());
    m_manager.addTab(new AliasTab());
    m_manager.setup(data);
}

Window::~Window()
{
    delete ui;
}

BashrcSource Window::getSource()
{
    QFile bashrc(QDir::homePath() + "/.bashrc");
    if(!bashrc.open(QFile::Text | QFile::ReadOnly))
    {
        //TODO error
    }
    QTextStream bashrcStream(&bashrc);
    QString bashrcSouce = bashrcStream.readAll();
    bashrc.close();

    QFile program(QDir::homePath() + "/.bashrc");
    if(!program.open(QFile::Text | QFile::ReadOnly))
    {
        //TODO error
    }
    QTextStream programStream(&program);
    QString programSouce = programStream.readAll();
    program.close();

    BashrcSource data;
    data.program = programSouce;
    data.bashrc =  bashrcSouce;

    return data;
}

Window::TabManager::TabManager(Ui::Window *ui)
    : window_ui(ui)
{
}

Window::TabManager &Window::TabManager::addTabs(QList<Tab *> tabs)
{
    for(Tab* tab : tabs)
    {
        m_tabs.append(tab);
    }
    return *this;
}

Window::TabManager &Window::TabManager::addTab(Tab *tab)
{
    addTabs(QList<Tab*>() << tab);
    return *this;
}

Window::TabManager &Window::TabManager::setup(const BashrcSource source)
{
    for(Tab* tab : m_tabs)
    {
        tab->setup(source);
        if(tab->widget() != nullptr)
            window_ui->tabWidget_Tabs->addTab(tab->widget(), tab->icon(), tab->name());
    }
    return *this;
}

BashrcSource Window::TabManager::exec(const BashrcSource source)
{
    BashrcSource rtn = source;
    for(Tab* tab : m_tabs)
    {
        rtn = tab->exec(source);
    }
    return rtn;
}
