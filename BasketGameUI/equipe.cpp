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
    QObject(parent), nom(nom), score(0) ,
    puissance4(new Puissance4(this))
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

QString Equipe::getNom() const
{
    return nom;
}

void Equipe::setNom(const QString& nom)
{
    this->nom = nom;
    qDebug() << Q_FUNC_INFO << "nom" << nom;
}

int Equipe::getScore() const
{
    return score;
}

void Equipe::setScore(int score)
{
    this->score = score;
    qDebug() << Q_FUNC_INFO << "score" << score;
}

void Equipe::incrementerScore()
{
   if(puissance4->estEquipeRouge())
   {
       score++;
   }
   else
   {
       score++;
   }
   qDebug() << Q_FUNC_INFO << "score" << score;

}
