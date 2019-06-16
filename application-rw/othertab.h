#ifndef OTHERTAB_H
#define OTHERTAB_H

#include "tab.h"

class OtherTab : public Tab
{
public:
    OtherTab();
    void setup(const BashrcSource data);
    BashrcSource exec(const BashrcSource data);
};

#endif // OTHERTAB_H
