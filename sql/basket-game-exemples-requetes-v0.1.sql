-- Exemples de requêtes SQL

-- Liste des joueurs

SELECT * FROM Joueur;

-- Nombre de joueurs

SELECT COUNT(*) AS NbJoueurs FROM Joueur;

-- Liste des matchs

SELECT * FROM Match;

SELECT * FROM Match 
INNER JOIN Joueur a ON (a.idJoueur = Match.idJoueurA) 
INNER JOIN Joueur b ON (b.idJoueur = Match.idJoueurB);

-- Détails des matchs

SELECT Match.idMatch, Match.nbManchesGagnantes, Match.tempsMaxTir, Match.nbPaniers, Match.horodatage, Match.estFinie, Match.idJoueurA, JoueurA.nom, Match.idJoueurB, JoueurB.nom FROM Match 
INNER JOIN Joueur JoueurA ON (JoueurA.idJoueur = Match.idJoueurA) 
INNER JOIN Joueur JoueurB ON (JoueurB.idJoueur = Match.idJoueurB);

-- Liste des manches

SELECT * FROM Manche;

-- Liste de toutes les manches d'un match

SELECT * FROM Manche 
INNER JOIN Match ON Manche.idMatch = Match.idMatch 
INNER JOIN Joueur JoueurA ON (JoueurA.idJoueur = Match.idJoueurA) 
INNER JOIN Joueur JoueurB ON (JoueurB.idJoueur = Match.idJoueurB) 
WHERE Match.idMatch = '1';

---- Commencer une manche

INSERT INTO Manche(idMatch,numeroManche,nbTirsReussisEquipeA,nbTirsReussisEquipeB, debut) VALUES (1,1,0,0,DATETIME('now'));

---- Finir une manche et enregistrer le vainqueur

UPDATE Manche SET fin=DATETIME('now'), nbTirsReussisEquipeA='4', nbTirsReussisEquipeB='1', vainqueur='7' WHERE idMatch='1' AND numeroManche='3';

-- Terminer un match

-- UPDATE Match SET estFinie=1 WHERE idMatch='1';
