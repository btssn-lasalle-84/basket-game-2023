-- LMD (langage de manipulation de données)

-- Contenu des tables (tests)

-- Table Joueur

INSERT INTO Joueur(idJoueur, nom) VALUES (null,'Air Jordan');
INSERT INTO Joueur(idJoueur, nom) VALUES (null,'King James');
INSERT INTO Joueur(idJoueur, nom) VALUES (null,'The Good Lord');
INSERT INTO Joueur(idJoueur, nom) VALUES (null,'Magic');
INSERT INTO Joueur(idJoueur, nom) VALUES (null,'Black Mamba');
INSERT INTO Joueur(idJoueur, nom) VALUES (null,'TP');

-- Table Match

INSERT INTO Match(idMatch, idJoueurA, idJoueurB, nbManchesGagnantes, estFinie, horodatage) VALUES (NULL,1,2,2,0,'2023-03-29 08:00:00');

-- Table Manche

INSERT INTO Manche(idMatch, numeroManche, nbTirsReussisEquipeA, nbTirsReussisEquipeB, vainqueur, debut) VALUES (1,1,4,3,1,'2023-03-29 08:15:00');
INSERT INTO Manche(idMatch, numeroManche, nbTirsReussisEquipeA, nbTirsReussisEquipeB, vainqueur, debut) VALUES (1,2,1,4,2,'2023-03-29 08:20:00');
INSERT INTO Manche(idMatch, numeroManche, nbTirsReussisEquipeA, nbTirsReussisEquipeB, vainqueur, debut) VALUES (1,3,4,0,1,'2023-03-29 08:45:00');
