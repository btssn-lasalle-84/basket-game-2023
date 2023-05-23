#ifndef COMMUNICATION_H
#define COMMUNICATION_H

/**
 * @file communication.h
 *
 * @brief Déclaration de la classe Communication
 * @author Nathanael CHANSARD
 * @version 0.2
 */
#include <QWidget>
#include <QString>
#include <QtBluetooth>

/**
 * @def TYPE_TRAME
 * @brief Définition de l'entête de trame
 */
#define ENTETE_TRAME "$BASKET"

/**
 * @def DELIMITEUR_FIN
 * @brief Délimiteur de fin de trame
 */
#define DELIMITEUR_FIN "\r\n"

/**
 * @def DELIMITEUR_CHAMP
 * @brief Délimiteur de champs
 */
#define DELIMITEUR_CHAMP ";"

static const QString serviceUuid(
  QStringLiteral("00001101-0000-1000-8000-00805F9B34FB"));
static const QString serviceNom(QStringLiteral("ModuleEcran"));

/**
 * @class Communication
 * @brief La classe de Communication Bluetooth
 */
class Communication : public QWidget
{
    Q_OBJECT
  public:
    explicit Communication(QWidget* parent = 0);
    ~Communication();

  public slots:

  private:
    QBluetoothSocket*     socket;  //!< La socket de communication Bluetooth
    QBluetoothServer*     serveur; //!< Le serveur Bluetooth
    QBluetoothLocalDevice peripheriqueLocal; //!< L'interface Bluetooth
    QBluetoothServiceInfo
      serviceBluetooth; //!< Informations sur le service bluetooth

  signals:
};

#endif // COMMUNICATION_H
