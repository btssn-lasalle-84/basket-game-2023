![C++ Badge](https://img.shields.io/badge/C%2B%2B-00599C?logo=cplusplus&logoColor=fff&style=plastic) ![](https://badgen.net/badge/Qt/5.12.8/green) ![Raspberry Pi Badge](https://img.shields.io/badge/Raspberry%20Pi-A22846?logo=raspberrypi&logoColor=fff&style=plastic) ![Java Badge](https://img.shields.io/badge/Java-ED8B00?style=for-the-badge&logo=java&logoColor=white&style=plastic) ![Android Badge](https://img.shields.io/badge/Android-3DDC84?logo=android&logoColor=fff&style=plastic)

[![pages-build-deployment](https://github.com/btssn-lasalle-84/basket-game-2023/actions/workflows/pages/pages-build-deployment/badge.svg?branch=develop)](https://github.com/btssn-lasalle-84/basket-game-2023/actions/workflows/pages/pages-build-deployment)

# Le projet basket-game-2023

---

## Documentation du code

https://btssn-lasalle-84.github.io/basket-game-2023/

## Base de données

Base de données SQLite :

![](sql/basket-game-v0.1.png)

```sql
-- Supprime les tables

DROP TABLE IF EXISTS Manche;
DROP TABLE IF EXISTS Match;
DROP TABLE IF EXISTS Joueur;

-- Création des tables

-- Table Joueur

CREATE TABLE IF NOT EXISTS Joueur(idJoueur INTEGER PRIMARY KEY AUTOINCREMENT, nom VARCHAR, UNIQUE(nom));

-- Table Match

CREATE TABLE IF NOT EXISTS Match(idMatch INTEGER PRIMARY KEY AUTOINCREMENT, idJoueurA INTEGER NOT NULL, idJoueurB INTEGER NOT NULL, nbManchesGagnantes INTEGER DEFAULT 1, tempsMaxTir INTEGER DEFAULT 0, nbPaniers INTEGER DEFAULT 4, estFinie INTEGER DEFAULT 0, horodatage DATETIME NOT NULL, CONSTRAINT fk_idJoueur_A FOREIGN KEY (idJoueurA) REFERENCES Joueur(idJoueur), CONSTRAINT fk_idJoueur_B FOREIGN KEY (idJoueurB) REFERENCES Joueur(idJoueur));

-- Table Manche

CREATE TABLE IF NOT EXISTS Manche(idMatch INTEGER NOT NULL, numeroManche INTEGER NOT NULL, nbTirsReussisEquipeA INTEGER DEFAULT 0, nbTirsReussisEquipeB INTEGER DEFAULT 0, vainqueur INTEGER DEFAULT 0, debut DATETIME NOT NULL, fin DATETIME, CONSTRAINT pk_manche PRIMARY KEY (idMatch,numeroManche), CONSTRAINT fk_idMatch_1 FOREIGN KEY (idMatch) REFERENCES Match(idMatch), CONSTRAINT fk_vainqueur_1 FOREIGN KEY (vainqueur) REFERENCES Joueur(idJoueur));
```

---
©️ LaSalle Avignon 2023
