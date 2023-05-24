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

class Equipe : public QObject
{
    Q_OBJECT

  private:
    QString nom;
    int     score;

  public:
    Equipe(const QString& nom, QObject* parent = 0);
    ~Equipe();

    QString getNom() const;
    void    setNom(const QString& nom);
    int     getScore() const;
    void    setScore(int score);
    void    incrementerScore();
};

#endif // EQUIPE_H
