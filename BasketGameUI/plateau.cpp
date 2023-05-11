#include "plateau.h"
#include "basketgame.h"

Plateau::Plateau(QWidget *parent) : QWidget(parent),
    plateau(NB_COLONNES),estVainqueur(false),estEquipeRouge(true)
{

}

/**
 * @fn Plateau::initialiserPlateau()
 * @brief méthode pour initialiser le plateau de puissance4
 */
void Plateau::initialiserPlateau()
{
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
void Plateau::verifierPlateau()
{
    verifierLigne();
    verifierColonne();
    verifierDiagonaleMontante();
    verifierDiagonaleDescendante();
}

/**
 * @fn Plateau::verifierLigne
 * @brief méthode pour vérifier si il y a une ligne de 4 pions de la même couleur
 */
void Plateau::verifierLigne()
{

    for (int ligne = 0; ligne < NB_LIGNES; ligne++)
        {
            for(int colonne = 0; colonne < NB_COLONNES - NB_PIONS_ALIGNES; colonne++)
            {
                    if(plateau[colonne][ligne] == CouleurJeton::JAUNE &&
                       plateau[colonne+1][ligne] == CouleurJeton::JAUNE &&
                       plateau[colonne+2][ligne] == CouleurJeton::JAUNE &&
                       plateau[colonne+3][ligne] == CouleurJeton::JAUNE)
                    {
                        estVainqueur = true;
                        qDebug() << Q_FUNC_INFO << "estVainqueurb" << estVainqueur << "JAUNE";
                        return;
                    }
                    else if(plateau[colonne][ligne] == CouleurJeton::ROUGE &&
                            plateau[colonne+1][ligne] == CouleurJeton::ROUGE &&
                            plateau[colonne+2][ligne] == CouleurJeton::ROUGE &&
                            plateau[colonne+3][ligne] == CouleurJeton::ROUGE)
                    {
                        estVainqueur = true;
                        qDebug() << Q_FUNC_INFO << "estVainqueur" << estVainqueur << "ROUGE";
                        return;
                    }
                }
        }
}

/**
 * @fn Plateau::verifierColonne
 * @brief éthode pour vérifier si il y a une colonne de 4 pions de la même couleur
 */
void Plateau::verifierColonne()
{
    estVainqueur = false;

    for (int colonne = 0; colonne < NB_COLONNES; ++colonne)
        {
            for(int ligne = 0; ligne < NB_LIGNES - 3; ++ligne)
            {
                       if(plateau[colonne][ligne] == CouleurJeton::JAUNE &&
                       plateau[colonne][ligne+1] == CouleurJeton::JAUNE &&
                       plateau[colonne][ligne+2] == CouleurJeton::JAUNE &&
                       plateau[colonne][ligne+3] == CouleurJeton::JAUNE)
                    {
                        estVainqueur = true;
                        qDebug() << Q_FUNC_INFO << "estVainqueur" << estVainqueur << "JAUNE";
                        return;
                    }
                    else if(plateau[colonne][ligne] == CouleurJeton::ROUGE &&
                            plateau[colonne][ligne+1] == CouleurJeton::ROUGE &&
                            plateau[colonne][ligne+2] == CouleurJeton::ROUGE &&
                            plateau[colonne][ligne+3] == CouleurJeton::ROUGE)
                    {
                        estVainqueur = true;
                        qDebug() << Q_FUNC_INFO << "estVainqueur" << estVainqueur << "ROUGE";
                        return;
                    }
                }
        }
}

/**
 * @fn Plateau::verifierDiagonaleDescendante
 * @brief éthode pour vérifier si il y a une diagonale descendante de 4 pions de la même couleur
 */
void Plateau::verifierDiagonaleDescendante()
{

    for(int ligne = 3; ligne < NB_LIGNES; ligne++) {
        for(int colonne = 0; colonne < NB_COLONNES - NB_PIONS_ALIGNES; colonne++) {
            if(plateau[ligne][colonne] == CouleurJeton::JAUNE &&
               plateau[ligne-1][colonne+1] == CouleurJeton::JAUNE &&
               plateau[ligne-2][colonne+2] == CouleurJeton::JAUNE &&
               plateau[ligne-3][colonne+3] == CouleurJeton::JAUNE)
            {
               estVainqueur = true;
               qDebug() << Q_FUNC_INFO << "estVainqueur" << estVainqueur << "ROUGE";
               return;
            }
            else if(plateau[ligne][colonne] == CouleurJeton::ROUGE &&
               plateau[ligne-1][colonne+1] == CouleurJeton::ROUGE &&
               plateau[ligne-2][colonne+2] == CouleurJeton::ROUGE &&
               plateau[ligne-3][colonne+3] == CouleurJeton::ROUGE)
            {
               estVainqueur = true;
               qDebug() << Q_FUNC_INFO << "estVainqueur" << estVainqueur << "ROUGE";
               return;
            }
        }
    }
}

/**
 * @fn Plateau::verifierDiagonaleMontante
 * @brief éthode pour vérifier si il y a une diagonale montante de 4 pions de la même couleur
 */
void Plateau::verifierDiagonaleMontante()
{
    estVainqueur = false;

    for(int ligne = 0; ligne <= NB_LIGNES - NB_PIONS_ALIGNES; ++ligne)
    {
        for(int colonne = 0 ; colonne < NB_COLONNES - NB_PIONS_ALIGNES ; ++colonne)
        {
            if(plateau[ligne][colonne]     == CouleurJeton::JAUNE &&
               plateau[ligne+1][colonne+1] == CouleurJeton::JAUNE &&
               plateau[ligne+2][colonne+2] == CouleurJeton::JAUNE &&
               plateau[ligne+3][colonne+3] == CouleurJeton::JAUNE)

            {
               estVainqueur =  true;
               qDebug() << Q_FUNC_INFO << "estVainqueur" << estVainqueur << "JAUNE";
               return;
            }
            else if (plateau[ligne][colonne]    == CouleurJeton::ROUGE &&
               plateau[ligne+1][colonne+1]      == CouleurJeton::ROUGE &&
               plateau[ligne+2][colonne+2]      == CouleurJeton::ROUGE &&
               plateau[ligne+3][colonne+3]      == CouleurJeton::ROUGE)
            {
               estVainqueur =  true;
               qDebug() << Q_FUNC_INFO << "estVainqueur" << estVainqueur << "ROUGE";
               return;
            }

        }
    }
}

/**
 * @fn Plateau::simulerPion()
 * @brief méthode pour simuler un coup de puissance4
 */
void Plateau::simulerPion()
{
    int colonne = randInt(0, NB_COLONNES - 1);
    int ligne   = 0;

    for(ligne = 0; ligne < NB_LIGNES; ++ligne)
    {
        if(colonne >= 0 && colonne < NB_COLONNES &&
           plateau[colonne][ligne] == CouleurJeton::AUCUNE)
        {
            plateau[colonne][ligne] =
              (estEquipeRouge ? CouleurJeton::ROUGE : CouleurJeton::JAUNE);
            qDebug() << Q_FUNC_INFO << "pion" << (estEquipeRouge ? CouleurJeton::ROUGE : CouleurJeton::JAUNE) << "ligne" << ligne << "colonne" << colonne;
            break;
        }
    }
}
/**
 * @fn Plateau::randInt(int min, int max)
 * @brief méthode pour créer une variable aléatoire
 */
int Plateau::randInt(int min, int max)
{
    return qrand() % ((max + 1) - min) + min;
}

#ifdef TEST_BASKETGAME
void Plateau::testUnitaire()
{
    int ligne = 0;
    int colonne = 0;
    initialiserPlateau();
    plateau[ligne+5][colonne+2] = CouleurJeton::ROUGE;
    plateau[ligne+4][colonne+3] = CouleurJeton::ROUGE;
    plateau[ligne+3][colonne+4] = CouleurJeton::ROUGE;
    plateau[ligne+2][colonne+5] = CouleurJeton::ROUGE;
    verifierPlateau();
}
#endif
