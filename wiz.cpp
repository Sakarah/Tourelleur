#include "global.h"
#include "WizardGroup.h"
#include <iostream>

std::vector<WizardGroup*> wizGrp; // Vecteur de wizard

/**
 * @fn void phase_deplacement()
 * Fonction appelée pendant la phase de déplacement
 */
void phase_deplacement()
{
    // BO de base ; 10 sur la gauche puis 20 sur la droite
    if(tour_actuel() == 0)
    {
        WizardGroup* g = new WizardGroup(HOME);
        g->setDestination(FONTAINE_LEFT);
        wizGrp.push_back(g);
    }
    else if(tour_actuel() == 2)
    {
        WizardGroup* g = new WizardGroup(HOME);
        g->setDestination(FONTAINE_TOP);
        wizGrp.push_back(g);
    }
    // Le mode panique dans les déplacements est beaucoup trop lent pour résister à un rush
    /*else if(countEnemyAround(HOME, 9) > 40 || countEnemyAround(HOME, 5) > 20)
    {
        if(countEnemyAround(HOME, 9) - 40 > nb_sorciers(remap(HOME), moi()))
        {
            std::cout << "Panic !";
            for(WizardGroup* g : wizGrp)
            {
                g->setDestination(HOME);
            }
        }
    }*/
    // Ending ; Attaquer l'artéfact ou le joueur le possédant si il est inaccessible
    else if(tour_actuel() >= 90 && tour_actuel() <= 92)
    {
        position destinationFinale = HOME;

        if(!chemin(remap(HOME), ARTEFACT).size())
        {
            int joueurArtefact = joueur_case(ARTEFACT);
            position baseJoueurArtefact = remap(base_joueur(joueurArtefact));
            if(joueurArtefact == -1 || baseJoueurArtefact == position(30, 30))
            {
                int towerBase1 = countEnemyTowerAttacking(position(30, 0));
                int towerBase2 = countEnemyTowerAttacking(position(0, 30));
                if(towerBase1 < towerBase2) destinationFinale = position(30, 0);
                else destinationFinale = position(0, 30);
            }
            else destinationFinale = baseJoueurArtefact;
        }
        else destinationFinale = ARTEFACT;

        WizardGroup* g = new WizardGroup(HOME, nb_sorciers(remap(HOME), moi()) - 100);
        g->setDestination(destinationFinale);
        wizGrp.push_back(g);
    }
    // Cas général : garder le plus souvent possible les fontaines
    else
    {
        int menaceTourelleTop = countEnemyTowerAttacking(FONTAINE_TOP);
        int menaceTourelleLeft = countEnemyTowerAttacking(FONTAINE_LEFT);

        int nbEnemiesTop = nb_sorciers(remap(FONTAINE_TOP), joueur_case(remap(FONTAINE_TOP)));
        int allyRequiredTop = nbEnemiesTop + 11 + countEnemyAround(FONTAINE_TOP, 5)/2 + (menaceTourelleTop*10);
        if(joueur_case(remap(FONTAINE_TOP)) == moi()) allyRequiredTop = 0;

        int nbEnemiesLeft = nb_sorciers(remap(FONTAINE_LEFT), joueur_case(remap(FONTAINE_LEFT)));
        int allyRequiredLeft = nbEnemiesLeft + 11 + countEnemyAround(FONTAINE_LEFT, 5)/2 + (menaceTourelleLeft*10);
        if(joueur_case(remap(FONTAINE_LEFT)) == moi()) allyRequiredLeft = 0;


        int sorciersBase = nb_sorciers(remap(HOME), moi());

        if(allyRequiredLeft + allyRequiredTop < sorciersBase)
        {
            WizardGroup* gTop = new WizardGroup(HOME, allyRequiredTop);
            gTop->setDestination(FONTAINE_TOP);
            wizGrp.push_back(gTop);

            WizardGroup* gLeft = new WizardGroup(HOME, allyRequiredLeft);
            gLeft->setDestination(FONTAINE_LEFT);
            wizGrp.push_back(gLeft);
        }
        else
        {
            position destination = position(-1, -1);
            int allyRequired = 0;

            if(allyRequiredTop && allyRequiredTop < allyRequiredLeft)
            {
                destination = FONTAINE_TOP;
                allyRequired = allyRequiredTop;
            }
            else if(allyRequiredLeft)
            {
                destination = FONTAINE_LEFT;
                allyRequired = allyRequiredLeft;
            }
            // Pire que mieux : Les tourelles peuvent être reconstruites trop vite
            /*else if(menaceTourelleTop)
            {
                destination = nearestEnemyTower(FONTAINE_TOP);
                allyRequired = countEnemyTowerAttacking(destination)*30;
            }
            else if(menaceTourelleLeft)
            {
                destination = nearestEnemyTower(FONTAINE_LEFT);
                allyRequired = countEnemyTowerAttacking(destination)*30;
            }*/

            if(allyRequired >= sorciersBase) allyRequired = 0;
            if(allyRequired > 0)
            {
                std::cout << "\nTour sorcier " << allyRequired << "#" << allyRequiredTop << "#" << allyRequiredLeft;

                WizardGroup* g = new WizardGroup(HOME, allyRequired);
                g->setDestination(destination);
                wizGrp.push_back(g);
            }
        }
    }

    for(WizardGroup* g : wizGrp)
    {
        g->doAction();
    }
}

/**
 * @fn void phase_siege()
 * Fonction appelée pendant la phase de siège des tourelles
 */
void phase_siege()
{
    for(WizardGroup* g : wizGrp)
    {
        g->atkNearestTower();
    }
}
