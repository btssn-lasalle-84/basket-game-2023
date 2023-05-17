#include "plateau.h"
#include "basketgame.h"

/**
 * @fn Plateau::Plateau
 * @brief Constructeur de la classe Plateau
 */
Plateau::Plateau(QObject* parent) :
    QObject(parent), plateau(NB_COLONNES), vainqueur(false), equipeRouge(true)
{
}

/**
 * @fn Plateau::initialiserPlateau
 * @brief méthode pour initialiser le plateau de puissance4
 */
void Plateau::initialiserPlateau()
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
 * @fn Plateau::placerPion
 * @brief méthode qui lace un pion dans une des clonnes du puissance4
 * @param colonne
 * @return ligne ou -1
 */
int Plateau::placerPion(int colonne)
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
 * @fn Plateau::verifierPlateau
 * @brief méthode pour vérifier si il y a 4 pions alignés de la même
 * couleur sur le plateau
 */
void Plateau::verifierPlateau()
{
    vainqueur = false;
    if(!verifierLigne())
        if(!verifierColonne())
            if(!verifierDiagonaleMontante())
                verifierDiagonaleDescendante();
}

/**
 * @fn Plateau::estVainqueur
 * @brief méthode qui retourne true si il y a 4 pions alignés de la même
 * couleur sur le plateau
 */
bool Plateau::estVainqueur() const
{
    return vainqueur;
}

bool Plateau::estEquipeRouge() const
{
    return equipeRouge;
}

/**
 * @fn Plateau::setTourEquipe
 * @brief fixe l'équipe qui doit jouer
 */
void Plateau::setTourEquipe(bool estEquipeRouge)
{
    this->equipeRouge = estEquipeRouge;
}

/**
 * @fn Plateau::verifierLigne
 * @brief méthode pour vérifier si il y a une ligne de 4 pions de la même
 * couleur
 */
bool Plateau::verifierLigne()
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
 * @fn Plateau::verifierColonne
 * @brief éthode pour vérifier si il y a une colonne de 4 pions de la même
 * couleur
 */
bool Plateau::verifierColonne()
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
 * @fn Plateau::verifierDiagonaleDescendante
 * @brief éthode pour vérifier si il y a une diagonale descendante de 4 pions de
 * la même couleur
 */
bool Plateau::verifierDiagonaleDescendante()
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
 * @fn Plateau::verifierDiagonaleMontante
 * @brief éthode pour vérifier si il y a une diagonale montante de 4 pions de la
 * même couleur
 */
bool Plateau::verifierDiagonaleMontante()
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
void Plateau::testUnitaireVerifierPlateau()
{
    initialiserPlateau();
    int ligne                       = 0;
    int colonne                     = 0;
    plateau[ligne + 5][colonne + 2] = CouleurJeton::ROUGE;
    plateau[ligne + 4][colonne + 3] = CouleurJeton::ROUGE;
    plateau[ligne + 3][colonne + 4] = CouleurJeton::ROUGE;
    plateau[ligne + 2][colonne + 5] = CouleurJeton::ROUGE;
    verifierPlateau();
    qDebug() << Q_FUNC_INFO << "estVainqueur" << vainqueur;
}
#endif
