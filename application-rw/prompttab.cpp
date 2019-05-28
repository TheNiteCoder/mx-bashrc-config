#include "prompttab.h"

#include "searcher.h"
#include <QMap>

PromptTab::PromptTab()
    : Tab("Prompt")
{
    ui = new Ui::PromptTab;
    setWidget(new QWidget);
    ui->setupUi(widget());
}

PromptTab::~PromptTab()
{
    delete widget();
}

void PromptTab::setup(const BashrcSource data)
{
    QString program = data.program;
    /* Maybe check in bashrc in case they don't want to reconfigure their fancy prompt */
#define CHECK_SEARCH(x) (x == Searcher::ReturnValueSearchStatesFailed || x == Searcher::ReturnValueSearchStringNotFound)
    Searcher searcher(program, Searcher::StateCheckDoubleQuotations |
                      Searcher::StateCheckSingleQuotations |
                      Searcher::StateCheckSpecialQuotations);
    int promptKeywordStart = searcher.search("prompt-");
    if(CHECK_SEARCH(promptKeywordStart))
    {
        //put control code for incase no fancy prompt is found
        return;
    }
    int promptTypeStart = promptKeywordStart + tr("prompt-").size();

    DEBUG << "PromptTab::setup: promptTypeStart: " << promptTypeStart;

    int promptTypeEnd = searcher.search(' ', promptTypeStart);
    if(CHECK_SEARCH(promptTypeEnd))
    {
        //put control code incase parsing for fancy prompt fails
        return;
    }

    QMap<QString, QCheckBox*> flags;
    flags["--ascii"] = ui->checkBox_DisableUnicode;
    flags["--mute"] = ui->checkBox_MutedColors;
    flags["--bold"] = ui->checkBox_BoldLines;
    flags["--compact"] = ui->checkBox_CompactLargePrompts;
    flags["--double"] = ui->checkBox_DoubleLines;
    flags["--nocolor"] = ui->checkBox_NoColor;
    flags["--parens"] = ui->checkBox_ParensInstead;

    for(QString flag : flags.keys())
    {
        int search = searcher.search(flag);
        flags[flag]->setChecked(!CHECK_SEARCH(search));
    }

    int search;
    search = searcher.search("--date=\"");
    if(CHECK_SEARCH(search))
    {
        int search2 = searcher.search('\"', search+1);
        if(CHECK_SEARCH(search2))
        {
            ui->lineEdit_DateFormatText->setText(searcher.source().mid(search, search2 - search));
        }
    }
    search = searcher.search("--lines=\"");
    if(CHECK_SEARCH(search))
    {
        int search2 = searcher.search('\"', search+1);
        if(CHECK_SEARCH(search2))
        {
            ui->spinBox_ExtraNewlinesBeforePrompt->setValue(searcher.source().mid(search, search2 - search).toInt());
        }
    }
    search = searcher.search("--prompt=\"");
    if(CHECK_SEARCH(search))
    {
        int search2 = searcher.search('\"', search+1);
        if(CHECK_SEARCH(search2))
        {
            ui->lineEdit_PromptText->setText(searcher.source().mid(search, search2 - search));
        }
    }
    search = searcher.search("--right=\"");
    if(CHECK_SEARCH(search))
    {
        int search2 = searcher.search('\"', search+1);
        if(CHECK_SEARCH(search2))
        {
            ui->spinBox_RightMargin->setValue(searcher.source().mid(search, search2 - search).toInt());
        }
    }
    search = searcher.search("--time=\"");
    if(CHECK_SEARCH(search))
    {
        int search2 = searcher.search('\"', search+1);
        if(CHECK_SEARCH(search2))
        {
            ui->lineEdit_TimeFormatText->setText(searcher.source().mid(search, search2 - search));
        }
    }
    search = searcher.search("--title=\"");
    if(CHECK_SEARCH(search))
    {
        int search2 = searcher.search('\"', search+1);
        if(CHECK_SEARCH(search2))
        {
            ui->lineEdit_TitleText->setText(searcher.source().mid(search, search2 - search));
        }
    }


#undef CHECK_SEARCH
}

BashrcSource PromptTab::exec(const BashrcSource data)
{
    BashrcSource rtn = data;

    QString promptCommand;

    if(ui->comboBox_SelectPromptProvider->currentText() == "Fancy Prompt")
    {
        promptCommand.append("prompt-");
        promptCommand.append(ui->comboBox_SelectFancyPrompt->currentText().toLower());
        promptCommand.append(' ');
        promptCommand.append((ui->checkBox_BoldLines->isChecked()) ? "--bold " : "");
        promptCommand.append((ui->checkBox_CompactLargePrompts->isChecked()) ? "--compact " : "");
        promptCommand.append((ui->checkBox_DisableUnicode->isChecked()) ? "--ascii " : "");
        promptCommand.append((ui->checkBox_DoubleLines->isChecked()) ? "--double " : "");
        promptCommand.append((ui->checkBox_MutedColors->isChecked()) ? "--mute " : "");
        promptCommand.append((ui->checkBox_NoColor->isChecked()) ? "--nocolor " : "");
        promptCommand.append((ui->checkBox_ParensInstead->isChecked()) ? "--parens " : "");
        promptCommand.append(tr(" --lines=\"%1\"").arg(ui->spinBox_ExtraNewlinesBeforePrompt->value()));
        promptCommand.append(tr(" --right=\"%1\"").arg(ui->spinBox_RightMargin->value()));
        promptCommand.append(tr(" --date=\"%1\"").arg(ui->lineEdit_DateFormatText->text()));
        promptCommand.append(tr(" --time=\"%1\"").arg(ui->lineEdit_TimeFormatText->text()));
        promptCommand.append(tr(" --prompt=\"%1\"").arg(ui->lineEdit_PromptText->text()));
        promptCommand.append(tr(" --title=\"%1\"").arg(ui->lineEdit_TitleText->text()));
        rtn.program.append("\n");
        rtn.program.append(promptCommand);
    }
    else
    {
        //code for non fancy prompt
    }

    return data;
}
