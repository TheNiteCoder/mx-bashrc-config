#include "window.h"

//import all tab implementations
#include "tab.h"
#include "prompttab.h"
#include "aliastab.h"

#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

#include <QSettings>

Window::Window(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Window),
    m_manager(ui)
{
    DEBUG_ENTER(Window::Window);
    ui->setupUi(this);

    readPositionSettings();

    setWindowTitle(NAME);

    m_manager.addTab(new AliasTab());
    m_manager.addTab(new PromptTab());
    auto data = getSource();
    m_manager.setup(getSource());

    connect(ui->pushButton_Apply, &QPushButton::clicked, [=](){
        auto source = getSource();
        if(!source.bashrc.contains("source $HOME/.config/mx-bashrc-config/bashrc.bash"))
        {
            source.bashrc.append("source $HOME/.config/mx-bashrc-config/bashrc.bash");
        }
        source.program = "";
        setSource(m_manager.exec(source));
    });
    connect(ui->pushButton_Close, &QPushButton::clicked, [=](){
        close();
    });
    connect(ui->pushButton_About, &QPushButton::clicked, [=](){
        QMessageBox::about(this, NAME, "An easy way to configure your ~/.bashrc and bash prompt");
    });
    connect(ui->pushButton_Help, &QPushButton::clicked, [=](){
#warning This program does not have an help browser yet
        QMessageBox::about(this, NAME, "Can't really do that web browser thing without a link to go to");
    });
    DEBUG_EXIT(Window::Window);
}

void Window::closeEvent(QCloseEvent *event)
{
    DEBUG_ENTER(Window::closeEvent);
    Q_UNUSED(event);
    writePositionSettings();
    DEBUG_EXIT(Window::closeEvent);
}

Window::~Window()
{
    DEBUG_ENTER(Window::~Window);
    delete ui;
    DEBUG_EXIT(Window::~Window);
}

BashrcSource Window::getSource()
{
    DEBUG_ENTER(Window::getSource);
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

    DEBUG_EXIT(Window::getSource);
    return data;
}

void Window::setSource(const BashrcSource data)
{
    DEBUG_ENTER(Window::setSource);
    QFile bashrc(QDir::homePath() + "/.bashrc");
    if(!bashrc.open(QFile::Text | QFile::WriteOnly))
    {
        //TODO error
        return;
    }
    QTextStream bashrcStream(&bashrc);
    bashrcStream << data.bashrc;
    bashrc.close();

    QFile program(QDir::homePath() + "/.config/mx-bashrc-config/bashrc.bash");
    QFileInfo programInfo(program);
    QDir dir;
    if(!dir.exists(programInfo.absolutePath()))
    {
        qDebug() << "Creating dir: " << programInfo.absolutePath();
        if(!dir.mkpath(programInfo.absolutePath()))
            qDebug() << "Failed creating dir";
    }
    if(!program.open(QFile::Text | QFile::WriteOnly))
    {
        //TODO error
        //doesn't exist or can't access
        return;
    }
    QTextStream programStream(&program);
    programStream << data.program;
    program.close();
    DEBUG_EXIT(Window::setSource);
}

Window::TabManager::TabManager(Ui::Window *ui)
    : window_ui(ui)
{
    DEBUG_ENTER(TabManager::TabManager);
    DEBUG_EXIT(TabManager::TabManager);
}

Window::TabManager &Window::TabManager::addTabs(QList<Tab *> tabs)
{
    DEBUG_ENTER(TabManager::addTabs);
    for(Tab* tab : tabs)
    {
        m_tabs.append(tab);
    }
    DEBUG_EXIT(TabManager::addTabs);
    return *this;
}

Window::TabManager &Window::TabManager::addTab(Tab *tab)
{
    DEBUG_ENTER(TabManager::addTab);
    addTabs(QList<Tab*>() << tab);
    DEBUG_EXIT(TabManager::addTab);
    return *this;
}

Window::TabManager &Window::TabManager::setup(const BashrcSource source)
{
    DEBUG_ENTER(TabManager::setup);
    for(Tab* tab : m_tabs)
    {
        tab->setup(source);
        if(tab->widget() != nullptr)
            window_ui->tabWidget_Tabs->addTab(tab->widget(), tab->icon(), tab->name());
    }
    DEBUG_EXIT(TabManager::setup);
    return *this;
}

BashrcSource Window::TabManager::exec(const BashrcSource source)
{
    DEBUG_ENTER(TabManager::exec);
    BashrcSource rtn = source;
    for(Tab* tab : m_tabs)
    {
        rtn = tab->exec(rtn);
    }
    DEBUG_EXIT(TabManager::exec);
    return rtn;
}

void Window::writePositionSettings()
{
    DEBUG_ENTER(Window::writePositionSettings);
    QSettings qsettings( "MX-Linux", "mx-bashrc-config" );

    qsettings.beginGroup( "mainwindow" );

    qsettings.setValue( "geometry", saveGeometry() );
    qsettings.setValue( "maximized", isMaximized() );
    if ( !isMaximized() ) {
        qsettings.setValue( "size", size() );
    }

    qsettings.endGroup();
    DEBUG_EXIT(Window::writePositionSettings);
}

void Window::readPositionSettings()
{
    DEBUG_ENTER(Window::readPositionSettings);
    QSettings qsettings( "MX-Linux", "mx-bashrc-config" );

    qsettings.beginGroup( "mainwindow" );

    restoreGeometry(qsettings.value( "geometry", saveGeometry() ).toByteArray());
    resize(qsettings.value( "size", size() ).toSize());
    if ( qsettings.value( "maximized", isMaximized() ).toBool() )
        showMaximized();

    qsettings.endGroup();
    DEBUG_EXIT(Window::readPositionSettings);
}
