![C++ Badge](https://img.shields.io/badge/C%2B%2B-00599C?logo=cplusplus&logoColor=fff&style=plastic) ![Android Badge](https://img.shields.io/badge/Android-3DDC84?logo=android&logoColor=fff&style=plastic)

![](https://badgen.net/badge/Qt/5.12.8/green) [![qt-build](https://github.com/btssn-lasalle-84/basket-game-2023/actions/workflows/make-qt.yml/badge.svg)](https://github.com/btssn-lasalle-84/basket-game-2023/actions/workflows/make-qt.yml) [![android-build](https://github.com/btssn-lasalle-84/basket-game-2023/actions/workflows/android-build.yml/badge.svg)](https://github.com/btssn-lasalle-84/basket-game-2023/actions/workflows/android-build.yml) [![pages-build-deployment](https://github.com/btssn-lasalle-84/basket-game-2023/actions/workflows/pages/pages-build-deployment/badge.svg?branch=develop)](https://github.com/btssn-lasalle-84/basket-game-2023/actions/workflows/pages/pages-build-deployment)

# Le projet basket-game 2023

- [Le projet basket-game 2023](#le-projet-basket-game-2023)
  - [Documentation du code](#documentation-du-code)
  - [Versions](#versions)
    - [1.0](#10)
    - [0.2](#02)
    - [0.1](#01)
  - [Diagramme de classes](#diagramme-de-classes)
  - [Planification](#planification)
  - [Simulateur](#simulateur)
  - [Auteurs](#auteurs)

---

**Basket Game** est un système numérique dédié à la phase d'entraînement au panier de Basket-ball afin d'ajouter un aspect ludique et esprit de compétition. Il propose de reproduire le jeu Puissance 4 (© Hasbro) en alignant plusieurs paniers de basket, chacun représentant une colonne du jeu. Deux équipes (ou 2 joueurs) lancent le ballon à tour de rôle. Chaque panier correspond à 1 jeton dans le jeu, de la couleur de l’équipe. Les équipes devront donc envoyer le ballon dans le bon panier afin de créer l’alignement de jetons, tout en bloquant l’adversaire. Un écran (de télévision) affichera la partie en cours.

![](images/logo-basketgame.png)

- Module de détection des paniers (EC)
- Module de signalisation (EC)​
- Module de gestion de partie (Android - IR)​
- Module de visualisation de partie (Qt - Raspberry Pi - Écran télévision - IR)​

![](images/basketgame-2023.png)

![](images/uc-basketgame-2023.png)

- Android :

| Fonctionnalité   | OUI | NON |
| ------------------------- |:-----:|:-----:|
| Paramétrer une partie | X |  |
| Gérer le déroulement d’une partie | - |  |
| Dialoguer avec les modules | X |  |
| Jouer une partie | - |  |

- Qt :

| Fonctionnalité   | OUI | NON |
| ------------------------- |:-----:|:-----:|
| Afficher le déroulement d’une partie | - |  |
| Gérer le puissance 4 | X |  |
| Dialoguer avec le module de gestion de partie | X |  |
| Produire des sons en fonction de la situation |  | X |


## Documentation du code

https://btssn-lasalle-84.github.io/basket-game-2023/


## Versions

![](images/livraisons-basketgame-2023.png)

### 1.0

- Android :

![](images/screenshot-android-nouvelle-partie-v1.0.png)

![](images/screenshot-android-parametres-v1.0.png)

![](images/screenshot-android-partie-suivi-v1.0.png)

![](images/screenshot-android-partie-interrompue-v1.0.png)

- Qt :

![](images/screenshot-qt-accueil-basketgame-v0.2.png)

![](images/screenshot-qt-puissance4-basketgame-v1.0.png)

![](images/screenshot-qt-puissance4-1-basketgame-v1.0.png)

![](images/screenshot-qt-puissance4-2-basketgame-v1.0.png)

### 0.2

![](images/jira-tickets-v0.2.png)

### 0.1

![](images/jira-tickets-v0.1.png)

## Diagramme de classes

- Android :

![](images/dc-basketgame-android-2023.png)

- Qt :

![](images/dc-basketgame-qt-2023.png)

## Planification

![](images/gantt-basketgame-2023.png)

![](images/roadbook-basketgame-2023.png)

## Simulateur

![](images/simulateur-2023.png)

Lien : [Protocole BASKET](simulateur/README.md)

## Auteurs

- Étudiant IR (Qt) : Nathanael CHANSARD <chansardnathanael@gmail.com>
- Étudiant IR (Android) : Florent GUILLAUMET <florent.guillaumet.pro@gmail.com>

---
©️ LaSalle Avignon 2023
