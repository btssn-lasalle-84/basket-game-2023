/**
 * @file src/main.cpp
 * @brief Programme principal B4SKET-GAME 2023
 * @author Thierry Vaira
 * @version 0.12
 */
#include <Arduino.h>
#include <BluetoothSerial.h>
#include <afficheur.h>
#include "esp_bt_main.h"
#include "esp_bt_device.h"

#define DEBUG

#define SANS_ACK
#define SANS_BOUTON_JOUEUR_SUIVANT

// Brochages
#define GPIO_LED_ROUGE   5    //!<
#define GPIO_LED_ORANGE  17   //!< Trame OK
#define GPIO_LED_VERTE   16   //!< Trame START
#define GPIO_SW1         12   //!< Pour simuler un tir
#define GPIO_SW2         14   //!< Pour simuler joueur suivant
#define ADRESSE_I2C_OLED 0x3c //!< Adresse I2C de l'OLED
#define BROCHE_I2C_SDA   21   //!< Broche SDA
#define BROCHE_I2C_SCL   22   //!< Broche SCL

// Protocole (cf. Google Drive)
#define EN_TETE_TRAME    "$BASKET"
#define DELIMITEUR_CHAMP ";"
#define DELIMITEURS_FIN  "\r\n"
#define DELIMITEUR_DATAS ';'
#define DELIMITEUR_FIN   '\n'

#define NB_PANIERS    7
#define NB_MANCHES    10
#define MAX_TEMPS_TIR 15 // 45

#define BLUETOOTH
#ifdef BLUETOOTH
#define BLUETOOTH_SLAVE //!< esclave
// #define BLUETOOTH_MASTER //!< maître
BluetoothSerial ESPBluetooth;
#endif

/**
 * @enum TypeTrame
 * @brief Les differents types de trame
 */
enum TypeTrame
{
    Inconnu = -1,
    SEANCE  = 0,
    START,
    TIR,
    STOP,
    RESET,
    ACK,
    NB_TRAMES
};

/**
 * @enum EtatPartie
 * @brief Les differents états d'une partie
 */
enum EtatPartie
{
    Finie = 0,
    EnCours,
    EnPause,
    Terminee
};

/**
 * @enum CouleurEquipe
 * @brief Les deux couleurs d'équipe
 */
enum CouleurEquipe
{
    ROUGE = 0,
    JAUNE = 1,
    NbEquipes
};

/**
 * @enum ChampsSeance
 * @brief Les différents champos du trame SEANCE
 */
enum ChampSeance
{
    Entete = 0, // $BASKET
    Type   = 1, // SEANCE
    NOM_EQUIPE1,
    NOM_EQUIPE2,
    TEMPS,
    PANIERS,
    MANCHES,
    NbChamps
};

const String nomsTrame[TypeTrame::NB_TRAMES] = {
    "SEANCE",
    "START",
    "TIR",
    "STOP",
    "RESET"
};                                       //!< nom des trames dans le protocole
EtatPartie    etatPartie   = Finie;      //!< l'état de la partie
int           numeroTir    = 0;          //!<
int           numeroPanier = 0;          //!< de 0 à NB_PANIERS
int           nbPaniers    = NB_PANIERS; //!< le nombre de paniers détectables
int           numeroPartie = 0;          //!<
int           nbManches    = 0;
unsigned long tempsMaxTir  = MAX_TEMPS_TIR * 1000;
unsigned long tempsPrecedent = 0;
CouleurEquipe joueurCourant =
  CouleurEquipe::ROUGE; //!< la couleur de l'équipe qui tire
const String codeEquipe[CouleurEquipe::NbEquipes] = {
    "ROUGE",
    "JAUNE"
}; //!< code de l'équipe qui tire
String equipe1 = codeEquipe[CouleurEquipe::ROUGE];
String equipe2 = codeEquipe[CouleurEquipe::JAUNE];
// String typePartie;
bool      refresh    = false; //!< demande rafraichissement de l'écran OLED
bool      antiRebond = false; //!< anti-rebond
Afficheur afficheur(ADRESSE_I2C_OLED,
                    BROCHE_I2C_SDA,
                    BROCHE_I2C_SCL);                //!< afficheur OLED SSD1306
