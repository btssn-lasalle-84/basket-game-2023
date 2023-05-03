#ifndef PLATEAU_H
#define PLATEAU_H

#include <vector>
#include <QWidget>
#include <QtWidgets>
#include <QTimer>

/**
 * @def NB_PANIERS
 * @brief Définit la constante du nombre de paniers de 2 à 7 (7 par défaut)
 */
#define NB_COLONNES 7 // colonnes

/**
 * @def NB_LIGNES
 * @brief Définit la constante du nombre de ligne
 */
#define NB_LIGNES 6 // rangées

/**
 * @def NB_PIONS_ALIGNES
 * @brief Définit la constante du nombre de pions par défaut à aligner pour gagner
 */

#define NB_PIONS_ALIGNES 4

#define LIBRE 0
#define ROUGE 1
#define JAUNE 2


struct Coup
{
    int ligne;
    int colonne;
    int numero;
};

class Plateau : public QWidget
{
    Q_OBJECT
public:
    explicit Plateau(QWidget *parent = 0);

public slots:

private:
    QVector<QVector<int> > plateau;


    void creerPlateau();
    void initialiserPlateau();
    void afficherPlateau();
    void attribuerRaccourcisClavier();

};

#endif // PLATEAU_H
