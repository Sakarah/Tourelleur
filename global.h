#ifndef GLOBAL_H
#define GLOBAL_H

#include "prologin.h"

#define HOME position(0, 0)
#define FONTAINE_TOP position(0, 15)
#define FONTAINE_LEFT position(15, 0)
#define FONTAINE_BOTTOM position(30, 15)
#define FONTAINE_RIGHT position(15, 30)
#define ARTEFACT position(15, 15)

position remap(position pos);
position nearestEnemyTower(position pos);
int countEnemyTowerAttacking(position pos);
int countEnemyAround(position pos, int radius);

inline int abs(int n) { return n > 0 ? n : -n; }
inline int manhatanDist(position a, position b) { return abs(a.x - b.x) + abs(a.y - b.y); }

inline bool operator==(const position& a, const position& b) { return a.x == b.x && a.y == b.y; }
inline bool operator!=(const position& a, const position& b) { return !(a==b); }
inline bool operator<(const position& a, const position& b) { return a.x*TAILLE_TERRAIN + a.y < b.x*TAILLE_TERRAIN + b.y; }

#endif // GLOBAL_H
