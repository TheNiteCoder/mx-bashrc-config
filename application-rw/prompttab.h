#ifndef PROMPTTAB_H
#define PROMPTTAB_H

#include "tab.h"
#include "ui_prompttab_fix.h"
namespace Ui{
class PromptTab;
}

class PromptTab : public Tab
{
public:
    PromptTab();
    ~PromptTab();
    void setup(const BashrcSource data);
    BashrcSource exec(const BashrcSource data);
protected:
    Ui::PromptTab* ui;
};

#endif // PROMPTTAB_H
