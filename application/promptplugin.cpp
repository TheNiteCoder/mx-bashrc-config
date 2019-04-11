#include "promptplugin.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <algorithm>
#include "buffer.h"

PromptPlugin::PromptPlugin()
    : Plugin("Prompt", Plugin::DisplayFormat_Tab),
      ui(new Ui::PromptPlugin)
{
    ui->setupUi(widget());
    connect(ui->checkBox_UseFancyPrompts, &QCheckBox::clicked, [=](bool a){
        ui->widget_FancyPrompts->setDisabled(!a);
        ui->stackedWidget_Prompt->setCurrentIndex(!a);
    });
}

PromptPlugin::~PromptPlugin()
{
    delete ui;
}

QString PromptPlugin::exec(const QString source, const QString bashrcSource)
{
    QString copy = source;
    QString promptConfig;
    if(ui->checkBox_UseFancyPrompts->isChecked())
    {
        promptConfig.append("prompt-");
        promptConfig.append(ui->comboBox_SelectFancyPrompt->currentText().toLower());
        promptConfig.append(" ");
        promptConfig.append("--title=\"");
        promptConfig.append(ui->lineEdit_CenteredTitle->text());
        promptConfig.append("\" ");
        if(ui->checkBox_BoldLines->isChecked())
            promptConfig.append("--bold ");
        if(ui->checkBox_DisableUnicode->isChecked())
            promptConfig.append("--ascii ");
        if(ui->checkBox_DoubledLines->isChecked())
            promptConfig.append("--double ");
        if(ui->checkBox_LargerCompact->isChecked())
            promptConfig.append("--compact ");
        if(ui->checkBox_MutedColors->isChecked())
            promptConfig.append("--mute ");
        else
            promptConfig.append("--bright ");
        if(ui->checkBox_UseParens->isChecked())
            promptConfig.append("--parens ");
        if(ui->checkBox_NoColors->isChecked())
            promptConfig.append("--nocolor ");
        promptConfig.append(tr("--right=%1 ").arg(ui->spinBox_LeaveRightSpaces->value()));
        promptConfig.append(tr("--lines=%1 ").arg(ui->spinBox_LinesBeforePrompt->value()));
        promptConfig.append(tr("--time=\"%1\" ").arg(ui->lineEdit_TimeFormat->text()));
        promptConfig.append(tr("--date=\"%1\" ").arg(ui->lineEdit_DateFormat->text()));
        promptConfig.append(tr("--prompt=\"%1\" ").arg(ui->lineEdit_PromptText->text()));
    }
    else
    {

    }
    return source + promptConfig + "\n";
}

void PromptPlugin::setup(const QString source, const QString bashrcSource)
{
    QStringList lines = source.split('\n');
    for(QString line : lines)
    {
        if(line.contains("prompt-"))
        {
            ui->checkBox_UseFancyPrompts->setChecked(true);
            int startStart = line.indexOf("prompt-");
            int typeStart = startStart + tr("prompt-").length();
            int typeEnd = line.indexOf(' ', typeStart);
            QString type = line.mid(typeStart, typeEnd - typeStart);
            int fancyPromptCount = ui->comboBox_SelectFancyPrompt->count();
            bool typeGood = false;
            for(int i = 0; i < fancyPromptCount; i++)
            {
                if(type.toLower() == ui->comboBox_SelectFancyPrompt->itemText(i).toLower())
                {
                    typeGood = true;
                    ui->comboBox_SelectFancyPrompt->setCurrentIndex(i);
                    break;
                }

            }
            if(!typeGood)
            {
                //error
            }
            ui->checkBox_BoldLines->setChecked(line.contains("--bold"));
            ui->checkBox_DisableUnicode->setChecked(line.contains("--ascii"));
            ui->checkBox_DoubledLines->setChecked(line.contains("--double"));
            ui->checkBox_LargerCompact->setChecked(line.contains("--compact"));
            ui->checkBox_MutedColors->setChecked(line.contains("--mute"));
            ui->checkBox_NoColors->setChecked(line.contains("--nocolor"));
            ui->checkBox_UseParens->setChecked(line.contains("--parens"));
            if(line.contains("--title=\""))
            {
                int start = line.indexOf("--title=\"") + tr("--title=\"").length();
                int end = line.indexOf('\"', start);

                ui->lineEdit_CenteredTitle->setText(line.mid(start, end - start));
            }
            if(line.contains("--prompt=\""))
            {
                int start = line.indexOf("--prompt=\"") + tr("--prompt=\"").length();
                int end = line.indexOf('\"', start);
                ui->lineEdit_PromptText->setText(line.mid(start, end - start));
            }
            if(line.contains("--date=\""))
            {
                int start = line.indexOf("--date=\"") + tr("--date=\"").length();
                int end = line.indexOf('\"', start);
                ui->lineEdit_DateFormat->setText(line.mid(start, end - start));
            }
            if(line.contains("--time=\""))
            {
                int start = line.indexOf("--time=\"") + tr("--time=\"").length();
                int end = line.indexOf('\"', start);
                ui->lineEdit_TimeFormat->setText(line.mid(start, end - start));
            }
            if(line.contains("--lines="))
            {
                int start = line.indexOf("--lines=") + tr("--lines=").length();
                int end = line.indexOf(' ', start);
                ui->spinBox_LinesBeforePrompt->setValue(line.mid(start, end - start).toInt());
            }
            if(line.contains("--right="))
            {
                int start = line.indexOf("--right=") + tr("--right=").length();
                int end = line.indexOf(' ', start);
                ui->spinBox_LeaveRightSpaces->setValue(line.mid(start, end - start).toInt());
            }

        }
    }
}



