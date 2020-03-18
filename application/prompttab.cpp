#include "prompttab.h"

#include "searcher.h"
#include <QMap>
#include <QMessageBox>
#include <QProcess>

#include <QVBoxLayout>
#include <QColorDialog>
#include <QPushButton>
#include <QPalette>
#include <QDialogButtonBox>
#include <QGroupBox>
#include <QCheckBox>

#include <iostream>

PromptTab::PromptTab()
    : Tab("Prompt")
{
    DEBUG_ENTER(PromptTab::PromptTab);
    ui = new Ui::PromptTab;
    setWidget(new QWidget);
    ui->setupUi(widget());

    connect(ui->comboBox_SelectPromptProvider, &QComboBox::currentTextChanged, [=](QString text){
        Q_UNUSED(text)
        if (ui->comboBox_SelectPromptProvider->currentIndex() == 1)
        {
            ui->stackedWidget->setCurrentIndex(0);
        }
        else if(ui->comboBox_SelectPromptProvider->currentIndex() == 0)
        {
            ui->stackedWidget->setCurrentIndex(1);
        }
        else if(ui->comboBox_SelectPromptProvider->currentIndex() ==  2)
        {
            ui->stackedWidget->setCurrentIndex(2);
        }
    });

    connect(ui->pushButton_PromptCustomAdd, &QPushButton::clicked, [=](){
        QListWidgetItem* item = CustomItemSelectorDialog::getItem(widget());
        if(item != nullptr)
        {
            ui->listWidget_PromptCustom->addItem(item);
        }
    });

    connect(ui->pushButton_PromptCustomRemove, &QPushButton::clicked, [=](){
        QList<QListWidgetItem*> items = ui->listWidget_PromptCustom->selectedItems();
        for(QListWidgetItem* item : items)
        {
            if(item != nullptr) delete ui->listWidget_PromptCustom->takeItem(ui->listWidget_PromptCustom->row(item));
        }
    });

    connect(ui->listWidget_PromptCustom, &QListWidget::itemDoubleClicked, [=](QListWidgetItem* item){
        CustomPromptItem* citem = dynamic_cast<CustomPromptItem*>(item);
        CustomPromptItemEditor::edit(citem);
    });

    DEBUG_EXIT(PromptTab::PromptTab);
}

PromptTab::~PromptTab()
{
    DEBUG_ENTER(PromptTab::~PromptTab);
    delete widget();
    DEBUG_EXIT(PromptTab::~PromptTab);
}

