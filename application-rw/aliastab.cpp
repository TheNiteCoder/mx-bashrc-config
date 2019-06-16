#include "aliastab.h"

#include "searcher.h"
#include <QTextStream>

AliasTab::AliasTab()
    : Tab("Aliases")
{
    ui = new Ui::AliasTab;
    setWidget(new QWidget());
    ui->setupUi(widget());
    ui->tableWidget_Aliases->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget_Aliases->setColumnCount(2);
    ui->tableWidget_Aliases->setHorizontalHeaderLabels(QStringList() << "Alias" << "Command");
    ui->tableWidget_Aliases->setSelectionBehavior(QTableWidget::SelectRows);

    connect(ui->pushButton_AliasAdd, &QPushButton::clicked, [=](){
        ui->tableWidget_Aliases->setRowCount(ui->tableWidget_Aliases->rowCount()+1);
        ui->tableWidget_Aliases->setItem(ui->tableWidget_Aliases->rowCount()-1, 0, new AliasTabTableWidgetItem("", false));
        ui->tableWidget_Aliases->setItem(ui->tableWidget_Aliases->rowCount()-1, 1, new AliasTabTableWidgetItem(""));
    });
    connect(ui->pushButton_AliasRemove, &QPushButton::clicked, [=](){
        for(int i = ui->tableWidget_Aliases->rowCount()-1; i > -1; i--)
        {
            if(ui->tableWidget_Aliases->item(i,0) != nullptr &&  ui->tableWidget_Aliases->item(i,0)->isSelected())
            {
                AliasData adata;
                adata.alias = ui->tableWidget_Aliases->item(i, 0)->text();
                adata.command = ui->tableWidget_Aliases->item(i, 1)->text();
                if(static_cast<AliasTabTableWidgetItem*>(ui->tableWidget_Aliases->item(i, 0))->info().toBool())
                {
                    adata.inBashrc = true;
                    m_deletedAliases << adata;
                }
                ui->tableWidget_Aliases->removeRow(i);
            }
        }
    });
}

AliasTab::~AliasTab()
{
    delete ui;
}

void AliasTab::setup(const BashrcSource data)
{
    QString tmp;
    QList<AliasData> aliases;

#define CHECK(x) if(x == -1) continue
    QTextStream stream(new QString(data.program));
    while(stream.readLineInto(&tmp))
    {
        AliasData adata;
        adata.inBashrc = false;
        QString scan = tmp;
        if(scan.indexOf('#') != -1)
        {
            scan = scan.mid(0, scan.indexOf('#'));
        }
        //TODO error checking
        int aliasKeywordBegin = scan.indexOf("alias");
        CHECK(aliasKeywordBegin);
        int aliasWordBegin = scan.indexOf(QRegularExpression("\\S"), aliasKeywordBegin+5);
        CHECK(aliasWordBegin);
        int equalSignPos = scan.indexOf('=', aliasWordBegin);
        CHECK(equalSignPos);
        adata.alias = scan.mid(aliasWordBegin, equalSignPos-aliasWordBegin);
        if(equalSignPos >= scan.length()) //in the case of nothing beginning assigned ex: alias hello=
        {
            aliases << adata;
            continue;
        }
        QChar quote = scan.at(equalSignPos+1);
        //check if the length is less or equal to equalSignPos+1 so indexOf doesn't recieve an invalid from point
        if(equalSignPos+1 >= scan.length())
        {
            continue;
        }
        int otherQuote = scan.indexOf(quote, equalSignPos+2);
        CHECK(otherQuote);
        adata.command = scan.mid(equalSignPos+2, otherQuote-(equalSignPos-1));
        adata.command.chop(1);
        aliases << adata;
    }

    stream.setString(new QString(data.bashrc));
    while(stream.readLineInto(&tmp))
    {
        AliasData adata;
        adata.inBashrc = true;
        QString scan = tmp;
        if(scan.indexOf('#') != -1)
        {
            scan = scan.mid(0, scan.indexOf('#'));
        }
        //TODO error checking
        int aliasKeywordBegin = scan.indexOf("alias");
        CHECK(aliasKeywordBegin);
        int aliasWordBegin = scan.indexOf(QRegularExpression("\\S"), aliasKeywordBegin+5);
        CHECK(aliasWordBegin);
        int equalSignPos = scan.indexOf('=', aliasWordBegin);
        CHECK(equalSignPos);
        adata.alias = scan.mid(aliasWordBegin, equalSignPos-aliasWordBegin);
        if(equalSignPos >= scan.length()) //in the case of nothing beginning assigned ex: alias hello=
        {
            aliases << adata;
            continue;
        }
        QChar quote = scan.at(equalSignPos+1);
        //check if the length is less or equal to equalSignPos+1 so indexOf doesn't recieve an invalid from point
        if(equalSignPos+1 >= scan.length())
        {
            continue;
        }
        int otherQuote = scan.indexOf(quote, equalSignPos+2);
        CHECK(otherQuote);
        adata.command = scan.mid(equalSignPos+2, otherQuote-(equalSignPos-1));
        adata.command.chop(1);
        aliases << adata;
    }

    for(AliasData adata : aliases)
    {
        ui->tableWidget_Aliases->setRowCount(ui->tableWidget_Aliases->rowCount()+1);
        ui->tableWidget_Aliases->setItem(ui->tableWidget_Aliases->rowCount()-1, 0, new AliasTabTableWidgetItem(adata.alias, adata.inBashrc));
        ui->tableWidget_Aliases->setItem(ui->tableWidget_Aliases->rowCount()-1, 1, new AliasTabTableWidgetItem(adata.command));
    }
}