String    entete        = String(EN_TETE_TRAME);    // caractère séparateur
String    separateur    = String(DELIMITEUR_CHAMP); // caractère séparateur
String    delimiteurFin = String(DELIMITEURS_FIN);  // fin de trame

void changerJoueur()
{
    // joueur suivant
    joueurCourant  = (CouleurEquipe)((int(joueurCourant) + 1) % NbEquipes);
    tempsPrecedent = millis();
    afficheur.setMessageLigne(Afficheur::Ligne4, codeEquipe[joueurCourant]);
    refresh = true;
#ifdef DEBUG
    if(joueurCourant == CouleurEquipe::ROUGE)
    {
        Serial.println("Suivant : ROUGE");
    }
    else
    {
        Serial.println("Suivant : JAUNE");
    }
#endif
}

String extraireChamp(String& trame, unsigned int numeroChamp)
{
    String       champ;
    unsigned int compteurCaractere  = 0;
    unsigned int compteurDelimiteur = 0;
    char         fin                = '\n';

    if(delimiteurFin.length() > 0)
        fin = delimiteurFin[0];

    for(unsigned int i = 0; i < trame.length(); i++)
    {
        if(trame[i] == separateur[0] || trame[i] == fin)
        {
            compteurDelimiteur++;
        }
        else if(compteurDelimiteur == numeroChamp)
        {
            champ += trame[i];
            compteurCaractere++;
        }
    }

    return champ;
}

/**
 * @brief Envoie une trame d'acquittement via le Bluetooth
 *
 */
void envoyerTrameAcquittement(bool panier = false)
{
#ifndef SANS_ACK
    char trameEnvoi[64];

    if(panier) // Format : $basket;ACK;{NB_PANIERS};\r
    {
        sprintf((char*)trameEnvoi,
                "%s;%s;%d;\r",
                entete.c_str(),
                nomsTrame[TypeTrame::ACK].c_str(),
                nbPaniers);
    }
    else // Format : $basket;ACK;\r
    {
        sprintf((char*)trameEnvoi,
                "%s;%s;\r",
                entete.c_str(),
                nomsTrame[TypeTrame::ACK].c_str());
    }
    ESPBluetooth.write((uint8_t*)trameEnvoi, strlen((char*)trameEnvoi));
#ifdef DEBUG
    String trame = String(trameEnvoi);
    trame.remove(trame.indexOf("\r"), 1);
    Serial.print("> ");
    Serial.println(trame);
#endif
#endif
}

/**
 * @brief Envoie une trame via le Bluetooth
 *
 */
void envoyerTrameDetection(int numeroPanier, CouleurEquipe couleurEquipe)
{
    if(numeroPanier == 0)
        return;
    char trameEnvoi[64];

    // Format : $BASKET;TIR;{EQUIPE};{NUMERO};\r\n
    sprintf((char*)trameEnvoi,
            "%s;TIR;%s;%d;\r\n",
            entete.c_str(),
            codeEquipe[(int)couleurEquipe].c_str(),
            numeroPanier);
    ESPBluetooth.write((uint8_t*)trameEnvoi, strlen((char*)trameEnvoi));
#ifdef DEBUG
    String trame = String(trameEnvoi);
    trame.remove(trame.indexOf("\r\n"), 2);
    Serial.print("> ");
    Serial.println(trame);
#endif
}

/**
 * @brief Déclenchée par interruption sur le bouton SW1
 * @fn tirerPanier()
 */
void IRAM_ATTR tirerPanier()
{
    if(etatPartie != EnCours || antiRebond)
        return;

    int tir = random(0, nbPaniers + 1);

    envoyerTrameDetection(tir, joueurCourant);
    numeroTir++;

    char strMessageDisplay[24];
    if(joueurCourant == CouleurEquipe::ROUGE)
    {
        if(tir == 0)
            sprintf(strMessageDisplay, "%s : loupé", equipe1.c_str());
        else
            sprintf(strMessageDisplay, "%s : panier %d", equipe1.c_str(), tir);
        afficheur.setMessageLigne(Afficheur::Ligne1, String(strMessageDisplay));
    }
    else
    {
        if(tir == 0)
            sprintf(strMessageDisplay, "%s : loupé", equipe2.c_str());
        else
            sprintf(strMessageDisplay, "%s : panier %d", equipe2.c_str(), tir);
        afficheur.setMessageLigne(Afficheur::Ligne2, String(strMessageDisplay));
    }

    refresh    = true;
    antiRebond = true;

#ifdef DEBUG
    if(joueurCourant == CouleurEquipe::ROUGE)
    {
        Serial.print("Equipe ROUGE -> ");
    }
    else
    {
        Serial.print("Equipe JAUNE -> ");
    }
    if(tir == 0)
    {
        Serial.println("loupé !");
    }
    else
    {
        Serial.print("panier ");
        Serial.println(tir);
    }
#endif

    if(tir == 0)
        return;

    changerJoueur();
}

