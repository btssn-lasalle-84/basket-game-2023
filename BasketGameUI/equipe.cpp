/**
 * @file equipe.cpp
 *
 * @brief Définition de la classe Equipe
 * @author Nathanael CHANSARD
 * @version 0.2
 */


#include "equipe.h"
#include <QDebug>

Equipe::Equipe(QWidget *parent) : QWidget(parent) ,
nomEquipeRouge("Bob"),nomEquipeJaune("Alice")
{

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


QString Equipe::getNomEquipeJaune() const
{
    return nomEquipeJaune;
}

QString Equipe::getNomEquipeRouge() const
{
    return nomEquipeRouge;
}

void Equipe::setNomEquipeJaune(const QString nomEquipeJaune)
{
    this->nomEquipeJaune = nomEquipeJaune;
    qDebug() << Q_FUNC_INFO << nomEquipeJaune;
}

void Equipe::setNomEquipeRouge(const QString nomEquipeRouge)
{
    this->nomEquipeRouge = nomEquipeRouge;
    qDebug() << Q_FUNC_INFO << nomEquipeRouge;
}

