#include "WizardGroup.h"
#include <map>
#include <iostream>

WizardGroup::WizardGroup(position initPos, int pow)
{
    pos = initPos;
    dest = initPos;
    power = pow;
}

WizardGroup::WizardGroup(position initPos)
{
    pos = initPos;
    dest = initPos;
    power = -1;
}

void WizardGroup::setDestination(position destination)
{
    dest = destination;
}

void WizardGroup::doAction()
{
    if(pos == dest) return;
    std::vector<position> path = chemin(remap(pos), remap(dest));
    position arriv;
    if(path.size() >= 4) arriv = path[3];
    else arriv = remap(dest);
    deplacer(remap(pos), arriv, getPow());
    pos = remap(arriv);
}

void WizardGroup::atkNearestTower()
{
    tourelle tToAtk;
    tToAtk.portee = -1;
    if(pos.x >= 0 && info_case(remap(position(pos.x-1, pos.y))) == CASE_TOURELLE)
        tToAtk = tourelle_case(remap(position(pos.x-1, pos.y)));
    if(pos.y >= 0 && info_case(remap(position(pos.x, pos.y-1))) == CASE_TOURELLE)
        tToAtk = tourelle_case(remap(position(pos.x, pos.y-1)));
    if(pos.x + 1 < TAILLE_TERRAIN && info_case(remap(position(pos.x+1, pos.y))) == CASE_TOURELLE)
        tToAtk = tourelle_case(remap(position(pos.x+1, pos.y)));
    if(pos.y + 1 < TAILLE_TERRAIN && info_case(remap(position(pos.x, pos.y+1))) == CASE_TOURELLE)
        tToAtk = tourelle_case(remap(position(pos.x, pos.y+1)));
    if(tToAtk.portee == -1 || tToAtk.joueur == moi()) return;

    std::cout << "AttackTower" << tToAtk.pos.x << '=' << tToAtk.pos.y;
    assieger(remap(pos), remap(tToAtk.pos), getPow());
}

int WizardGroup::getPow()
{
    int nbSorciers = nb_sorciers(remap(pos), moi());
    if(power == -1 || power > nbSorciers) return nbSorciers;
    return power;
}

// Code buggé de détection de chemin en évitant les tours
// TODO : La débugger puis l'utiliser pour les chemins des groupes de magiciens
/*std::vector<position> pathToAvoidingTowers(position from, position to)
{
    std::map<position, position> checked;
    checked[from] = from;
    std::vector<position> toCheck;
    toCheck.push_back(from);
    std::vector<position> nextToCheck;
    while(toCheck.size())
    {
        for(position p : toCheck)
        {
            if(p == to)
            {
                std::vector<position> solution;
                solution.push_back(to);
                while(solution.front() != from)
                {
                    solution.insert(solution.begin(), checked[solution.front()]);
                    return solution;
                }
            }
            if(p.x-1 >= 0)
            {
                position newP = p;
                newP.x -= 1;
                position towerNear = nearestEnemyTower(newP);
                if(!tourelle_case(towerNear).portee >= manhatanDist(newP, towerNear))
                {
                    checked[newP] = p;
                    nextToCheck.push_back(newP);
                }
            }
            if(p.y-1 >= 0)
            {
                position newP = p;
                newP.y -= 1;
                position towerNear = nearestEnemyTower(newP);
                if(!tourelle_case(towerNear).portee >= manhatanDist(newP, towerNear))
                {
                    checked[newP] = p;
                    nextToCheck.push_back(newP);
                }
            }
            if(p.x+1 <= TAILLE_TERRAIN)
            {
                position newP = p;
                newP.x += 1;
                position towerNear = nearestEnemyTower(newP);
                if(!tourelle_case(towerNear).portee >= manhatanDist(newP, towerNear))
                {
                    checked[newP] = p;
                    nextToCheck.push_back(newP);
                }
            }
            if(p.y+1 <= TAILLE_TERRAIN)
            {
                position newP = p;
                newP.y += 1;
                position towerNear = nearestEnemyTower(newP);
                if(!tourelle_case(towerNear).portee >= manhatanDist(newP, towerNear))
                {
                    checked[newP] = p;
                    nextToCheck.push_back(newP);
                }
            }
        }
        toCheck = nextToCheck;
        nextToCheck.clear();
    }

    return std::vector<position>();
}*/