/**
 * @brief Déclenchée par interruption sur le bouton SW2
 * @fn terminerTir()
 */
void IRAM_ATTR terminerTir()
{
#ifndef SANS_BOUTON_JOUEUR_SUIVANT
    if(etatPartie != EnCours || antiRebond)
        return;
    changerJoueur();
    antiRebond = true;
#endif
}

/**
 * @brief Lit une trame via le Bluetooth
 *
 * @fn lireTrame(String &trame)
 * @param trame la trame reçue
 * @return bool true si une trame a été lue, sinon false
 */
bool lireTrame(String& trame)
{
    if(ESPBluetooth.available())
    {
#ifdef DEBUG
        Serial.print("Disponible : ");
        Serial.println(ESPBluetooth.available());
#endif
        trame.clear();
        trame = ESPBluetooth.readStringUntil(DELIMITEUR_FIN);
#ifdef DEBUG
        Serial.print("< ");
        Serial.println(trame);
#endif
        trame.concat(DELIMITEUR_FIN); // remet le DELIMITEUR_FIN
        return true;
    }

    return false;
}

/**
 * @brief Vérifie si la trame reçue est valide et retorune le type de la trame
 *
 * @fn verifierTrame(String &trame)
 * @param trame
 * @return TypeTrame le type de la trame
 */
TypeTrame verifierTrame(String& trame)
{
    String format;

    for(int i = 0; i < TypeTrame::NB_TRAMES; i++)
    {
        // Format : $BASKET;{TYPE};\r
        format =
          entete + separateur + nomsTrame[i] + separateur; // + delimiteurFin;
#ifdef DEBUG
        Serial.print("Verification trame : ");
        Serial.print(format);
#endif
        // if(trame == format)
        if(trame.indexOf(format) != -1)
        {
            return (TypeTrame)i;
        }
        else
        {
#ifdef DEBUG
            Serial.println("");
#endif
        }
    }
#ifdef DEBUG
    Serial.println("Type de trame : inconnu");
#endif
    return Inconnu;
}

void reinitialiserAffichage()
{
    afficheur.setMessageLigne(Afficheur::Ligne1, "");
    afficheur.setMessageLigne(Afficheur::Ligne2, "");
    afficheur.setMessageLigne(Afficheur::Ligne3, "");
    afficheur.setMessageLigne(Afficheur::Ligne4, "");
    refresh = true;
}

/**
 * @brief Retourne true si l'échéance de la durée fixée a été atteinte
 * @param intervalle unsigned long
 * @return bool true si la durée est arrivée à échéance
 */
bool estEcheance(unsigned long intervalle)
{
    if(etatPartie != EnCours)
        return false;
    unsigned long temps = millis();
    if(temps - tempsPrecedent >= intervalle)
    {
        tempsPrecedent = temps;
        return true;
    }
    return false;
}

/**
 * @brief Initialise les ressources du programme
 *
 * @fn setup
 *
 */
