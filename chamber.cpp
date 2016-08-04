#include "chamber.h"

Chamber::Chamber(QString newname)
{
    name = newname;
}

void Chamber::addChip(Chip* newchip)
{
    chips.push_back(newchip);
}

std::vector<Chip*> Chamber::getChips() const
{
    return chips;
}

QString Chamber::getName()
{
    return name;
}
