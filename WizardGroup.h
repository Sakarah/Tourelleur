#ifndef WIZARDGROUP_H
#define WIZARDGROUP_H

#include "global.h"

/**
 * @class WizardGroup
 * Un groupe de magicien correspond à un ensemble de jetteur de sort qui se déplacent ensemble vers un objectif commun
 */

class WizardGroup
{
public:
    WizardGroup(position initPos, int pow);
    WizardGroup(position initPos);
    void setDestination(position);
    void doAction();
    void atkNearestTower();
private:
    int getPow();
    int power;
    position pos;
    position dest;
};

#endif // WIZARDGROUP_H
