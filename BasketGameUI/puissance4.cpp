#include "puissance4.h"
#include "basketgame.h"

/**
 * @fn Puissance4::Puissance4
 * @brief Constructeur de la classe Puissance4
 */
Puissance4::Puissance4(QObject* parent) :
    QObject(parent), puissance4(NB_COLONNES), vainqueur(false), equipeRouge(true)
{
}

/**
 * @fn Puissance4::initialiserPuissance4
 * @brief méthode pour initialiser le puissance4 de puissance4
 */
void Puissance4::initialiserPuissance4()
{
    vainqueur = false;
    for(int i = 0; i < puissance4.size(); ++i)
    {
        puissance4[i].resize(NB_LIGNES);
    }

    qDebug() << Q_FUNC_INFO << "colonnes" << puissance4.size() << "lignes"
             << puissance4[0].size();

    for(int colonne = 0; colonne < puissance4.size(); ++colonne)
    {
        for(int ligne = 0; ligne < puissance4[colonne].size(); ++ligne)
        {
            puissance4[colonne][ligne] = CouleurJeton::AUCUNE;
        }
    }
}

/**
 * @fn Puissance4::placerPion
 * @brief méthode qui lace un pion dans une des clonnes du puissance4
 * @param colonne
 * @return ligne ou -1
 */
int Puissance4::placerPion(int colonne)
{
    int ligne = 0;
    for(ligne = 0; ligne < NB_LIGNES; ++ligne)
    {
        if(colonne >= 0 && colonne < NB_COLONNES &&
           puissance4[colonne][ligne] == CouleurJeton::AUCUNE)
        {
            puissance4[colonne][ligne] =
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
 * @fn Puissance4::verifierPuissance4
 * @brief méthode pour vérifier si il y a 4 pions alignés de la même
 * couleur sur le puissance4
 */
void Puissance4::verifierPuissance4()
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
 * couleur sur le puissance4
 */
bool Puissance4::estVainqueur() const
{
    return vainqueur;
}

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
            if(puissance4[colonne][ligne] == CouleurJeton::JAUNE &&
               puissance4[colonne + 1][ligne] == CouleurJeton::JAUNE &&
               puissance4[colonne + 2][ligne] == CouleurJeton::JAUNE &&
               puissance4[colonne + 3][ligne] == CouleurJeton::JAUNE)
            {
                vainqueur = true;
                qDebug() << Q_FUNC_INFO << "estVainqueur" << vainqueur
                         << "JAUNE";
                return true;
            }
            else if(puissance4[colonne][ligne] == CouleurJeton::ROUGE &&
                    puissance4[colonne + 1][ligne] == CouleurJeton::ROUGE &&
                    puissance4[colonne + 2][ligne] == CouleurJeton::ROUGE &&
                    puissance4[colonne + 3][ligne] == CouleurJeton::ROUGE)
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
            if(puissance4[colonne][ligne] == CouleurJeton::JAUNE &&
               puissance4[colonne][ligne + 1] == CouleurJeton::JAUNE &&
               puissance4[colonne][ligne + 2] == CouleurJeton::JAUNE &&
               puissance4[colonne][ligne + 3] == CouleurJeton::JAUNE)
            {
                vainqueur = true;
                qDebug() << Q_FUNC_INFO << "estVainqueur" << vainqueur
                         << "JAUNE";
                return true;
            }
            else if(puissance4[colonne][ligne] == CouleurJeton::ROUGE &&
                    puissance4[colonne][ligne + 1] == CouleurJeton::ROUGE &&
                    puissance4[colonne][ligne + 2] == CouleurJeton::ROUGE &&
                    puissance4[colonne][ligne + 3] == CouleurJeton::ROUGE)
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
            if(puissance4[ligne][colonne] == CouleurJeton::JAUNE &&
               puissance4[ligne - 1][colonne + 1] == CouleurJeton::JAUNE &&
               puissance4[ligne - 2][colonne + 2] == CouleurJeton::JAUNE &&
               puissance4[ligne - 3][colonne + 3] == CouleurJeton::JAUNE)
            {
                vainqueur = true;
                qDebug() << Q_FUNC_INFO << "estVainqueur" << vainqueur
                         << "JAUNE";
                return true;
            }
            else if(puissance4[ligne][colonne] == CouleurJeton::ROUGE &&
                    puissance4[ligne - 1][colonne + 1] == CouleurJeton::ROUGE &&
                    puissance4[ligne - 2][colonne + 2] == CouleurJeton::ROUGE &&
                    puissance4[ligne - 3][colonne + 3] == CouleurJeton::ROUGE)
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
 * @fn Platsudo apt-get install boumleau::verifierDiagonaleMontante
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
            if(puissance4[ligne][colonne] == CouleurJeton::JAUNE &&
               puissance4[ligne + 1][colonne + 1] == CouleurJeton::JAUNE &&
               puissance4[ligne + 2][colonne + 2] == CouleurJeton::JAUNE &&
                puissance4[ligne + 3][colonne + 3] == CouleurJeton::JAUNE)

            {
                vainqueur = true;
                qDebug() << Q_FUNC_INFO << "estVainqueur" << vainqueur
                         << "JAUNE";
                return true;
            }
            else if(puissance4[ligne][colonne] == CouleurJeton::ROUGE &&
                    puissance4[ligne + 1][colonne + 1] == CouleurJeton::ROUGE &&
                    puissance4[ligne + 2][colonne + 2] == CouleurJeton::ROUGE &&
                    puissance4[ligne + 3][colonne + 3] == CouleurJeton::ROUGE)
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
void Puissance4::testUnitaireVerifierPuissance4()
{
    initialiserPuissance4();
    int ligne                       = 0;
    int colonne                     = 0;
    puissance4[ligne + 5][colonne + 2] = CouleurJeton::ROUGE;
    puissance4[ligne + 4][colonne + 3] = CouleurJeton::ROUGE;
    puissance4[ligne + 3][colonne + 1] = CouleurJeton::ROUGE;
    puissance4[ligne + 2][colonne + 5] = CouleurJeton::ROUGE;
    verifierPuissance4();
    qDebug() << Q_FUNC_INFO << "estVainqueur" << vainqueur;
}
#endif