BashrcSource AliasTab::exec(const BashrcSource data)
{
    BashrcSource rtn;
    rtn.bashrc = data.bashrc;
    rtn.program = data.program;

    QList<AliasData> aliases;
    for(int r = 0; r < ui->tableWidget_Aliases->rowCount(); r++)
    {
        AliasData adata;
        adata.alias = ui->tableWidget_Aliases->item(r, 0)->text();
        adata.command = ui->tableWidget_Aliases->item(r, 1)->text();
        adata.inBashrc = static_cast<AliasTabTableWidgetItem*>(ui->tableWidget_Aliases->item(r, 0))->info().toBool();
        aliases << adata;
    }

    for(AliasData adata : m_deletedAliases)
    {
        Searcher searcher(new QString(rtn.bashrc));
        if(CHECK_SEARCH(searcher.search(QRegularExpression(tr("[\\s]{0,}alias[\\s]+") + adata.alias + "=(\"|')" + adata.command + "(\"|')"))))
        {
            rtn.bashrc.remove(QRegularExpression(tr("[\\s]{0,}alias[\\s]+") + adata.alias + "=(\"|')" + adata.command + "(\"|')"));
        }
    }

    for(AliasData adata : aliases)
    {
        if(adata.inBashrc)
        {
            //TODO make a smart system for detecting if a alias exsists but the command changed
            Searcher searcher(new QString(rtn.bashrc));
            if(CHECK_SEARCH(searcher.search(QRegularExpression(tr("[\\s]{0,}alias[\\s]+") + adata.alias + "=(\"|')" + adata.command + "(\"|')"))))
            {
                continue;
            }
            adata.command.replace('\'', "\\\'");
            adata.command.replace('"', "\\\"");
            rtn.bashrc.append("\n" + tr("alias ") + adata.alias + "='" + adata.command + "'");
        }
        else
        {
            adata.alias.replace('\'', "\\'");
            adata.command.replace('"', "\\\"");
            rtn.program.append("\n" + tr("alias ") + adata.alias + tr("='") + adata.command + tr("'"));
        }
    }
    return rtn;
}

AliasTabTableWidgetItem::AliasTabTableWidgetItem(QString text, QVariant info)
{
    setText(text);
    setInfo(info);
}

AliasTabTableWidgetItem::~AliasTabTableWidgetItem()
{
     //None
}

AliasTabTableWidgetItem &AliasTabTableWidgetItem::setInfo(QVariant info)
{
    m_info = info;
    return *this;
}

//temp fix QVarient crashes program
QVariant& AliasTabTableWidgetItem::info()
{
    return m_info;
}

#undef CHECK_SEARCH
