/**
 * @file equipe.cpp
 *
 * @brief Définition de la classe Equipe
 * @author Nathanael CHANSARD
 * @version 1.0
 */

#include "equipe.h"
#include "puissance4.h"
#include <QDebug>

/**
 * @brief Constructeur de la classe Equipe
 *
 * @fn Equipe::Equipe
 *
 */
Equipe::Equipe(const QString& nom, QObject* parent) :
    QObject(parent), nom(nom), scorePanier(0) , scoreManche(0)
{
    qDebug() << Q_FUNC_INFO << "nom" << nom;
}

/**
 * @brief Destructeur de la classe Equipe
 *
 * @fn Equipe::~Equipe
 *
 */
Equipe::~Equipe()
{
    qDebug() << Q_FUNC_INFO;
}


/**
 * @fn Equipe::getNom()
 * @brief Retourne le nom de l'équipe
 */
QString Equipe::getNom() const
{
    return nom;
}

/**
 * @fn Equipe::setNom()
 * @brief Modifie le nom de l'équipe
 */
void Equipe::setNom(const QString& nom)
{
    this->nom = nom;
    qDebug() << Q_FUNC_INFO << "nom" << nom;
}

/**
 * @fn Equipe::getScore()
 * @brief Retourne le score de l'équipe
 */
int Equipe::getScorePanier() const
{
    return scorePanier;
}

/**
 * @fn Equipe::setScore()
 * @brief  Modifie le score de l'équipe
 */
void Equipe::setScorePanier(int scorePanier)
{
    this->scorePanier = scorePanier;
    qDebug() << Q_FUNC_INFO << "score" << scorePanier;
}
/**
 * @fn Equipe::getScoreManche()
 * @brief Retourne le score de l'équipe
 */
int Equipe::getScoreManche() const
{
    return scoreManche;
}

void Equipe::setScoreManche(int scoreManche)
{
    this->scoreManche = scoreManche;
    qDebug() << Q_FUNC_INFO << "scoreManche" << scoreManche;

}

/**
 * @fn Equipe::incrementerScore()
 * @brief méthode pour incrementer le score de chaque équipe
 */
void Equipe::incrementerScorePanier()
{
    scorePanier++;
    qDebug() << Q_FUNC_INFO << "score" << scorePanier;
}

void Equipe::incrementerScoreManche()
{
    scoreManche++;
}