void PromptTab::setup(const BashrcSource data)
{
    DEBUG_ENTER(PromptTab::setup);
    QString program = data.program;
    /* Maybe check in bashrc in case they don't want to reconfigure their fancy prompt */
    Searcher searcher(&program, Searcher::StateCheckDoubleQuotations |
                      Searcher::StateCheckSingleQuotations |
                      Searcher::StateCheckSpecialQuotations);
    int promptKeywordStart = searcher.search("prompt-");
    if(CHECK_SEARCH(searcher.search(customPromptCommentString)))
    {
        QString promptString;
        QRegularExpressionMatchIterator iter = QRegularExpression("^\\s{0,}export\\s+PS1=\"(.+)\"\\s{0,}$").globalMatch(&program);
        while(iter.hasNext())
        {
            QRegularExpressionMatch match = iter.next();
            promptString = match.captured(1);
        }
        DEBUG << "promptString: " << promptString;
        // R"()" is for a raw string C++ 11 feature note: () are neccarry
        iter = QRegularExpression(R"(\\\[\\003\[([0-9;]+)m\\\])").globalMatch(promptString);
    }
    else if(!CHECK_SEARCH(promptKeywordStart))
    {
        ui->comboBox_SelectPromptProvider->setCurrentText("Default");
        ui->stackedWidget->setCurrentIndex(1);
        DEBUG << "Selected Default Prompt Provider";
        QRegularExpression regexFindPS1("PS1=('|\")(.{0,})(\\1)");
        // need to do the same for the program's bashrc
        QRegularExpressionMatchIterator iter = regexFindPS1.globalMatch(data.program);
        QRegularExpressionMatch lastMatch;
        bool foundPS1 = iter.hasNext();
        while(iter.hasNext())
        {
            lastMatch = iter.next();
        }
        QString programQuotedText = lastMatch.captured(2);
        DEBUG_VAR(programQuotedText);
        // removing stderr because bash says an error through stderr: bash: cannot set terminal process group (2761): Inappropriate ioctl for device\nbash: no job control in this shell
        ExecuteResult result = runCmd("echo -n \"$PS1\"", true, true); // -n for removing the newline and \"'s for keeping leading whitespace
        DEBUG_VAR(result.output);

        if(!result.output.isEmpty())
        {
            QString PS1 = result.output;
            DEBUG_VAR(PS1);
            if(programQuotedText == PS1.remove("\\n") && foundPS1)
            {
                ui->checkBox_RemoveAllNewlines->setChecked(true);
            }
            else
            {
                ui->checkBox_RemoveAllNewlines->setChecked(false);
            }
        }
        else DEBUG << "PS1 variable not found in QProcess bash -i";
    }
    else
    {
        DEBUG << "Selected Fancy Prompt Prompt Provider";
        ui->comboBox_SelectPromptProvider->setCurrentText("Fancy Prompt");
        program.append("source /usr/local/bin/fancy-prompt.bash\n");
        ui->stackedWidget->setCurrentIndex(0);
        QString promptsize = "prompt-";
        int promptTypeStart = promptKeywordStart + promptsize.size();

        int promptTypeEnd = searcher.search(' ', promptTypeStart);
        if(!CHECK_SEARCH(promptTypeEnd))
        {
            DEBUG_EXIT(PromptTab::setup);
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
        flags["--compact2"] = ui->checkBox_CompactLargePrompts2;
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
    DEBUG_EXIT(PromptTab::setup);
}

int rgbToAnsi256(QColor color)
{
    int ansi256 = 1;
    // taken from: https://stackoverflow.com/questions/15682537/ansi-color-specific-rgb-sequence-bash
    bool finishedAnsi256 = false;
    if(color.red() == color.green() && color.green() == color.blue())
    {
        if(color.red() < 8)
        {
            if(!finishedAnsi256)
                ansi256 = 16;
            finishedAnsi256 = true;
        }
        if(color.red() > 248)
        {
            if(!finishedAnsi256)
                ansi256 = 231;
            finishedAnsi256 = true;
        }
        if(!finishedAnsi256)
            ansi256 = qRound(static_cast<float>((color.red() - 8) / 247) * 24) + 232;
    }
    if(!finishedAnsi256)
    {
        ansi256 = 16 +
                (36 * qRound(static_cast<float>(color.red() / 255 * 5))) +
                (6 * qRound(static_cast<float>(color.green() / 255 * 5))) +
                qRound(static_cast<float>(color.blue() / 255 * 5));
    }

    return ansi256;
}

BashrcSource PromptTab::exec(const BashrcSource data)
{
    DEBUG_ENTER(PromptTab::exec);
    BashrcSource rtn;
    // Copy contructor seems to work may be issue in future
    rtn = data;

    QString promptCommand;

    if(ui->comboBox_SelectPromptProvider->currentText() == "Fancy Prompt")
    {
        promptCommand.append("source /usr/local/bin/fancy-prompts.bash");
        promptCommand.append("\nprompt-");
        promptCommand.append(ui->comboBox_SelectFancyPrompt->currentText().toLower());
        promptCommand.append(' ');
        promptCommand.append((ui->checkBox_BoldLines->isChecked()) ? "--bold " : "");
        promptCommand.append((ui->checkBox_CompactLargePrompts->isChecked()) ? "--compact " : "");
        promptCommand.append((ui->checkBox_CompactLargePrompts2->isChecked()) ? "--compact2 " : "");
        promptCommand.append((ui->checkBox_DisableUnicode->isChecked()) ? "--ascii " : "");
        promptCommand.append((ui->checkBox_DoubleLines->isChecked()) ? "--double " : "");
        promptCommand.append((ui->checkBox_MutedColors->isChecked()) ? "--mute " : "");
        promptCommand.append((ui->checkBox_NoColor->isChecked()) ? "--nocolor " : "");
        promptCommand.append((ui->checkBox_ParensInstead->isChecked()) ? "--parens " : "");
        promptCommand.append(QString(" --lines=\"%1\"").arg(ui->spinBox_ExtraNewlinesBeforePrompt->value()));
        promptCommand.append(QString(" --right=\"%1\"").arg(ui->spinBox_RightMargin->value()));
        promptCommand.append(QString(" --date=\"%1\"").arg(ui->lineEdit_DateFormatText->text()));
        promptCommand.append(QString(" --time=\"%1\"").arg(ui->lineEdit_TimeFormatText->text()));
        promptCommand.append(QString(" --prompt=\"%1\"").arg(ui->lineEdit_PromptText->text()));
        promptCommand.append(QString(" --title=\"%1\"").arg(ui->lineEdit_TitleText->text()));
        rtn.program.append("\n");
        rtn.program.append(promptCommand);
    }
    else if(ui->comboBox_SelectPromptProvider->currentText() == "Custom")
    {
        QString lines;
        lines.append("export PS1=\"\"\n");
        for(int i = 0; i < ui->listWidget_PromptCustom->count(); i++)
        {
            QString code{""};
            CustomPromptItem* item = static_cast<CustomPromptItem*>(ui->listWidget_PromptCustom->item(i));
            /// stringifying item
            if(item->type() == CustomPromptItemType::SimpleText)
            {
                SimpleTextItem* obj = static_cast<SimpleTextItem*>(item);
                QColor foreground = obj->propertyForeground().first;
                QColor background = obj->propertyBackground().first;
                bool foregroundEnabled = obj->propertyForeground().second;
                bool backgroundEnabled = obj->propertyBackground().second;
                bool bold = obj->propertyBold();
                QString text = obj->propertyString();
                int ansi256 = rgbToAnsi256(foreground);
            //    int ansi16 = (color.red()*6/256)*36 + (color.green()*6/256)*6 + (color.blue()*6/256);

                if(bold)
                {
                    code.append("export PS1=\"$(tput bold)\"\n");
                }

                if(foregroundEnabled)
                {
                    code.append("if [ $COLORTERM == \"truecolor\" ]; then\n");
                    code.append(QString("\texport PS1=\"$PS1\\[\\033[38;2;%1;%2;%3m\\]\"\n").arg(foreground.red()).arg(foreground.green()).arg(foreground.blue()));
                    code.append("else\n");
                    code.append(QString("\texport PS1=\"$PS1\\[\\033[38;5;%1m\\]\"\n").arg(ansi256));
                    code.append("fi\n");
                }

                ansi256 = rgbToAnsi256(background);
                if(backgroundEnabled)
                {
                    code.append("if [ $COLORTERM == \"truecolor\" ]; then\n");
                    code.append(QString("\texport PS1=\"$PS1\\[\\033[48;2;%1;%2;%3m\\]\"\n").arg(background.red()).arg(background.green()).arg(background.blue()));
                    code.append("else\n");
                    code.append(QString("\texport PS1=\"$PS1\\[\\033[48;5;%1m\\]\"\n").arg(ansi256));
                    code.append("fi\n");
                }
                // add text

                code.append(QString("export PS1=\"$PS1%1\"\n").arg(text));

                lines.append(code);
            }
            else if(item->type() == CustomPromptItemType::Special)
            {
                SpecialItem* obj = static_cast<SpecialItem*>(item);
                QColor foreground = obj->propertyForeground().first;
                QColor background = obj->propertyBackground().first;
                bool foregroundEnabled = obj->propertyForeground().second;
                bool backgroundEnabled = obj->propertyBackground().second;
                bool bold = obj->propertyBold();
                auto type = obj->itemType();
                int ansi256 = rgbToAnsi256(foreground);
            //    int ansi16 = (color.red()*6/256)*36 + (color.green()*6/256)*6 + (color.blue()*6/256);

                if(bold)
                {
                    code.append("export PS1=\"$(tput bold)\"\n");
                }

                if(foregroundEnabled)
                {
                    code.append("if [ $COLORTERM == \"truecolor\" ]; then\n");
                    code.append(QString("\texport PS1=\"$PS1\\[\\033[38;2;%1;%2;%3m\\]\"\n").arg(foreground.red()).arg(foreground.green()).arg(foreground.blue()));
                    code.append("else\n");
                    code.append(QString("\texport PS1=\"$PS1\\[\\033[38;5;%1m\\]\"\n").arg(ansi256));
                    code.append("fi\n");
                }
                ansi256 = rgbToAnsi256(background);
                if(backgroundEnabled)
                {
                    code.append("if [ $COLORTERM == \"truecolor\" ]; then\n");
                    code.append(QString("\texport PS1=\"$PS1\\[\\033[48;2;%1;%2;%3m\\]\"\n").arg(background.red()).arg(background.green()).arg(background.blue()));
                    code.append("else\n");
                    code.append(QString("\texport PS1=\"$PS1\\[\\033[48;5;%1m\\]\"\n").arg(ansi256));
                    code.append("fi\n");
                }
                // add text

                code.append(QString("export PS1=\"$PS1%1\"\n").arg(SpecialItem::typeToString(type)));

                lines.append(code);

            }
        }
        // this stops the escape squences from applying anymore
        lines.append("export PS1=\"$PS1$(tput sgr0)\"\n");
        rtn.program.append(lines);
    }
    else
    {
        if(ui->checkBox_RemoveAllNewlines->isChecked())
        {
            ExecuteResult result = runCmd("echo -n \"$PS1\"", true, true); // -n for removing the newline and \"'s for keeping leading whitespace
            if(!result.output.isEmpty())
            {
                QString PS1 = result.output;
                DEBUG_VAR(PS1);
                rtn.program.append(QString("PS1=\"%1\"").arg(PS1.remove("\\n")));
            }
            else
            {
                int ui = QMessageBox::warning(widget(), NAME + QString(" - Warning"), QString("No Prompt Configurations Found! Can Not Remove Newlines"), QMessageBox::Ok, QMessageBox::NoButton);
                goto end;
            }
        }
    }
    end:
    DEBUG_EXIT(PromptTab::exec);
    return rtn;
}

CustomPromptProperty::CustomPromptProperty(QString name, QObject* parent)
    : QObject(parent),
      m_name(name)
{
}

CustomPromptProperty::~CustomPromptProperty()
{
    if(m_widget != nullptr) delete m_widget;
}

ColorProperty::ColorProperty(QString name, QObject* parent)
    : CustomPromptProperty(name, parent)
{
    setWidget(new QWidget);
    widget()->setLayout(new QHBoxLayout(widget()));
    m_enabledColor = false;
    m_enabled = new QCheckBox("Enabled", widget());
    connect(m_enabled, &QCheckBox::clicked, [=](){
        m_enabledColor = m_enabled->isChecked();
        m_colorBtn->setDisabled(!m_enabledColor);
    });
    m_colorBtn = new QPushButton(widget());
    connect(m_colorBtn, &QPushButton::clicked, this, &ColorProperty::onBtnPress);
    m_colorBtn->setFlat(true);
    m_colorBtn->setText("");
    m_colorBtn->setAutoFillBackground(true);
    m_color = QColor(0,0,0);
    updateBtn();
    widget()->layout()->addWidget(m_enabled);
    widget()->layout()->addWidget(m_colorBtn);
}

bool ColorProperty::good()
{
    return true;
}

void ColorProperty::updateBtn()
{
    QPalette pal = m_colorBtn->palette();
    pal.setColor(QPalette::Button, m_color);
    m_colorBtn->setPalette(pal);
    m_colorBtn->update();
}

void ColorProperty::onBtnPress()
{
    m_color = QColorDialog::getColor(m_color);
    updateBtn();
}

CheckboxProperty::CheckboxProperty(QString name, QObject *parent)
    : CustomPromptProperty(name, parent)
{
    m_checkbox = new QCheckBox(name);
    setWidget(m_checkbox);
}

bool CheckboxProperty::good()
{
    return true;
}

TextProperty::TextProperty(QString name, QObject *parent)
    : CustomPromptProperty(name, parent)
{
    m_editor = new QLineEdit();
    setWidget(m_editor);
}

bool TextProperty::good()
{
    return true;
}

SimpleTextItem::SimpleTextItem(QString name)
    : CustomPromptItem(name)
{
    m_properties.push_back(new ColorProperty(QObject::tr("Foreground Color")));
    m_properties.push_back(new ColorProperty(QObject::tr("Background Color")));
    m_properties.push_back(new CheckboxProperty(QObject::tr("Bold")));
    m_properties.push_back(new TextProperty(QObject::tr("Text")));
}

void SimpleTextItem::save()
{
    foreground = static_cast<ColorProperty*>(properties()[0])->color();
    background = static_cast<ColorProperty*>(properties()[1])->color();
    foregroundEnabled = static_cast<ColorProperty*>(properties()[0])->enabled();
    backgroundEnabled = static_cast<ColorProperty*>(properties()[1])->enabled();
    bold = static_cast<CheckboxProperty*>(properties()[2])->checked();
    str = static_cast<TextProperty*>(properties()[3])->text();
    setText(str);
    if(backgroundEnabled) setBackgroundColor(background);
    if(foregroundEnabled) setTextColor(foreground);
}


QString SimpleTextItem::stringify()
{
    QString rtn;
    rtn.append("\\[\\033[");
    if(bold) rtn.append("1;");
    rtn.append(QString("38;2;%1;%2;%3m\\]").arg(foreground.red()).arg(foreground.green()).arg(foreground.blue()));
    rtn.append(QString("48;2;%1;%2;%3m\\]").arg(background.red()).arg(background.green()).arg(background.blue()));
    rtn.append(str);
    rtn.append("\\[\\033[0m\\]");
    return rtn;
}

QListWidgetItem *CustomItemSelectorDialog::getItem(QWidget *parent)
{
    Q_UNUSED(parent)
    QDialog dialog;
    dialog.setLayout(new QVBoxLayout);
    QListWidget* list = new QListWidget;
    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    QObject::connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    QObject::connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    list->addItem("Text");
    list->addItem("Hostname (Long)");
    list->addItem("Hostname (Short)");
    list->addItem("Username");
    list->addItem("Current Directory (Long)");
    list->addItem("Current Directory (Short)");
    dialog.layout()->addWidget(list);
    dialog.layout()->addWidget(buttonBox);
    int result = dialog.exec();
    if(result == QDialog::Rejected) return nullptr;
    if(list->currentItem()->text() == "Text")
    {
        return new SimpleTextItem("Text");
    }
    else if(list->currentItem()->text() == "Hostname (Long)")
    {
        return new SpecialItem("", SpecialItem::Type::HostLong);
    }
    else if(list->currentItem()->text() == "Hostname (Short)")
    {
        return new SpecialItem("", SpecialItem::Type::HostShort);
    }
    else if(list->currentItem()->text() == "Username")
    {
        return new SpecialItem("", SpecialItem::Type::Username);
    }
    else if(list->currentItem()->text() == "Current Directory (Long)")
    {
        return new SpecialItem("", SpecialItem::Type::WorkingLong);
    }
    else if(list->currentItem()->text() == "Current Directory (Short)")
    {
        return new SpecialItem("", SpecialItem::Type::WorkingShort);
    }
    return nullptr;
}

CustomPromptItem::CustomPromptItem(QString name)
{
    setText(name);
}

void CustomPromptItemEditor::edit(CustomPromptItem *item, QWidget* parent)
{
    QDialog* dialog = new QDialog;
    dialog->setParent(parent);
    dialog->setModal(true);

    dialog->setLayout(new QVBoxLayout);
    for(CustomPromptProperty* prop : item->properties())
    {
        QGroupBox* box = new QGroupBox(prop->name());
        box->setLayout(new QVBoxLayout);
        box->layout()->addWidget(prop->widget());
        dialog->layout()->addWidget(box);
    }
    std::cout << "Debug" << std::endl;
    QDialogButtonBox* btnBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    QObject::connect(btnBox, &QDialogButtonBox::rejected, dialog, &QDialog::reject);
    QObject::connect(btnBox, &QDialogButtonBox::accepted, dialog, &QDialog::accept);
    dialog->layout()->addWidget(btnBox);
    int result = dialog->exec();
    if(result == QDialog::Accepted) item->save();
}

SpecialItem::SpecialItem(QString name, SpecialItem::Type type)
    : CustomPromptItem(name)
{
    m_type = type;
    switch(type)
    {
    case Type::HostLong:
        setText("Hostname (Long)");
        break;
    case Type::HostShort:
        setText("Hostname (Short)");
        break;
    case Type::Username:
        setText("Username");
        break;
    case Type::WorkingLong:
        setText("Current Directory(Long)");
        break;
    case Type::WorkingShort:
        setText("Current Directory(Short");
        break;
    }
    m_properties.push_back(new ColorProperty("Foreground"));
    m_properties.push_back(new ColorProperty("Background"));
    m_properties.push_back(new CheckboxProperty("Bold"));
}

void SpecialItem::save()
{
    foreground = static_cast<ColorProperty*>(properties()[0])->color();
    background = static_cast<ColorProperty*>(properties()[1])->color();
    foregroundEnabled = static_cast<ColorProperty*>(properties()[0])->enabled();
    backgroundEnabled = static_cast<ColorProperty*>(properties()[1])->enabled();
    bold = static_cast<CheckboxProperty*>(properties()[2])->checked();
    setBackgroundColor(background);
    setTextColor(foreground);
}

QString SpecialItem::stringify()
{
    QString rtn;
    rtn.append("\\[\\033[");
    if(bold) rtn.append("1;");
    rtn.append(QString("38;2;%1;%2;%3m\\]").arg(foreground.red()).arg(foreground.green()).arg(foreground.blue()));
    rtn.append(QString("48;2;%1;%2;%3m\\]").arg(background.red()).arg(background.green()).arg(background.blue()));
    switch(m_type)
    {
    case Type::HostLong:
        rtn.append("\\h");
        break;
    case Type::HostShort:
        rtn.append("\\H");
        break;
    case Type::Username:
        rtn.append("\\u");
        break;
    case Type::WorkingLong:
        rtn.append("\\w");
        break;
    case Type::WorkingShort:
        rtn.append("\\W");
        break;
    }
    rtn.append("\\[\\033[0m\\]");
    return rtn;
}

CustomPromptProperty* CustomPromptItem::propertyByName(QString name)
{
    for(CustomPromptProperty* prop : properties())
    {
        if(prop->name() == name)
        {
            return prop;
        }
    }
    return nullptr;
}

QString ColorProperty::stringify(QColor color) const
{
    int ansi256 = 1;
    // taken from: https://stackoverflow.com/questions/15682537/ansi-color-specific-rgb-sequence-bash
    bool finishedAnsi256 = false;
    if(color.red() == color.green() && color.green() == color.blue())
    {
        if(color.red() < 8)
        {
            if(!finishedAnsi256)
                ansi256 = 16;
            finishedAnsi256 = true;
        }
        if(color.red() > 248)
        {
            if(!finishedAnsi256)
                ansi256 = 231;
            finishedAnsi256 = true;
        }
        if(!finishedAnsi256)
            ansi256 = qRound(static_cast<float>((color.red() - 8) / 247) * 24) + 232;
    }
    if(!finishedAnsi256)
    {
        ansi256 = 16 +
                (36 * qRound(static_cast<float>(color.red() / 255 * 5))) +
                (6 * qRound(static_cast<float>(color.green() / 255 * 5))) +
                qRound(static_cast<float>(color.blue() / 255 * 5));
    }

//    int ansi16 = (color.red()*6/256)*36 + (color.green()*6/256)*6 + (color.blue()*6/256);

    QString code;

    code.append("if [ $COLORTERM == \"truecolor\" ]; then\n");
    code.append(QString("\texport PS1=\"$PS1\\x1b[38;2;%1;%2;%3m\"\n").arg(color.red()).arg(color.green()).arg(color.blue()));
    code.append("else\n");
    code.append(QString("\texport PS1=\"$PS1\\x1b[38;5;%1m").arg(ansi256));
    code.append("fi\n");


    return QString();
}

QString SpecialItem::typeToString(Type t)
{
#define CASE(a,b) case Type::a: return b;
    switch(t)
    {
    CASE(HostLong, "\\H")
    CASE(HostShort, "\\h")
    CASE(Username, "\\u")
    CASE(WorkingLong, "\\w")
    CASE(WorkingShort, "\\W")
    }
#undef CASE
}
