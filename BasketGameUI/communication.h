#ifndef _COMMUNICATION_H
#define _COMMUNICATION_H

/**
 * @file communication.h
 *
 * @brief Déclaration de la classe Communication
 * @author Nathanael CHANSARD
 * @version 1.0
 */

#define ENTETE_DEBUT     "$BASKET"
#define ENTETE_FIN       "\r\n"
#define DELIMITEUR_CHAMP ";"
#define TYPE_TRAME       1
// Trame SEANCE :
// $BASKET;SEANCE;NOM_EQUIPE1;NOM_EQUIPE2;TEMPS;PANIERS;MANCHES;\r\n
#define NOM_EQUIPE1    2
#define NOM_EQUIPE2    3
#define TEMPS_TOUR_MAX 4
#define NB_PANIERS     5
#define NB_MANCHES     6
// Trame START : $BASKET;START;NUMERO_PARTIE;\r\n
#define NUMERO_PARTIE 2
// Trame TIR : $BASKET;TIR;COULEUR;NUMERO_PANIER;\r\n
#define COULEUR_EQUIPE 2
#define NUMERO_PANIER  3

#include <QObject>
#include <QtBluetooth>
#include <QString>

static const QString serviceUuid(
  QStringLiteral("00001101-0000-1000-8000-00805F9B34FB"));
static const QString serviceNom(QStringLiteral("BasketGame"));

class Basketgame;

class Communication : public QObject
{
    Q_OBJECT
  public:
    enum TypeTrame
    {
        Inconnu = -1,
        Seance,
        Start,
        Tir,
        Stop,
        Reset
    };
    Communication(QObject* parent = 0);
    ~Communication();

    void initialiser();
    void demarrer();
    void arreter();
    void deconnecter();
    bool estValide();
    bool estConnecte();

    QString getNomPeripheriqueLocal();
    QString getAdressePeripheriqueLocal();

  public slots:

  private:
    QBluetoothLocalDevice peripheriqueLocal;
    QBluetoothServer*     serveur;     //!< Le serveur Bluetooth
    QBluetoothSocket*     socket;      //!< La socket de communication Bluetooth
    QBluetoothServiceInfo serviceInfo; //!< Information sur le service Bluetooth
    bool                  connecte;    //!< Etat de connexion de la socket client
    QString               nomPeripheriqueLocal;
    QString               adressePeripheriqueLocal;
    QString trame; //!< Le contenu des données reçues sur la socket

    Communication::TypeTrame recupererTypeTrame(QString champType);
    void                     traiterTrame(const QStringList& champsTrame);

  private slots:
    void connecterTablette(const QBluetoothAddress& adresse);
    void deconnecterTablette(const QBluetoothAddress& adresse);
    void connecterSocket();
    void deconnecterSocket();
    void recevoirDonnees();
    void recupererErreurSocket(QBluetoothSocket::SocketError erreurBluetooth);
    void recupererErreurBluetooth(QBluetoothLocalDevice::Error erreurBluetooth);
    void recupererEtatSocket(QBluetoothSocket::SocketState etatBluetooth);

  signals:
    void clientConnecte();
    void clientReconnecte();
    void clientDeconnecte();
    void tabletteConnectee();
    void tabletteDeconnectee();
    void partieConfiguree(QString nomEquipeRouge,
                          QString nomEquipeJaune,
                          int     nbPaniers,
                          int     tempsTour,
                          int     nbManches);
    void partieDemarree(int numeroManche);
    void partieArretee(int numeroManche);
    void partieReinitialisee();
    void tirPanier(QString couleurEquipe, int numeroPanier);
};

#endif