void setup()
{
    Serial.begin(115200);
    while(!Serial)
        ;

    pinMode(GPIO_LED_ROUGE, OUTPUT);
    pinMode(GPIO_LED_ORANGE, OUTPUT);
    pinMode(GPIO_LED_VERTE, OUTPUT);
    pinMode(GPIO_SW1, INPUT_PULLUP);
    pinMode(GPIO_SW2, INPUT_PULLUP);

    attachInterrupt(digitalPinToInterrupt(GPIO_SW1), tirerPanier, FALLING);
    attachInterrupt(digitalPinToInterrupt(GPIO_SW2), terminerTir, FALLING);

    digitalWrite(GPIO_LED_ROUGE, HIGH);
    digitalWrite(GPIO_LED_ORANGE, LOW);
    digitalWrite(GPIO_LED_VERTE, LOW);

    afficheur.initialiser();

    String titre  = "";
    String stitre = "=====================";

#ifdef BLUETOOTH
#ifdef BLUETOOTH_MASTER
    String nomBluetooth = "iot-esp-maitre";
    ESPBluetooth.begin(nomBluetooth, true);
    const uint8_t* adresseESP32 = esp_bt_dev_get_address();
    char           str[18];
    sprintf(str,
            "%02x:%02x:%02x:%02x:%02x:%02x",
            adresseESP32[0],
            adresseESP32[1],
            adresseESP32[2],
            adresseESP32[3],
            adresseESP32[4],
            adresseESP32[5]);
    stitre = String("== ") + String(str) + String(" ==");
    titre  = nomBluetooth;
#else
    String nomBluetooth = "basket-detection";
    ESPBluetooth.begin(nomBluetooth);
    const uint8_t* adresseESP32 = esp_bt_dev_get_address();
    char           str[18];
    sprintf(str,
            "%02x:%02x:%02x:%02x:%02x:%02x",
            adresseESP32[0],
            adresseESP32[1],
            adresseESP32[2],
            adresseESP32[3],
            adresseESP32[4],
            adresseESP32[5]);
    stitre = String("== ") + String(str) + String(" ==");
    titre  = nomBluetooth;
#endif
#endif

#ifdef DEBUG
    Serial.println(titre);
    Serial.println(stitre);
#endif

    afficheur.setTitre(titre);
    afficheur.setSTitre(stitre);
    afficheur.afficher();

    // initialise le générateur pseudo-aléatoire
    // Serial.println(randomSeed(analogRead(34)));
    Serial.println(esp_random());
}

/**
 * @brief Boucle infinie d'exécution du programme
 *
 * @fn loop
 *
 */
