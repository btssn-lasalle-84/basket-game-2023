#ifndef PLATEAU_H
#define PLATEAU_H

#include <vector>
#include <QWidget>
#include <QtWidgets>
#include <QTimer>

#define TEST_BASKETGAME

/**
 * @def DEPLACEMENT_Y
 * @brief Définit le nombre de pions pour gagner une partie
 */
#define NB_PIONS_ALIGNES 4
/**
 * @def NB_COLONNES
 * @brief Définit le nombre de colonne par défaut
 */
#define NB_COLONNES 7

/**
 * @def NB_LIGNES
 * @brief Définit le nombre de ligne par défaut
 */
#define NB_LIGNES 6

class Plateau : public QWidget
{
    Q_OBJECT
public:
    explicit Plateau(QWidget *parent = 0);
    void initialiserPlateau();
    void verifierPlateau();
    void verifierLigne();
    void verifierColonne();
    void verifierDiagonaleMontante();
    void verifierDiagonaleDescendante();
    int  randInt(int min, int max);
#ifdef TEST_BASKETGAME
    void testUnitaire();
#endif
public slots:
    void simulerPion();

private:

    /**
     * @enum CouleurJeton
     * @brief Les différents couleur de jeton
     */
    enum CouleurJeton
    {
        AUCUNE = 0,
        ROUGE,
        JAUNE,
        NB_COULEURS
    };

    QVector<QVector<CouleurJeton> > plateau;
    bool                            estVainqueur;
    bool                            estEquipeRouge;
};

#endif // PLATEAU_H
