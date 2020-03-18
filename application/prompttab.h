#ifndef PROMPTTAB_H
#define PROMPTTAB_H

#include "tab.h"
#include "ui_prompttab_fix.h"

#include <QListWidgetItem>
#include <QCheckBox>
#include <QLineEdit>
#include <QDialog>

class QPushButton;
class QLineEdit;

namespace Ui{
class PromptTab;
}

enum class CustomPromptItemType
{
    Base = 0,
    SimpleText,
    Special,
};

enum class CustomPromptPropertyType
{
    Base = 0,
    Color,
    Checkbox,
    Text,
};

class CustomPromptProperty : public QObject
{
    Q_OBJECT
public:
    CustomPromptProperty(QString name, QObject* parent = nullptr);
    virtual ~CustomPromptProperty();
    QWidget* widget() { return m_widget; }
    void setWidget(QWidget* widget) { m_widget = widget; }
    QString name() const { return m_name; }
    void setName(QString name) { m_name = name; }
    virtual bool good() = 0; // reporting that all inputs are good
    virtual CustomPromptPropertyType type() const { return CustomPromptPropertyType::Base; }
protected:
    QWidget* m_widget;
    QString m_name;
};

class ColorProperty : public CustomPromptProperty
{
    Q_OBJECT
public:
    ColorProperty(QString name, QObject* parent = nullptr);
    bool good() override;
    void setColor(QColor color) { m_color = color; updateBtn(); }
    QColor color() const { return m_color; }
    bool enabled() const { return m_enabledColor; }
    QString stringify(QColor color) const;
    CustomPromptPropertyType type() const override { return CustomPromptPropertyType::Color; }
private slots:
    void onBtnPress();
private:
    void updateBtn();
    QPushButton* m_colorBtn;
    QColor m_color;
    QCheckBox* m_enabled;
    bool m_enabledColor;
};

class CheckboxProperty : public CustomPromptProperty
{
    Q_OBJECT
public:
    CheckboxProperty(QString name, QObject* parent = nullptr);
    bool good() override;
    void setChecked(bool checked) { m_checkbox->setChecked(checked); }
    bool checked() { return m_checkbox->isChecked(); }
    CustomPromptPropertyType type() const override { return CustomPromptPropertyType::Checkbox; }
private:
    QCheckBox* m_checkbox;
};

class TextProperty : public CustomPromptProperty
{
    Q_OBJECT
public:
    TextProperty(QString name, QObject* parent = nullptr);
    bool good() override;
    void setText(QString text) { m_editor->setText(text); }
    QString text() { return m_editor->text(); }
    CustomPromptPropertyType type() const override { return CustomPromptPropertyType::Text; }
private:
    QLineEdit* m_editor;
};

class CustomPromptItem : public QListWidgetItem
{
public:
    CustomPromptItem(QString name);
    QVector<CustomPromptProperty*> properties() const { return m_properties; }
    CustomPromptProperty* propertyByName(QString name);
    virtual void save() = 0;
    virtual QString stringify() { return QString{}; }
    virtual CustomPromptItemType type() const { return CustomPromptItemType::Base; }
protected:
    QVector<CustomPromptProperty*> m_properties;
};

class SimpleTextItem : public CustomPromptItem
{
public:
    SimpleTextItem(QString name);
    void save() override;
    QString stringify() override;
    CustomPromptItemType type() const override { return CustomPromptItemType::SimpleText; }
    QPair<QColor, bool> propertyForeground() const { return {foreground, foregroundEnabled}; }
    QPair<QColor, bool> propertyBackground() const { return {background, backgroundEnabled}; }
    QString propertyString() const { return str; }
    bool propertyBold() const { return bold; }
protected:
    QColor foreground, background;
    QString str;
    bool bold, foregroundEnabled, backgroundEnabled;
};

class SpecialItem : public CustomPromptItem
{
public:
    enum class Type
    {
        HostLong,
        HostShort,
        Username,
        WorkingLong,
        WorkingShort,
    };
    static QString typeToString(Type t);
    SpecialItem(QString name, Type type);
    void save() override;
    QString stringify() override;
    Type itemType() const { return m_type; }
    QPair<QColor, bool> propertyForeground() const { return {foreground, foregroundEnabled}; }
    QPair<QColor, bool> propertyBackground() const { return {background, backgroundEnabled}; }
    bool propertyBold() const { return bold; }
    CustomPromptItemType type() const override { return CustomPromptItemType::Special; }
protected:
    QColor foreground, background;
    bool bold, foregroundEnabled, backgroundEnabled;
    Type m_type;
};

class CustomItemSelectorDialog
{
public:
    static QListWidgetItem* getItem(QWidget* parent = nullptr);
};

class CustomPromptItemEditor
{
public:
    static void edit(CustomPromptItem* item, QWidget* parent = nullptr);
};

class PromptTab : public Tab
{
public:
    PromptTab();
    ~PromptTab();
    void setup(const BashrcSource data);
    BashrcSource exec(const BashrcSource data);
protected:
    Ui::PromptTab* ui;
    const QString customPromptCommentString = "#BASH_CONFIG_USING_CUSTOM_PROMPT";
};

#endif // PROMPTTAB_H
