-- LDD (langage de définition de données)

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

-- Voir aussi :
-- ON DELETE CASCADE
