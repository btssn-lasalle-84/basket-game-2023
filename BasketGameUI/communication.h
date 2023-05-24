#ifndef _COMMUNICATION_H
#define _COMMUNICATION_H

/**
 * @file communication.h
 *
 * @brief Déclaration de la classe Communication
 * @author Nathanael CHANSARD
 * @version 1.0
 */
#include <QObject>
#include <QtBluetooth>


static const QString serviceUuid(QStringLiteral("00001101-0000-1000-8000-00805F9B34FB"));
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

    QString getNomPeripheriqueLocal();
    QString getAdressePeripheriqueLocal();

  public slots:

  private:
    QBluetoothLocalDevice peripheriqueLocal;
    QBluetoothServer* serveur; //!< Le serveur Bluetooth
    QBluetoothSocket* socket;  //!< La socket de communication Bluetooth
    QBluetoothServiceInfo serviceInfo; //!< Information sur le service Bluetooth
    bool estConnecter;

    QString nomPeripheriqueLocal;
    QString adressePeripheriqueLocal;

  private slots:
    void connecterTablette(const QBluetoothAddress &adresse);
    void deconnecterTablette(const QBluetoothAddress &adresse);
    void connecterSocket();
    void deconnecterSocket();
    void recupererDonneesSocket();

    void recupererErreurSocket(QBluetoothSocket::SocketError erreurBluetooth);
    void recupererErreurBluetooth(QBluetoothLocalDevice::Error erreurBluetooth);
    void recupererEtatSocket(QBluetoothSocket::SocketState etatBluetooth);


  signals:
    void clientConnecter();
    void clientDeconnecter();
    void afficherMessage(QString message);
};

#endif

