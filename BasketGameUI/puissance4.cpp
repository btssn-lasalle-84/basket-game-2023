/**
 * @file puissance4.cpp
 * @brief Définition de la classe Puissance4
 * @author Nathanael CHANSARD
 * @version 0.2
 */

#include "puissance4.h"
#include "basketgame.h"

/**
 * @fn Puissance4::Puissance4
 * @brief Constructeur de la classe Puissance4
 */
Puissance4::Puissance4(QObject* parent) :
    QObject(parent), plateau(NB_COLONNES), vainqueur(false), equipeRouge(true)
{
}

/**
 * @fn Puissance4::initialiserPlateau
 * @brief méthode pour initialiser le plateau de puissance4
 */
void Puissance4::initialiserPlateau()
{
    vainqueur = false;
    for(int i = 0; i < plateau.size(); ++i)
    {
        plateau[i].resize(NB_LIGNES);
    }

    qDebug() << Q_FUNC_INFO << "colonnes" << plateau.size() << "lignes"
             << plateau[0].size();

    for(int colonne = 0; colonne < plateau.size(); ++colonne)
    {
        for(int ligne = 0; ligne < plateau[colonne].size(); ++ligne)
        {
            plateau[colonne][ligne] = CouleurJeton::AUCUNE;
        }
    }
}

/**
 * @fn Puissance4::placerPion
 * @brief méthode qui lace un pion dans une des colonnes du plateau
 * @param colonne
 * @return ligne ou -1
 */
int Puissance4::placerPion(int colonne)
{
    int ligne = 0;
    for(ligne = 0; ligne < NB_LIGNES; ++ligne)
    {
        if(colonne >= 0 && colonne < NB_COLONNES &&
           plateau[colonne][ligne] == CouleurJeton::AUCUNE)
        {
            plateau[colonne][ligne] =
              (equipeRouge ? CouleurJeton::ROUGE : CouleurJeton::JAUNE);
            qDebug() << Q_FUNC_INFO << "pion"
                     << (equipeRouge ? CouleurJeton::ROUGE
                                     : CouleurJeton::JAUNE)
                     << "ligne" << ligne << "colonne" << colonne;
            return ligne;
        }
    }
    return -1;
}

/**
 * @fn Puissance4::verifierPlateau
 * @brief méthode pour vérifier si il y a 4 pions alignés de la même
 * couleur sur le plateau
 */
void Puissance4::verifierPlateau()
{
    vainqueur = false;
    if(!verifierLigne())
        if(!verifierColonne())
            if(!verifierDiagonaleMontante())
                verifierDiagonaleDescendante();
}

/**
 * @fn Puissance4::estVainqueur
 * @brief méthode qui retourne true si il y a 4 pions alignés de la même
 * couleur sur le plateau
 * @return bool
 */
bool Puissance4::estVainqueur() const
{
    return vainqueur;
}

/**
 * @fn Puissance4::estEquipeRouge
 * @brief méthode qui retourne true si c'est à l'équipe rouge de jouer
 * @return bool
 */
bool Puissance4::estEquipeRouge() const
{
    return equipeRouge;
}

/**
 * @fn Puissance4::setTourEquipe
 * @brief fixe l'équipe qui doit jouer
 */
void Puissance4::setTourEquipe(bool estEquipeRouge)
{
    this->equipeRouge = estEquipeRouge;
}

/**
 * @fn Puissance4::verifierLigne
 * @brief méthode pour vérifier si il y a une ligne de 4 pions de la même
 * couleur
 */
bool Puissance4::verifierLigne()
{
    for(int ligne = 0; ligne < NB_LIGNES; ligne++)
    {
        for(int colonne = 0; colonne < NB_COLONNES - NB_PIONS_ALIGNES;
            colonne++)
        {
            if(plateau[colonne][ligne] == CouleurJeton::JAUNE &&
               plateau[colonne + 1][ligne] == CouleurJeton::JAUNE &&
               plateau[colonne + 2][ligne] == CouleurJeton::JAUNE &&
               plateau[colonne + 3][ligne] == CouleurJeton::JAUNE)
            {
                vainqueur = true;
                qDebug() << Q_FUNC_INFO << "estVainqueur" << vainqueur
                         << "JAUNE";
                return true;
            }
            else if(plateau[colonne][ligne] == CouleurJeton::ROUGE &&
                    plateau[colonne + 1][ligne] == CouleurJeton::ROUGE &&
                    plateau[colonne + 2][ligne] == CouleurJeton::ROUGE &&
                    plateau[colonne + 3][ligne] == CouleurJeton::ROUGE)
            {
                vainqueur = true;
                qDebug() << Q_FUNC_INFO << "estVainqueur" << vainqueur
                         << "ROUGE";
                return true;
            }
        }
    }
    return false;
}

/**
 * @fn Puissance4::verifierColonne
 * @brief éthode pour vérifier si il y a une colonne de 4 pions de la même
 * couleur
 */
