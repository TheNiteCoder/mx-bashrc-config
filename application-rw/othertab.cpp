#include "othertab.h"

OtherTab::OtherTab()
    : Tab("Other")
{

}

void OtherTab::setup(const BashrcSource data)
{
}

BashrcSource OtherTab::exec(const BashrcSource data)
{
    return data;
}
