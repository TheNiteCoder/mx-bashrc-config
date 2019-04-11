#include "window.h"
#include "ui_window.h"
#include "config.h"
#include <QFile>
#include <QTextStream>
#include <QVBoxLayout>
#include <QGroupBox>

Window::Window(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Window)
{
    //ui setup
    ui->setupUi(this);
    setWindowTitle(tr(APPNAME)+tr(" ")+tr(VERSION));
    //add all plugins wanted
    pluginManager.addPlugin(new RestorePlugin);
    pluginManager.addPlugin(new PromptPlugin);
    //pluginManager.addPlugin(new FancyPromptPlugin);
    //add guis to tab widget
    QFile programBashrc(PROGRAM_BASHRC);
    if(!programBashrc.open(QFile::Text | QFile::ReadOnly))
    {
        //error
    }
    QTextStream programBashrcStream(&programBashrc);
    QString source = programBashrcStream.readAll();
    programBashrc.close();
    int otherTab = -1;
    for(int i = 0; i < ui->tabWidget->count(); i++)
    {
        if(ui->tabWidget->tabText(i) == tr("Other")) otherTab = i;
    }
    if(otherTab == -1)
    {
        ui->tabWidget->addTab(new QWidget, "Other");
        ui->tabWidget->currentWidget()->setLayout(new QVBoxLayout);
    }
    otherTab = ui->tabWidget->currentIndex();
    foreach(Plugin* p, pluginManager.plugins())
    {
        if(p->displayFormat() == Plugin::DisplayFormat_Tab)
            ui->tabWidget->addTab(p->widget(), QIcon(),
                                  (p->name().isEmpty()
                                   || p->name().isNull()) ? tr("Unnamed") : p->name());
        else if(p->displayFormat() == Plugin::DisplayFormat_Section)
        {
            int saveTabIndex = ui->tabWidget->currentIndex();

            if(otherTab == -1)
            {
                ui->tabWidget->addTab(new QWidget, "Other");
                ui->tabWidget->currentWidget()->setLayout(new QVBoxLayout);
            }
            ui->tabWidget->setCurrentIndex(otherTab);
            QGroupBox* groupBox = new QGroupBox(p->name());
            groupBox->setLayout(new QVBoxLayout);
            groupBox->layout()->addWidget(p->widget());
            ui->tabWidget->currentWidget()->layout()->addWidget(groupBox);
            ui->tabWidget->setCurrentIndex(saveTabIndex);
        }
        else if(p->displayFormat() == Plugin::DisplayFormat_Single)
        {
            int tabCount = ui->tabWidget->count();
            int saveTabIndex = ui->tabWidget->currentIndex();
            for(int i = 0; i < tabCount; i++)
            {
                if(ui->tabWidget->tabText(i) == tr("Other")) otherTab = i;
            }
            if(otherTab == -1)
            {
                ui->tabWidget->addTab(new QWidget, "Other");
                ui->tabWidget->currentWidget()->setLayout(new QVBoxLayout);
            }
            ui->tabWidget->setCurrentIndex(otherTab);
            ui->tabWidget->currentWidget()->layout()->addWidget(p->widget());
            ui->tabWidget->setCurrentIndex(saveTabIndex);
        }
        p->setup(source, QString());
    }
    if(otherTab != -1)
    {
        ui->tabWidget->setCurrentIndex(otherTab);
        dynamic_cast<QVBoxLayout*>(ui->tabWidget->currentWidget()->layout())->addStretch();
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
    QString bashrcContent = bashrcStream.readAll();
#if DO_SEPERATE_FILE
    if(!bashrcContent.contains(QRegExp(tr("source[\\s]+") + PROGRAM_BASHRC)) ||
            !bashrcContent.contains(QRegExp(tr(".[\\s]+") + PROGRAM_BASHRC)))
    {
        bashrcStream << "#This line is important because it enables all of mx-bashrc-config's configuration\n"
                     << "source "
                     << PROGRAM_BASHRC;
    }
    QFile programBashrc(PROGRAM_BASHRC);
    if(!programBashrc.open(QFile::Text | QFile::WriteOnly))
    {
        //error
    }
    programBashrc.resize(0);
    QTextStream programBashrcStream(&programBashrc);
    QString code = pluginManager.execPlugins(QString(), bashrcContent);
    programBashrcStream << code;
    bashrc.close();
    programBashrc.close();
#else
    //implment changing the actual file here
    QString newBashrcContent = pluginManager.execPlugins(bashrcContent);
#endif

}


