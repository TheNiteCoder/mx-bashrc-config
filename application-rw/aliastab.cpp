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
}

AliasTab::~AliasTab()
{
    delete ui;
}

void AliasTab::setup(const BashrcSource data)
{
#define CHECK_SEARCH(x) (x != Searcher::ReturnValueSearchStatesFailed) && (x != Searcher::ReturnValueSearchStringNotFound)
    QTextStream stream(new QString(data.program));
    QString tmp;
    QList<AliasData> aliases;
    while(stream.readLineInto(&tmp))
    {
        Searcher searcher(tmp, Searcher::StateCheckDoubleQuotations | Searcher::StateCheckSingleQuotations |
                          Searcher::StateCheckSpecialQuotations);

        QRegularExpression aliasRegExp("[\\s]{0,}alias[\\s]+[\\w]+=\"|'[\\s\\S]{0,}\"|'");

        int pos;
        while(CHECK_SEARCH((pos = searcher.search(aliasRegExp))))
        {
            AliasData adata;
            adata.inBashrc = false;
            tmp = tmp.mid(pos);
            QString work = tmp;
            searcher.setSource(work);
            int chopPos = searcher.search("alias");
            work = work.mid(chopPos + 5);
            DEBUG_VAR(work);
            searcher.setSource(work);
            work = work.mid(searcher.search(QRegularExpression("\\S")));
            DEBUG_VAR(work);
            adata.alias = work.mid(0, searcher.search('=')-1);
            work = work.mid(searcher.search('=')+1);
            DEBUG_VAR(work);
            searcher.setSource(work);
            bool doubleQuotes = true;
            if(work.at(0) != '"')
                doubleQuotes = false;
#define QUOTE (doubleQuotes) ? '"' : '\''
            work = work.mid(1);
            DEBUG_VAR(work);
            searcher.setSource(work);
            adata.command = work.mid(0, searcher.search(QUOTE));
            aliases << adata;
            tmp = work.mid(searcher.search(QUOTE)+1);
#undef QUOTE
            searcher.setStates(Searcher::StateCheckDoubleQuotations | Searcher::StateCheckSingleQuotations |
                               Searcher::StateCheckSpecialQuotations);
            searcher.setSource(tmp);
        }


    }
    stream.setString(new QString(data.bashrc));
    while(stream.readLineInto(&tmp))
    {
        Searcher searcher(tmp, Searcher::StateCheckDoubleQuotations | Searcher::StateCheckSingleQuotations |
                          Searcher::StateCheckSpecialQuotations);

        QRegularExpression aliasRegExp("[\\s]{0,}alias[\\s]+[\\w]+='|\"[\\s\\S]{0,}'|\"");

        int pos;
        while(CHECK_SEARCH((pos = searcher.search(aliasRegExp))))
        {
            AliasData adata;
            adata.inBashrc = true;
            tmp = tmp.mid(pos);
            QString work = tmp;
            searcher.setSource(work);
            int chopPos = searcher.search("alias");
            work = work.mid(chopPos + 5);
            searcher.setSource(work);
            work = work.mid(searcher.search(QRegularExpression("\\S")));
            adata.alias = work.mid(0, searcher.search('=')-1);
            work = work.mid(searcher.search('=')+1);
            searcher.setSource(work);
            bool doubleQuotes = true;
            if(work.length() < 1)
            {
                continue;
            }
            if(work.at(0) != '"')
                doubleQuotes = false;
#define QUOTE (doubleQuotes) ? '"' : '\''
            work = work.mid(1);
            searcher.setSource(work);
            adata.command = work.mid(0, searcher.search(QUOTE));
            aliases << adata;
            tmp = work.mid(searcher.search(QUOTE)+1);
#undef QUOTE
            searcher.setStates(Searcher::StateCheckDoubleQuotations | Searcher::StateCheckSingleQuotations |
                               Searcher::StateCheckSpecialQuotations);
            searcher.setSource(tmp);
        }


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
        adata.inBashrc = dynamic_cast<AliasTabTableWidgetItem*>(ui->tableWidget_Aliases)->info().toBool();
    }

    for(AliasData adata : aliases)
    {
        if(adata.inBashrc)
        {
            //TODO make a smart system for detecting if a alias exsists but the command changed
            Searcher searcher(rtn.bashrc);
            if(CHECK_SEARCH(searcher.search(QRegularExpression(tr("[\\s]{0,}alias[\\s]+") + adata.alias + "=(\"|')" + adata.command + "(\"|')"))))
            {
                continue;
            }
            adata.command.replace('\'', "\\\'");
            rtn.bashrc.append(tr("alias ") + adata.alias + "='" + adata.command + "'");
        }
        else
        {
            adata.alias.replace('\'', "\\'");
            rtn.program.append(tr("alias ") + adata.alias + tr("='") + adata.command + tr("'"));
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

QVariant AliasTabTableWidgetItem::info()
{
    return m_info;
}

#undef CHECK_SEARCH
