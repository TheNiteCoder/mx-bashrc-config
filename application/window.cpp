#include "window.h"

//import all tab implementations
#include "tab.h"
#include "prompttab.h"
#include "aliastab.h"
#include "othertab.h"

#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

#include <unistd.h>

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

    m_manager.addTab(new AliasTab);
    m_manager.addTab(new PromptTab);
    m_manager.addTab(new OtherTab);
    auto data = getSource();
    m_manager.setup(getSource());

    connect(ui->pushButton_Apply, &QPushButton::clicked, [=](){
        auto source = getSource();
        if(!source.bashrc.contains("source $HOME/.config/bash-config/bashrc.bash"))
        {
            source.bashrc.append("source $HOME/.config/bash-config/bashrc.bash");
        }
        source.program = "";
        setSource(m_manager.exec(source));
    });
    connect(ui->pushButton_Close, &QPushButton::clicked, [=](){
        close();
    });
    connect(ui->pushButton_About, &QPushButton::clicked, [=](){
        QMessageBox::about(this, NAME, tr("An easy way to configure your ~/.bashrc and bash prompt"));
    });
    DEBUG_EXIT(Window::Window);
}

void Window::closeEvent(QCloseEvent *event)
{
    DEBUG_ENTER(Window::closeEvent);
    Q_UNUSED(event)
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
    QFile bashrc(USER_BASHRC);
    if(!bashrc.open(QFile::Text | QFile::ReadOnly))
    {
        DEBUG << bashrc.fileName() + " isn't readable or exists";
        return BashrcSource();
    }
    QTextStream bashrcStream(&bashrc);
    QString bashrcSource = bashrcStream.readAll();
    bashrc.close();

    QFile program(PROGRAM_BASHRC);
    if(!program.open(QFile::Text | QFile::ReadOnly))
    {
        //TODO error
        //doesn't exist or can't access
        DEBUG << program.fileName() + " isn't readable or exists";
        BashrcSource source;
        source.bashrc = bashrcSource;
        return source;
    }
    QTextStream programStream(&program);
    QString programSource = programStream.readAll();
    program.close();

    QFile bashrcAliases(USER_BASHRC_ALIASES);
    if(!bashrcAliases.open(QFile::Text | QFile::ReadOnly))
    {
        //TODO error
        //doesn't exist or can't access
        DEBUG << bashrcAliases.fileName() + " isn't readable or exists";
        BashrcSource source;
        source.bashrc = bashrcSource;
        source.program = programSource;
        return source;
    }
    QTextStream bashrcAliasesStream(&bashrcAliases);
    QString bashrcAliasesSource = bashrcAliasesStream.readAll();
    bashrcAliases.close();

    BashrcSource data;
    data.program = programSource;
    data.bashrc =  bashrcSource;
    data.bashrcAliases = bashrcAliasesSource;

    DEBUG_EXIT(Window::getSource);
    return data;
}

void Window::setSource(const BashrcSource data)
{
    DEBUG_ENTER(Window::setSource);
    QFile bashrc(USER_BASHRC);
    if(!bashrc.open(QFile::Text | QFile::WriteOnly))
    {
        //TODO error
        return;
    }
    QTextStream bashrcStream(&bashrc);
    bashrcStream << data.bashrc;
    bashrc.close();

    QFile program(PROGRAM_BASHRC);
    QFileInfo programInfo(program);
    QDir dir;
    if(!dir.exists(programInfo.absolutePath()))
    {
        DEBUG << "Creating dir: " << programInfo.absolutePath();
        if(!dir.mkpath(programInfo.absolutePath()))
            DEBUG << "Failed creating dir";
    }
    if(!program.open(QFile::Text | QFile::WriteOnly))
    {
        //TODO error
        //doesn't exist or can't access
        DEBUG << program.fileName() + "isn't writable or exists";
        return;
    }
    QTextStream programStream(&program);
    programStream << data.program;
    program.close();

    QFile bashrcAliases(USER_BASHRC_ALIASES);
    QFileInfo bashrcAliasesInfo(bashrcAliases);
    if(!dir.exists(bashrcAliasesInfo.absolutePath()))
    {
        DEBUG << "Creating dir: " << bashrcAliasesInfo.absolutePath();
        if(!dir.mkpath(bashrcAliasesInfo.absolutePath()))
            DEBUG << "Failed creating dir";
    }
    if(!bashrcAliases.open(QFile::Text | QFile::WriteOnly))
    {
        //TODO error
        //doesn't exist or can't access
        DEBUG << bashrcAliases.fileName() + "isn't writable or exists";
        return;
    }
    QTextStream bashrcAliasesStream(&bashrcAliases);
    bashrcAliasesStream << data.bashrcAliases;
    bashrcAliases.close();
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
    QSettings qsettings( "MX-Linux", "bash-config" );

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
    QSettings qsettings( "MX-Linux", "bash-config" );

    qsettings.beginGroup( "mainwindow" );

    restoreGeometry(qsettings.value( "geometry", saveGeometry() ).toByteArray());
    resize(qsettings.value( "size", size() ).toSize());
    if ( qsettings.value( "maximized", isMaximized() ).toBool() )
        showMaximized();

    qsettings.endGroup();
    DEBUG_EXIT(Window::readPositionSettings);
}

void Window::on_pushButton_Help_clicked()
{
    QLocale locale;
    QString lang = locale.bcp47Name();

    QFileInfo viewer("/usr/bin/mx-viewer");
    QFileInfo viewer2("/usr/bin/antix-viewer");

    QString url = "file:///usr/share/doc/bash-config/help/bash-config.html";
    QString cmd;

    if (viewer.exists()){
         cmd = QString("mx-viewer %1 '%2' &").arg(url).arg(tr("Bash Config"));
    } else if (viewer2.exists()) {
         cmd = QString("antix-viewer %1 '%2' &").arg(url).arg(tr("Bash Config"));
    } else {
         cmd = QString("xdg-open %1 &").arg(url).arg(tr("Bash Config"));
    }

    system(cmd.toUtf8());

}
