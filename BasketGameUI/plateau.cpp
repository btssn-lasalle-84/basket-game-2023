
#include "plateau.h"

Plateau::Plateau(QWidget * parent) : QWidget(parent)
{

}

void Plateau::creerPlateau() {
    // un QVector de QVector
    for(int i = 0; i < plateau.size(); ++i)
    {
        plateau[i].resize(NB_LIGNES);
    }
}

//@fn Plateau::initialiserPlateau
//@brief méthode pour initialiser le plateau de puissance4
//

void Plateau::initialiserPlateau() {
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

void Plateau::afficherPlateau() {
    QString ressource = QString(":/home/nath/Downloads/puissance4_7.png");
    ui->UiEquipe->setPixmap(QPixmap(ressource));
    afficherMessage("");
    initialiserPlateau();
}

void Plateau::attribuerRaccourcisClavier() {
    // les touches pour "jouer"
    QAction* quitter = new QAction(this);
    quitter->setShortcut(QKeySequence(
      QKeySequence(Qt::Key_Up))); // fleche du haut pour quitter l'application
    addAction(quitter);
    connect(quitter,
            SIGNAL(triggered()),
            this,
            SLOT(fermerApplication())); // Pour fermer l'application
    QAction* actionPion1 = new QAction(this);
    actionPion1->setShortcut(QKeySequence(Qt::Key_Right));
    addAction(actionPion1);
    connect(actionPion1, SIGNAL(triggered()), this, SLOT(simulerPion1()));
    QAction* actionPion2 = new QAction(this);
    actionPion2->setShortcut(QKeySequence(Qt::Key_Left));
    addAction(actionPion2);
    connect(actionPion2, SIGNAL(triggered()), this, SLOT(simulerPion2()));
    QAction* actionReset = new QAction(this);
    actionReset->setShortcut(QKeySequence(Qt::Key_Down));
    addAction(actionReset);
    connect(actionReset, SIGNAL(triggered()), this, SLOT(afficherPlateau()));
    QAction* actionAnnuler = new QAction(this);
    actionAnnuler->setShortcut(QKeySequence(Qt::Key_A));
    addAction(actionAnnuler);
    connect(actionAnnuler, SIGNAL(triggered()), this, SLOT(annulerPion()));
}

