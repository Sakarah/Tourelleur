Tourelleur est une IA codée en moins de 36 heures lors de l'édition Prologin de 2014.

Pour la tester il faut suivre les instructions de https://bitbucket.org/prologin/stechec2 pour lancer des parties sur le jeu prologin2014.

L'IA fonctionne sur un principe très défensif qui concentre son attaque unique vers la fin du jeu.
Son code source est divisé en 5 fichers + 2 header qui correspondent aux fonctions logiques du programme.
global.cpp (ici) est un fichier qui rassemble des fonctions utilisées à plusieurs endroit dans le programme.
construction.cpp traite de toute la phase construction et des fonctions qui lui sont propres.
wiz.cpp gère la stratégie globale des sorciers et donc agit dans les phases déplacement et siège
tirs.cpp s'occupe du tir des tourelles
WizardGroup.cpp est le gérant des mouvements de groupes de sorciers

En phase construction l'IA commence par une routine prédéfinie qui prend les deux fontaines adjacentes puis enchaine
sur la construction des 9 tourelles en fonction des disponibilités trésorières. Ensuite elle accumule les magiciens
sauf en cas de danger sur la base ou elle construit un wall défensif.

En phase mouvement les magiciens sont systématiquement envoyés sur les fontaines par groupes suffisant pour garantir la
prise du point stratégique. Sur les 10 derniers tours, ils rushent sur l'artéfact central pour essayer de s'emparer de
la victoire.

En phase tir les tourelles utilisent une carte de dangerosité pour déterminer quel est la cible prioritaire.

En phase siège les magiciens se contentent de détruire les tourelles a coté d'eux. (les magiciens ne recherchent pas
le siège donc cette partie de l'IA est peu développée).

Au final cette IA est arrivée 9ème au classement général.

L'ensemble de son code à l'exception des fichiers prologin.hh, prologin.cc et Makefile (sous GPLv2) est sous license Apache 2.0.
