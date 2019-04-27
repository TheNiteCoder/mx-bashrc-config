#include "aliasplugin.h"

#include <QTableWidgetItem>
#include "seacher.h"

AliasPlugin::AliasPlugin()
    : Plugin("Aliases", Plugin::DisplayFormat_Tab)
{
    ui->setupUi(widget());
    ui->tableWidget_Aliases->setSelectionBehavior(QTableWidget::SelectRows);
    ui->tableWidget_Aliases->horizontalHeader()->setStretchLastSection(true);
}

AliasPlugin::~AliasPlugin()
{

}

void AliasPlugin::setup(const QString source, const QString bashrcSource)
{
    //connect btns with actions
    connect(ui->pushButton_RemoveAlias, &QPushButton::clicked, [=](){
        for(int i = ui->tableWidget_Aliases->rowCount()-1; i > -1; i--)
        {
            if(ui->tableWidget_Aliases->item(i,0) != nullptr && ui->tableWidget_Aliases->item(i,0)->isSelected())
            {
                ui->tableWidget_Aliases->removeRow(i);
            }
        }
    });
    connect(ui->pushButton_AddAlias, &QPushButton::clicked, [=](){
        ui->tableWidget_Aliases->setItem(ui->tableWidget_Aliases->rowCount(), 0, new QTableWidgetItem("<Alias>"));
        ui->tableWidget_Aliases->setItem(ui->tableWidget_Aliases->rowCount(), 1, new QTableWidgetItem("<Command>"));
    });
    Searcher searcher(Searcher::CheckForDoubleQuotes | Searcher::CheckForSingleQuotes | Searcher::CheckForSpecialQuotes);
    for(QString line : bashrcSource.split('\n'))
    {
        if(searcher.test(line, "#") != Searcher::ReturnOptionFailed ||
                searcher.test(line, "#") != Searcher::ReturnSubstringNotFound)
        {
            line = line.mid(searcher.test(line, "#"));
        }

    }
}

QString AliasPlugin::exec(const QString source, const QString bashrcSource)
{
    return source;
}

AliasPlugin::AliasList AliasPlugin::aliases(QString source)
{
    AliasList rtn;

    QTextStream sourceStream(new QString(source));
    QString line;
    while(sourceStream.readLineInto(&line))
    {
    }
    return rtn;
}
