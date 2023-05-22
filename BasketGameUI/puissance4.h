#ifndef PUISSANCE4_H
#define PUISSANCE4_H

#include <QObject>
#include <QVector>

/**
 * @def TEST_ALIGNEMENT
 * @brief Pour tester les vérifications de combinaison
 */
#define TEST_ALIGNEMENT

/**
 * @def NB_PIONS_ALIGNES
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

/**
 * @def NB_PIONS
 * @brief Définit le nombre de pions max
 */
#define NB_PIONS (NB_LIGNES * NB_COLONNES)

class Puissance4 : public QObject
{
    Q_OBJECT
  public:
    explicit Puissance4(QObject* parent = 0);
    void initialiserPuissance4();
    int  placerPion(int colonne);
    void verifierPuissance4();
    bool estVainqueur() const;
    bool estEquipeRouge() const;
    void setTourEquipe(bool estEquipeRouge);

  public slots:
#ifdef TEST_ALIGNEMENT
    void testUnitaireVerifierPuissance4();
#endif

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

    QVector<QVector<CouleurJeton> > puissance4;
    bool                            vainqueur;
    bool                            equipeRouge;

    bool verifierLigne();
    bool verifierColonne();
    bool verifierDiagonaleMontante();
    bool verifierDiagonaleDescendante();
};

#endif // PLATEAU_H
