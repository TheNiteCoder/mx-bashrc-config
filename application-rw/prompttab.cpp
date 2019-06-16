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
    Searcher searcher(&program, Searcher::StateCheckDoubleQuotations |
                      Searcher::StateCheckSingleQuotations |
                      Searcher::StateCheckSpecialQuotations);
    int promptKeywordStart = searcher.search("prompt-");
    if(!CHECK_SEARCH(promptKeywordStart))
    {
        ui->comboBox_SelectPromptProvider->setCurrentText("Default");
        return;
    }
    else
    {
        ui->comboBox_SelectPromptProvider->setCurrentText("Fancy Prompt");
    }

    int promptTypeStart = promptKeywordStart + tr("prompt-").size();

    int promptTypeEnd = searcher.search(' ', promptTypeStart);
    if(!CHECK_SEARCH(promptTypeEnd))
    {
        return;
    }

    QString promptType = searcher.source().mid(promptTypeStart, promptTypeEnd-promptTypeStart);
    promptType[0] = promptType.at(0).toUpper();
    ui->comboBox_SelectFancyPrompt->setCurrentText(promptType);

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
        flags[flag]->setChecked(CHECK_SEARCH(search));
    }

    QMap<QString, QLineEdit*> textOpts;
    textOpts["--date=\""] = ui->lineEdit_DateFormatText;
    textOpts["--time=\""] = ui->lineEdit_TimeFormatText;
    textOpts["--prompt=\""] = ui->lineEdit_PromptText;
    textOpts["--title=\""] = ui->lineEdit_TitleText;

    for(QString flag : textOpts.keys())
    {
        int search = searcher.search(flag);
        search += flag.length();
        int search2 = searcher.search('\"', search);
        textOpts[flag]->setText(searcher.source().mid(search, search2 - search));
    }

    QMap<QString, QSpinBox*> numOpts;
    numOpts["--right=\""] = ui->spinBox_RightMargin;
    numOpts["--lines=\""] = ui->spinBox_ExtraNewlinesBeforePrompt;

    for(QString flag : numOpts.keys())
    {
        int search = searcher.search(flag);
        search += flag.length();
        int search2 = searcher.search('\"', search);
        numOpts[flag]->setMinimum(0);
        numOpts[flag]->setValue(searcher.source().mid(search, search2 - search).toInt());
    }

}

BashrcSource PromptTab::exec(const BashrcSource data)
{
    DEBUG_VAR(data.program);
    BashrcSource rtn;
    rtn.bashrc = data.bashrc;
    rtn.program = data.program;

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
    return rtn;
}
#undef CHECK_SEARCH
