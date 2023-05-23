#ifndef EQUIPE_H
#define EQUIPE_H

#include <QWidget>
#include <QString>

class Equipe : public QWidget
{
    Q_OBJECT

private :

public:
    explicit Equipe(QWidget *parent = 0);
    ~Equipe();

    QString getNomEquipeJaune() const;
    QString getNomEquipeRouge() const;
    QString         nomEquipeJaune;
    QString         nomEquipeRouge;

    void    setNomEquipeJaune(QString nomEquipeJaune);
    void    setNomEquipeRouge(QString nomEquipeRouge);
};

#endif // EQUIPE_H
