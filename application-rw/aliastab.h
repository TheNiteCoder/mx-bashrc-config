#ifndef ALIASTAB_H
#define ALIASTAB_H

#include "tab.h"
#include "ui_aliastab.h"

namespace Ui
{
class AliasTab;
}

struct AliasData
{
    QString alias;
    QString command;
    bool inBashrc;
};

//special spec for a table item class so it can hold some data
class AliasTabTableWidgetItem : public QObject, public QTableWidgetItem
{
    Q_OBJECT
public:
    AliasTabTableWidgetItem() = default;
    AliasTabTableWidgetItem(QString text, QVariant info = QVariant());
    ~AliasTabTableWidgetItem();
    AliasTabTableWidgetItem& setInfo(QVariant info);
    QVariant info();
protected:
    QVariant m_info;
};

class AliasTab : public Tab
{
    Q_OBJECT
public:
    AliasTab();
    virtual ~AliasTab();

    void setup(const BashrcSource data);
    BashrcSource exec(const BashrcSource data);
protected:
   Ui::AliasTab* ui;
};

#endif // ALIASTAB_H