void loop()
{
    String    trame;
    TypeTrame typeTrame;

    if(refresh)
    {
        afficheur.afficher();
        refresh = false;
    }

    if(antiRebond)
    {
        afficheur.afficher();
        delay(300);
        antiRebond = false;
    }

    if(estEcheance(tempsMaxTir))
    {
        changerJoueur();
    }

    if(lireTrame(trame))
    {
        typeTrame = verifierTrame(trame);
        refresh   = true;
#ifdef DEBUG
        if(typeTrame >= 0)
            Serial.println("\nTrame : " + nomsTrame[typeTrame]);
#endif
        switch(typeTrame)
        {
            case Inconnu:
                break;
            case TypeTrame::SEANCE:
                //$BASKET;SEANCE;NOM_EQUIPE1;NOM_EQUIPE2;TEMPS;PANIERS;MANCHES;\r\n
                if(etatPartie == Finie)
                {
                    equipe1 = extraireChamp(trame, ChampSeance::NOM_EQUIPE1);
                    equipe2 = extraireChamp(trame, ChampSeance::NOM_EQUIPE2);
#ifdef DEBUG
                    Serial.print("Equipe n°1 : ");
                    Serial.println(equipe1);
                    Serial.print("Equipe n°2 : ");
                    Serial.println(equipe2);
#endif
                    String nb;
                    nb = extraireChamp(trame, ChampSeance::TEMPS);
                    if(nb.toInt() > 0 && nb.toInt() <= MAX_TEMPS_TIR)
                        tempsMaxTir = nb.toInt() * 1000;
                    else
                        tempsMaxTir = MAX_TEMPS_TIR * 1000;
#ifdef DEBUG
                    Serial.print("Temps max tir : ");
                    Serial.println(tempsMaxTir);
#endif
                    nb = extraireChamp(trame, ChampSeance::PANIERS);
                    if(nb.toInt() > 0 && nb.toInt() <= NB_PANIERS)
                        nbPaniers = nb.toInt();
#ifdef DEBUG
                    Serial.print("Nombre de paniers : ");
                    Serial.println(nbPaniers);
#endif
                    nb = extraireChamp(trame, ChampSeance::MANCHES);
                    if(nb.toInt() > 0 && nb.toInt() <= NB_MANCHES)
                        nbManches = nb.toInt();
                    else
                        nbManches = NB_MANCHES;
#ifdef DEBUG
                    Serial.print("Nombre de manches : ");
                    Serial.println(nbManches);
#endif
                    /**
                     * @todo Gérer les paramètres d'une séance
                     *
                     */
#ifdef DEBUG
                    Serial.println("Nouvelle séance");
#endif
                    reinitialiserAffichage();
                    envoyerTrameAcquittement(true);
                    afficheur.setMessageLigne(Afficheur::Ligne3,
                                              String("Seance"));
                    afficheur.afficher();
                }
                break;
            case TypeTrame::START:
                //$BASKET;START;NUMERO_PARTIE;\r\n
                if(etatPartie == Finie)
                {
                    String nb    = extraireChamp(trame, 2);
                    numeroPartie = nb.toInt();
#ifdef DEBUG
                    Serial.print("Numéro de partie : ");
                    Serial.println(numeroPartie);
#endif
                    reinitialiserAffichage();
                    envoyerTrameAcquittement(true);
                    tempsPrecedent = millis();
                    joueurCourant  = CouleurEquipe::ROUGE;
                    numeroTir      = 0;
                    etatPartie     = EnCours;
                    digitalWrite(GPIO_LED_ROUGE, LOW);
                    digitalWrite(GPIO_LED_ORANGE, LOW);
                    digitalWrite(GPIO_LED_VERTE, HIGH);
                    afficheur.setMessageLigne(Afficheur::Ligne3,
                                              String("En cours"));
                    afficheur.setMessageLigne(Afficheur::Ligne4,
                                              codeEquipe[joueurCourant]);
                    afficheur.afficher();
#ifdef DEBUG
                    Serial.println("Nouvelle partie");
#endif
                }
                break;
            /*case TypeTrame::PAUSE:
                if(etatPartie == EnCours)
                {
                    envoyerTrameAcquittement();
                    etatPartie = EnPause;
                    digitalWrite(GPIO_LED_ROUGE, LOW);
                    digitalWrite(GPIO_LED_ORANGE, HIGH);
                    digitalWrite(GPIO_LED_VERTE, LOW);
                    afficheur.setMessageLigne(Afficheur::Ligne3,
                                              String("En pause"));
                    afficheur.afficher();
                }
                break;*/
            /*case TypeTrame::PLAY:
                if(etatPartie == EnPause)
                {
                    envoyerTrameAcquittement();
                    etatPartie = EnCours;
                    digitalWrite(GPIO_LED_ROUGE, LOW);
                    digitalWrite(GPIO_LED_ORANGE, LOW);
                    digitalWrite(GPIO_LED_VERTE, HIGH);
                    afficheur.setMessageLigne(Afficheur::Ligne3,
                                              String("En cours"));
                    afficheur.afficher();
                }
                break;*/
            case TypeTrame::STOP:
                if(etatPartie > Finie)
                {
                    reinitialiserAffichage();
                    envoyerTrameAcquittement();
                    etatPartie = Finie;
                    digitalWrite(GPIO_LED_ROUGE, HIGH);
                    digitalWrite(GPIO_LED_ORANGE, LOW);
                    digitalWrite(GPIO_LED_VERTE, LOW);
                    afficheur.setMessageLigne(Afficheur::Ligne3,
                                              String("Finie"));
                    afficheur.afficher();
                }
                break;
            case TypeTrame::RESET:
                reinitialiserAffichage();
                envoyerTrameAcquittement();
                etatPartie = Finie;
                digitalWrite(GPIO_LED_ROUGE, HIGH);
                digitalWrite(GPIO_LED_ORANGE, LOW);
                digitalWrite(GPIO_LED_VERTE, LOW);
                afficheur.setMessageLigne(Afficheur::Ligne3, String("Finie"));
                reinitialiserAffichage();
                break;
            case TypeTrame::ACK:
                digitalWrite(GPIO_LED_ORANGE, HIGH);
                delay(150);
                digitalWrite(GPIO_LED_ORANGE, LOW);
                break;
            default:
#ifdef DEBUG
                Serial.println("Trame invalide !");
#endif
                break;
        }
    }
}
