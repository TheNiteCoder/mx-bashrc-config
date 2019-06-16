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

    m_manager.addTab(new AliasTab());
    //loading alias tab first because alias overriding somewhere FIX
    m_manager.addTab(new PromptTab());
    auto data = getSource();
    if(data.bashrc.contains("source $HOME/.config/mx-bashrc-config/bashrc.bash"))
    {
        ui->checkBox_Use->setChecked(true);
    }
    else
    {
        ui->checkBox_Use->setChecked(false);
    }
    m_manager.setup(getSource());

    connect(ui->pushButton_Apply, &QPushButton::clicked, [=](){
        auto source = getSource();
        if(!source.bashrc.contains("source $HOME/.config/mx-bashrc-config/bashrc.bash"))
        {
            if(ui->checkBox_Use->isChecked())
                source.bashrc.append("source $HOME/.config/mx-bashrc-config/bashrc.bash");
        }
        else
        {
            if(!ui->checkBox_Use->isChecked())
            {
                source.bashrc.remove("source $HOME/.config/mx-bashrc-config/bashrc.bash");
            }
        }
        source.program = "";
        setSource(m_manager.exec(source));
    });
    connect(ui->pushButton_Close, &QPushButton::clicked, [=](){
        close();
    });
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
        return BashrcSource();
    }
    QTextStream bashrcStream(&bashrc);
    QString bashrcSouce = bashrcStream.readAll();
    bashrc.close();

    QFile program(QDir::homePath() + "/.config/mx-bashrc-config/bashrc.bash");
    if(!program.open(QFile::Text | QFile::ReadOnly))
    {
        //TODO error
        //doesn't exist or can't access
        BashrcSource source;
        source.bashrc = bashrcSouce;
        return source;
    }
    QTextStream programStream(&program);
    QString programSouce = programStream.readAll();
    program.close();

    BashrcSource data;
    data.program = programSouce;
    data.bashrc =  bashrcSouce;

    return data;
}

void Window::setSource(const BashrcSource data)
{
    DEBUG << "Program Bashrc: " << data.program;
    QFile bashrc(QDir::homePath() + "/.bashrc");
    if(!bashrc.open(QFile::Text | QFile::WriteOnly))
    {
        //TODO error
        DEBUG << "Cannot open bashrc";
        return;
    }
    QTextStream bashrcStream(&bashrc);
    bashrcStream << data.bashrc;
    bashrc.close();

    QFile program(QDir::homePath() + "/.config/mx-bashrc-config/bashrc.bash");
    if(!program.open(QFile::Text | QFile::WriteOnly))
    {
        //TODO error
        //doesn't exist or can't access
        DEBUG << "Cannot open program bashrc";
        return;
    }
    QTextStream programStream(&program);
    programStream << data.program;
    program.close();
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
        rtn = tab->exec(rtn);
        DEBUG << "Bashrc Program: " << rtn.program;
    }
    return rtn;
}
