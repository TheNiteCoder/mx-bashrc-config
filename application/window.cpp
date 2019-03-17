#include "window.h"
#include "ui_window.h"

Window::Window(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Window),
    parser(new BashrcParser(this))
{
    ui->setupUi(this);
    setupAliasTable();
}

Window::~Window()
{
    delete ui;
}

void Window::setupAliasTable()
{
    ui->tableWidget_Aliases->setSelectionBehavior(QTableWidget::SelectRows);
    ui->tableWidget_Aliases->horizontalHeader()->stretchLastSection();
    ui->tableWidget_Aliases->setHorizontalHeaderLabels(QStringList() << "Alias" << "Command");
    QList<Alias> la = parser->getAliases();
    foreach (Alias a, la)
    {
        ui->tableWidget_Aliases->setRowCount(ui->tableWidget_Aliases->rowCount()+1);
        ui->tableWidget_Aliases->setItem(ui->tableWidget_Aliases->rowCount()-1, 0, new QTableWidgetItem(a.alias));
        ui->tableWidget_Aliases->setItem(ui->tableWidget_Aliases->rowCount()-1, 1, new QTableWidgetItem(a.command));
    }

}
