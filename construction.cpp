#include "global.h"
#include <iostream>
#include <map>

/* Le pattern des tourelles est un L décalé de 1 vers le centre pour ne pas gêner la passage des sorciers.
 * Il permet de plus un wall facile en ne déplaçant que 2 tourelles.
 * On construit des tourelles adaptées a notre magie courante pour ne pas perdre de temps.
 * Les tourelles à 4 sont skippées car elles ne permettent pas de taper une fois de plus sur un sorcier avant
 * son arrivée au contact.
 * Les tourelles à 7 sont gardées pour une extension de portée lors de la protection des autres tours.
 *
 * La construction rapide de tourelle est importante pour gagner un avantage sur les fontaines rapidement.
 * C'est pour cela qu'après l'envoi de magiciens vers les deux fontaines, on ne fait que des tours.
 * L'alternance droite/gauche permet d'éviter des rushs tourelles sur notre base.
 */

const std::vector<position> wallPos = {position(0,3), position(1,2), position(2,1), position(3,0)};
bool panicMode = false;
bool sideLeft = false;

/**
 * @fn void towerConstruction()
 * Construit des tours selon un pattern en L autour de la base.
 */
void towerConstruction()
{
    if(panicMode)
    {
        std::cout << "Calm down";
        supprimer(remap(position(0, 3)));
        supprimer(remap(position(3, 0)));
        panicMode = false;
    }

    if(tourelles_joueur(moi()).size() >= 9) return;

    sideLeft = !sideLeft;
    position pos;

    if(!sideLeft)
    {
        pos = position(-1, 1);

        bool alignNextFound = false;
        while(!alignNextFound)
        {
            pos.x += 3;
            if(constructible(remap(pos), moi())) alignNextFound = true;
            else if(pos.x > 16)
            {
                pos.x = -1;
                pos.y += 3;
            }
        }
    }
    else
    {
        pos = position(1, -1);

        bool alignNextFound = false;
        while(!alignNextFound)
        {
            pos.y += 3;
            if(constructible(remap(pos), moi())) alignNextFound = true;
            else if(pos.y > 16)
            {
                pos.y = -1;
                pos.x += 3;
            }
        }
    }

    //std::cout << pos.x << "/" << pos.y;

    if(magie(moi()) > COUT_TOURELLE + COUT_PORTEE + 16) construire(remap(pos), PORTEE_TOURELLE + 4);
    else if(magie(moi()) > COUT_TOURELLE + COUT_PORTEE + 4) construire(remap(pos), PORTEE_TOURELLE + 2);
    else construire(remap(pos), PORTEE_TOURELLE);
}

/**
 * @fn void wizardBuild()
 * Dépense toute sa magie pour fabriquer des magiciens
 */
void wizardBuild()
{
    creer(magie(moi())/COUT_SORCIER);
}

/**
 * @fn void panicBuild()
 * Wall sur la diagonale à 4 case près de la base pour éviter une mort certaine face à un danger avéré
 */
void panicBuild()
{
    if(!panicMode)
    {
        //On supprimme les tourelles lointaines
        supprimer(remap(position(1, 11)));
        supprimer(remap(position(11, 1)));
        panicMode = true;
    }

    // On compte les trous de notre wall (et on supprimme les tours usées)
    std::vector<position> wallMissing;
    for(position w : wallPos)
    {
        if(info_case(remap(w)) != CASE_TOURELLE) wallMissing.push_back(w);
    }
    int wallCount = wallMissing.size();

    // On construit les meilleures tourelles possible pour waller la base
    if(magie(moi()) > (COUT_TOURELLE + COUT_PORTEE + 16) * wallCount)
    {
        for(position w : wallMissing) construire(remap(w), PORTEE_TOURELLE + 4);
    }
    else if(magie(moi()) > (COUT_TOURELLE + COUT_PORTEE + 4) * wallCount)
    {
        for(position w : wallMissing) construire(remap(w), PORTEE_TOURELLE + 2);
    }
    else
    {
        for(position w : wallMissing) construire(remap(w), PORTEE_TOURELLE);
    }
}

/**
 * @fn void phase_construction()
 * Fonction appelée pendant la phase de construction
 */
void phase_construction()
{
    if(tour_actuel() < 3) wizardBuild();
    else if(countEnemyAround(HOME, 9) > 40 || countEnemyAround(HOME, 5) > 20)
    {
        if(countEnemyAround(HOME, 9) - 40 > nb_sorciers(remap(HOME), moi()))
        {
            std::cout << "Panic !";
            panicBuild();
            wizardBuild();
        }
    }
    else
    {
        towerConstruction();
        wizardBuild();
    }
}
