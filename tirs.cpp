#include "global.h"
#include <iostream>

/* Les tirs sont gérés de façon a eviter au maximum la présence de sorciers proche de la base ou des autres tours.
 * On créé une échelle de dangerosité qui prend en compte ces deux facteurs pour chaque case.
 * Chaque tour dépense ses points d'actions de façon a attaquer les enemis du plus 'dangereux' au moins 'dangereux'.
 */

int dangerMap[TAILLE_TERRAIN][TAILLE_TERRAIN];
int enemyMap[TAILLE_TERRAIN][TAILLE_TERRAIN];

/**
 * @fn void increaseZoneDanger(position pos)
 * Augmente localement la dangerosité des cases
 * @param pos centre de la zone autour de laquelle augmenter le danger
 */
void increaseZoneDanger(position pos)
{
    dangerMap[pos.x][pos.y] += 10;
    if(pos.x >= 0) dangerMap[pos.x - 1][pos.y] += 5;
    if(pos.y >= 0) dangerMap[pos.x][pos.y - 1] += 5;
    if(pos.x + 1 < TAILLE_TERRAIN) dangerMap[pos.x + 1][pos.y] += 5;
    if(pos.y + 1 < TAILLE_TERRAIN) dangerMap[pos.x][pos.y + 1] += 5;
}

/**
 * @fn void initDangerMap()
 * Construit une carte de dangerosité basée sur l'éloignement à la base et la position des fontaines
 */
void initDangerMap()
{
    for(int x = 0 ; x < TAILLE_TERRAIN ; x++)
    {
        for(int y = 0 ; y < TAILLE_TERRAIN ; y++)
        {
            int danger = 50 - manhatanDist(HOME, position(x, y));
            dangerMap[x][y] = danger;
        }
    }

    increaseZoneDanger(FONTAINE_TOP);
    increaseZoneDanger(FONTAINE_LEFT);
    increaseZoneDanger(ARTEFACT);
    increaseZoneDanger(FONTAINE_RIGHT);
    increaseZoneDanger(FONTAINE_BOTTOM);
}

/**
 * @fn void initEnemyMap()
 * Initialise la carte des ennemis
 */
void initEnemyMap()
{
    for(int x = 0 ; x < TAILLE_TERRAIN ; x++)
    {
        for(int y = 0 ; y < TAILLE_TERRAIN ; y++)
        {
            int player = joueur_case(remap(position(x, y)));
            if(!player || player == moi()) enemyMap[x][y] = 0;
            else
            {
                int wiz = nb_sorciers(remap(position(x, y)), player);
                enemyMap[x][y] = wiz;
            }
        }
    }
}

/**
 * @fn std::vector<position> getShootable(position from, int radius)
 * Renvoie tous les ennemis attaquable depuis une position sur un certain rayon
 * @param from Position a partir de laquelle attaquer
 * @param radius Rayon d'attaque
 * @return Liste des positions des ennemis attaquables
 */
std::vector<position> getShootable(position from, int radius)
{
    std::vector<position> result;
    int deltaY = -1;
    //std::cout << from.x << '$' << from.y << '$' << radius << " ";
    for(int x = from.x - radius ; x <= from.x + radius ; x++)
    {
        if(x > from.x) deltaY -= 1;
        else deltaY += 1;
        if(x < 0 || x >= TAILLE_TERRAIN) continue;

        for(int y = from.y - deltaY ; y <= from.y + deltaY ; y++)
        {
            if(y < 0 || y >= TAILLE_TERRAIN) continue;
            if(enemyMap[x][y])
            {
                result.push_back(position(x, y));
            }
        }
    }

    return result;
}

/**
 * @fn void phase_tirs()
 * Fonction appelée pendant la phase de tirs des tourelles
 */
void phase_tirs()
{
    initDangerMap();
    initEnemyMap();

    std::vector<tourelle> tourelles = tourelles_joueur(moi());
    for(tourelle t : tourelles) increaseZoneDanger(remap(t.pos));
    for(tourelle t : tourelles)
    {
        std::vector<position> shootable = getShootable(remap(t.pos), t.portee);
        bool end = false;
        while(t.attaque && !end)
        {
            position maxRisk = position(-1, -1);
            int riskMax = -1;
            int toKill = 0;
            for(position e : shootable)
            {
                int risk = dangerMap[e.x][e.y];
                if(risk > riskMax)
                {
                    maxRisk = e;
                    riskMax = risk;
                    toKill = enemyMap[e.x][e.y];
                }
            }

            if(riskMax == -1) end = true;
            else
            {
                if(toKill > t.attaque) toKill = t.attaque;
                t.attaque -= toKill;
                tirer(toKill, t.pos, remap(maxRisk));
            }
        }
    }
}
