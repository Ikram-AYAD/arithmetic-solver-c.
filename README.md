# Le Compte est Bon — Jeu en C

> 🇫🇷 Français | [🇬🇧 English below](#le-compte-est-bon--c-game)

---

## Description

Implémentation en C du jeu télévisé **Le Compte est Bon**.  
Le joueur dispose de 6 nombres et doit atteindre une cible comprise entre 100 et 999 en combinant ces nombres avec les opérations +, -, *, /.

## Fonctionnalités

- Génération aléatoire des nombres et de la cible
- Mode joueur interactif (saisie des opérations en console)
- Intelligence artificielle par exploration récursive exhaustive
- Affichage détaillé des étapes de calcul de l'IA
- Comparaison automatique entre le résultat du joueur et celui de l'IA

## Technologies

- Langage : **C (C99)**
- Compilation : **GCC**
- Librairies standard : `stdio.h`, `stdlib.h`, `string.h`, `time.h`, `limits.h`

## Lancer le projet

```bash
make
./compte_est_bon
```

Ou sans Makefile :

```bash
gcc compte_est_bon.c -o compte_est_bon
./compte_est_bon
```

## Règles du jeu

1. Une cible entre 100 et 999 est générée aléatoirement
2. Vous disposez de 6 nombres (petits : 1–10, grands : 25, 50, 75, 100)
3. À chaque étape, choisissez deux nombres et une opération : `a op b`
4. Le résultat remplace les deux nombres utilisés
5. Continuez jusqu'à n'avoir plus qu'un seul nombre
6. Tapez `0 0 0` pour abandonner et laisser l'IA jouer

## Projet académique

Projet réalisé en groupe dans le cadre d'un cours de **Programmation en Langage C**.  

**Mon rôle :** développement du code, implémentation de l'algorithme récursif de l'IA, rédaction du rapport, tests et relecture.

---

# Le Compte est Bon — C Game

> [🇫🇷 Français ci-dessus](#le-compte-est-bon--jeu-en-c) | 🇬🇧 English

## Description

A C implementation of the French TV game show **Le Compte est Bon** (similar to *Countdown*).  
The player is given 6 numbers and must reach a target between 100 and 999 using +, -, *, / operations.

## Features

- Random generation of numbers and target
- Interactive player mode (console input)
- AI solver using exhaustive recursive backtracking
- Step-by-step display of the AI's solution
- Automatic comparison between player result and AI result

## Technologies

- Language: **C (C99)**
- Compiler: **GCC**
- Standard libraries: `stdio.h`, `stdlib.h`, `string.h`, `time.h`, `limits.h`

## Run the project

```bash
make
./compte_est_bon
```

Or without Makefile:

```bash
gcc compte_est_bon.c -o compte_est_bon
./compte_est_bon
```

## Game Rules

1. A target between 100 and 999 is randomly generated
2. You have 6 numbers (small: 1–10, large: 25, 50, 75, 100)
3. Each turn, pick two numbers and an operation: `a op b`
4. The result replaces the two numbers used
5. Continue until only one number remains
6. Type `0 0 0` to give up and let the AI play

## Academic Project

Group project completed as part of a **C Programming** course.  

**My role:** code development, recursive AI algorithm implementation, report writing, testing and proofreading.