bool Puissance4::verifierColonne()
{
    for(int colonne = 0; colonne < NB_COLONNES; ++colonne)
    {
        for(int ligne = 0; ligne < NB_LIGNES - 3; ++ligne)
        {
            if(plateau[colonne][ligne] == CouleurJeton::JAUNE &&
               plateau[colonne][ligne + 1] == CouleurJeton::JAUNE &&
               plateau[colonne][ligne + 2] == CouleurJeton::JAUNE &&
               plateau[colonne][ligne + 3] == CouleurJeton::JAUNE)
            {
                vainqueur = true;
                qDebug() << Q_FUNC_INFO << "estVainqueur" << vainqueur
                         << "JAUNE";
                return true;
            }
            else if(plateau[colonne][ligne] == CouleurJeton::ROUGE &&
                    plateau[colonne][ligne + 1] == CouleurJeton::ROUGE &&
                    plateau[colonne][ligne + 2] == CouleurJeton::ROUGE &&
                    plateau[colonne][ligne + 3] == CouleurJeton::ROUGE)
            {
                vainqueur = true;
                qDebug() << Q_FUNC_INFO << "estVainqueur" << vainqueur
                         << "ROUGE";
                return true;
            }
        }
    }
    return false;
}

/**
 * @fn Puissance4::verifierDiagonaleDescendante
 * @brief éthode pour vérifier si il y a une diagonale descendante de 4 pions de
 * la même couleur
 */
bool Puissance4::verifierDiagonaleDescendante()
{
    for(int ligne = 3; ligne < NB_LIGNES; ligne++)
    {
        for(int colonne = 0; colonne < NB_COLONNES - NB_PIONS_ALIGNES;
            colonne++)
        {
            if(plateau[ligne][colonne] == CouleurJeton::JAUNE &&
               plateau[ligne - 1][colonne + 1] == CouleurJeton::JAUNE &&
               plateau[ligne - 2][colonne + 2] == CouleurJeton::JAUNE &&
               plateau[ligne - 3][colonne + 3] == CouleurJeton::JAUNE)
            {
                vainqueur = true;
                qDebug() << Q_FUNC_INFO << "estVainqueur" << vainqueur
                         << "JAUNE";
                return true;
            }
            else if(plateau[ligne][colonne] == CouleurJeton::ROUGE &&
                    plateau[ligne - 1][colonne + 1] == CouleurJeton::ROUGE &&
                    plateau[ligne - 2][colonne + 2] == CouleurJeton::ROUGE &&
                    plateau[ligne - 3][colonne + 3] == CouleurJeton::ROUGE)
            {
                vainqueur = true;
                qDebug() << Q_FUNC_INFO << "estVainqueur" << vainqueur
                         << "ROUGE";
                return true;
            }
        }
    }
    return false;
}

/**
 * @fn Puissance4::verifierDiagonaleMontante
 * @brief éthode pour vérifier si il y a une diagonale montante de 4 pions de la
 * même couleur
 */
bool Puissance4::verifierDiagonaleMontante()
{
    for(int ligne = 0; ligne <= NB_LIGNES - NB_PIONS_ALIGNES; ++ligne)
    {
        for(int colonne = 0; colonne < NB_COLONNES - NB_PIONS_ALIGNES;
            ++colonne)
        {
            if(plateau[ligne][colonne] == CouleurJeton::JAUNE &&
               plateau[ligne + 1][colonne + 1] == CouleurJeton::JAUNE &&
               plateau[ligne + 2][colonne + 2] == CouleurJeton::JAUNE &&
               plateau[ligne + 3][colonne + 3] == CouleurJeton::JAUNE)

            {
                vainqueur = true;
                qDebug() << Q_FUNC_INFO << "estVainqueur" << vainqueur
                         << "JAUNE";
                return true;
            }
            else if(plateau[ligne][colonne] == CouleurJeton::ROUGE &&
                    plateau[ligne + 1][colonne + 1] == CouleurJeton::ROUGE &&
                    plateau[ligne + 2][colonne + 2] == CouleurJeton::ROUGE &&
                    plateau[ligne + 3][colonne + 3] == CouleurJeton::ROUGE)
            {
                vainqueur = true;
                qDebug() << Q_FUNC_INFO << "estVainqueur" << vainqueur
                         << "ROUGE";
                return true;
            }
        }
    }
    return false;
}

#ifdef TEST_ALIGNEMENT
void Puissance4::testUnitaireVerifierPlateau()
{
    initialiserPlateau();
    int ligne                       = 0;
    int colonne                     = 0;
    plateau[ligne + 5][colonne + 2] = CouleurJeton::ROUGE;
    plateau[ligne + 4][colonne + 3] = CouleurJeton::ROUGE;
    plateau[ligne + 3][colonne + 1] = CouleurJeton::ROUGE;
    plateau[ligne + 2][colonne + 5] = CouleurJeton::ROUGE;
    verifierPlateau();
    qDebug() << Q_FUNC_INFO << "estVainqueur" << vainqueur;
}
#endif
