#ifndef CHAMBER_H
#define CHAMBER_H

#include <vector>

#include "chip.h"

class Chamber
{
public:
    Chamber(QString);
    void addChip(Chip*);
    std::vector<Chip*> getChips() const;
    QString getName();

private:
    QString name;
    std::vector<Chip*> chips;

};

#endif // CHAMBER_H
