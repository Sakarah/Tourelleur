#include "global.h"

/* README :
 * L'IA fonctionne sur un principe très défensif qui concentre son attaque unique vers la fin du jeu.
 * Son code source est divisé en 5 fichers + 2 header qui correspondent aux fonctions logiques du programme.
 * global.cpp (ici) est un fichier qui rassemble des fonctions utilisées à plusieurs endroit dans le programme.
 * construction.cpp traite de toute la phase construction et des fonctions qui lui sont propres.
 * wiz.cpp gère la stratégie globale des sorciers et donc agit dans les phases déplacement et siège
 * tirs.cpp s'occupe du tir des tourelles
 * WizardGroup.cpp est le gérant des mouvements de groupes de sorciers
 *
 * En phase construction l'IA commence par une routine prédéfinie qui prend les deux fontaines adjacentes puis enchaine
 * sur la construction des 9 tourelles en fonction des disponibilités trésorières. Ensuite elle accumule les magiciens
 * sauf en cas de danger sur la base ou elle construit un wall défensif.
 *
 * En phase mouvement les magiciens sont systématiquement envoyés sur les fontaines par groupes suffisant pour garantir la
 * prise du point stratégique. Sur les 10 derniers tours, ils rushent sur l'artéfact central pour essayer de s'emparer de
 * la victoire.
 *
 * En phase tir les tourelles utilisent une carte de dangerosité pour déterminer quel est la cible prioritaire.
 *
 * En phase siège les magiciens se contentent de détruire les tourelles a coté d'eux. (les magiciens ne recherchent pas
 * le siège donc cette partie de l'IA est peu développée).
 */

/**
 * @fn position remap(position pos)
 * Remappe la position comme si le joueur était le joueur 1
 * Cette fonction est a appeler sur toute les données entrantes ou sortantes de l'API
 * @param pos position à remapper
 * @return postion remappée
 */
position remap(position pos)
{
    bool horizontalRotation = false;
    bool verticalRotation = false;
    if(base_joueur(moi()).x) horizontalRotation = true;
    if(base_joueur(moi()).y) verticalRotation = true;

    if(horizontalRotation) pos.x = TAILLE_TERRAIN-1-pos.x;
    if(verticalRotation) pos.y = TAILLE_TERRAIN-1-pos.y;

    return pos;
}

/**
 * @fn position nearestEnemyTower(position pos)
 * Renvoie la position de la tourelle ennemie la plus proche
 * @param pos position à partir de laquelle rechercher
 * @return position de la tourelle la plus proche
 */
position nearestEnemyTower(position pos)
{
    position posFound(-1, -1);
    int radius = 1;
    while(posFound.x == -1 && radius < TAILLE_TERRAIN)
    {
        for(int x = pos.x - radius ; x < pos.x + radius ; x++)
        {
            for(int y = pos.y - radius ; y < pos.y + radius ; y++)
            {
                if(info_case(remap(position(x, y))) == CASE_TOURELLE && joueur_case(remap(position(x, y))) != moi())
                    posFound = position(x, y);
            }
        }
        radius++;
    }
    if(posFound.x == -1) return position(0, 0);
    return posFound;
}

/**
 * @fn int countEnemyTowerAttacking(position pos)
 * Compte le nombre de tourelles qui pourraient attaquer une position définie
 * @param pos position attaquable
 * @return le nombre de positions attaquables
 */
int countEnemyTowerAttacking(position pos)
{
    int count = 0;
    int deltaY = -1;
    for(int x = pos.x - 7 ; x <= pos.x + 7 ; x++)
    {
        if(x > pos.x) deltaY -= 1;
        else deltaY += 1;
        if(x < 0 || x >= TAILLE_TERRAIN) continue;

        for(int y = pos.y - deltaY ; y <= pos.y + deltaY ; y++)
        {
            if(y < 0 || y >= TAILLE_TERRAIN) continue;
            if(info_case(remap(position(x, y))) == CASE_TOURELLE)
            {
                tourelle t = tourelle_case(remap(position(x, y)));
                if(t.portee <= manhatanDist(t.pos, pos)) count++;
            }
        }
    }
    return count;
}

/**
 * @fn int countEnemyAround(position pos, int radius)
 * Compte le nombre d'ennemis autour d'une position donnée
 * @param pos position a partir de laquelle compter
 * @param radius rayon pour lequel comptabiliser le nombre d'ennemis
 * @return nombre d'ennemis autour de la position
 */
int countEnemyAround(position pos, int radius)
{
    int count = 0;
    int deltaY = -1;
    for(int x = pos.x - radius ; x <= pos.x + radius ; x++)
    {
        if(x > pos.x) deltaY -= 1;
        else deltaY += 1;
        if(x < 0 || x >= TAILLE_TERRAIN) continue;

        for(int y = pos.y - deltaY ; y <= pos.y + deltaY ; y++)
        {
            if(y < 0 || y >= TAILLE_TERRAIN) continue;

            int own = joueur_case(remap(position(x, y)));
            if(own == moi()) continue;
            count += nb_sorciers(remap(position(x, y)), own);
        }
    }
    return count;
}

/**
 * @fn void partie_debut()
 * Fonction appelée au début de la partie par l'API
 */
void partie_debut()
{
    // Aucune initialisation particulière...
}

/**
 * Fonction appelée à la fin de la partie par l'API
 */
void partie_fin()
{
    // Aucune libération particulière...
}

