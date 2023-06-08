#ifndef EQUIPE_H
#define EQUIPE_H

/**
 * @file equipe.h
 *
 * @brief Déclaration de la classe Equipe
 * @author Nathanael CHANSARD
 * @version 1.0
 */
#include <QObject>
#include <QString>

class Puissance4;

class Equipe : public QObject
{
    Q_OBJECT

  private:
    QString nom;
    int     scorePanier;
    int     scoreManche;

  public:
    Equipe(const QString& nom, QObject* parent = 0);
    ~Equipe();

    QString getNom() const;
    void    setNom(const QString& nom);
    int     getScorePanier() const;
    void    setScorePanier(int scorePanier);
    int     getScoreManche() const;
    void    setScoreManche(int scoreManche);
    void    incrementerScorePanier();
    void    incrementerScoreManche();

};

#endif // EQUIPE_H
