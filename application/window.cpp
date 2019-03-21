#include "window.h"
#include "ui_window.h"
#include "config.h"
#include <QFile>
#include <QTextStream>

Window::Window(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Window)
{
    //ui setup
    ui->setupUi(this);
    setWindowTitle(tr(APPNAME)+tr(" ")+tr(VERSION));
    //add all plugins wanted
    pluginManager.addPlugin(new FancyPromptPlugin);
    //add guis to tab widget
    foreach(Plugin* p, pluginManager.plugins())
    {
        ui->tabWidget->addTab(p->widget(), QIcon(), (p->name().isEmpty() || p->name().isNull()) ? tr("Unnamed") : p->name());
    }
    connect(ui->pushButton_Apply, &QPushButton::clicked, this, &Window::apply);
}

Window::~Window()
{
    delete ui;
}

void Window::apply()
{
    QFile bashrc(BASHRC);
    if(!bashrc.open(QFile::ReadWrite))
    {
        //ERROR
    }
    QTextStream bashrcStream(&bashrc);
    QString source = bashrcStream.readAll();
    source = pluginManager.execPlugins(source);
    bashrc.resize(0);
    bashrcStream << source;
    bashrc.close();
}


