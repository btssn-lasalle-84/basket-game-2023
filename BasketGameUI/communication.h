#ifndef _COMMUNICATION_H
#define _COMMUNICATION_H

/**
 * @file communication.h
 *
 * @brief Déclaration de la classe Communication
 * @author Nathanael CHANSARD
 * @version 1.0
 */

#define ENTETE_DEBUT        "$BASKET"
#define ENTETE_FIN          "\r\n"
#define DELIMITEUR_CHAMP    ";"
#define TYPE_TRAME          1

#define NOM_CLUB_ROUGE      3
#define NOM_CLUB_JAUNE      4
#define NOMBRE_PANIER       5
#define TEMPS_TOUR_TRAME    6

#define NUMERO_PANIER       7
#define COULEUR_EQUIPE      8

#include <QObject>
#include <QtBluetooth>
#include <QString>

static const QString serviceUuid(
  QStringLiteral("00001101-0000-1000-8000-00805F9B34FB"));
static const QString serviceNom(QStringLiteral("BasketGame"));

class Communication : public QObject
{
    Q_OBJECT
  public:
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
    enum TypeTrame
    {
        Seance,
        Start,
        Tir,
        Stop,
        Reset
    };
    QBluetoothLocalDevice peripheriqueLocal;
    QBluetoothServer*     serveur;     //!< Le serveur Bluetooth
    QBluetoothSocket*     socket;      //!< La socket de communication Bluetooth
    QBluetoothServiceInfo serviceInfo; //!< Information sur le service Bluetooth
    bool                  connecte; //!< Etat de connexion de la socket client
    QString               nomPeripheriqueLocal;
    QString               adressePeripheriqueLocal;
    QString               trame;        //!< Le contenu des données reçues sur la socket
    QStringList           champsTrame;

  private slots:
    void connecterTablette(const QBluetoothAddress& adresse);
    void deconnecterTablette(const QBluetoothAddress& adresse);
    void connecterSocket();
    void deconnecterSocket();
    void recevoirDonnees(QStringList champsTrame);
    void traiterTrame(QStringList champsTrame);

    void recupererErreurSocket(QBluetoothSocket::SocketError erreurBluetooth);
    void recupererErreurBluetooth(QBluetoothLocalDevice::Error erreurBluetooth);
    void recupererEtatSocket(QBluetoothSocket::SocketState etatBluetooth);

  signals:
    void clientConnecte();
    void clientDeconnecte();
    void tabletteConnectee();
    void tabletteDeconnectee();
    void trameRecue(QString message);
    void configurerPartie(int nomEquipeRouge , int nomEquipeJaune , int nombrePanier , int tempsTour);
    void demanderStart();
    void demanderStop();
    void demanderReset();
    void marquerPanier(int couleurEquipe , int numeroPanier);

};

#endif
