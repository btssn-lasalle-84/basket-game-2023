/**
 * @file ProtocoleBasket.java
 * @brief Déclaration de la classe ProtocoleBasket
 * @author Guillaumet Florent
 */

package com.basket_game;

/**
 * @class Communication
 * @brief Définit la classe du protocole de basket game
 */
public class ProtocoleBasket
{
    /**
     * Constantes
     */
    public final static String DELIMITEUR_DEBUT_TRAME  = "$BASKET"; // délimiteur début trame
    public final static String DELIMITEUR_FIN_TRAME    = "\r\n";    // délimiteur fin trame
    public final static String DELIMITEUR_CHAMPS_TRAME = ";";       // délimiteur champs trame

    public final static int    CHAMP_TYPE_TRAME  = 1;
    public final static String TYPE_TRAME_SEANCE = "SEANCE";
    public final static String TYPE_TRAME_START  = "START";
    public final static String TYPE_TRAME_TIR    = "TIR";
    // Trame TIR : $BASKET;TIR;COULEUR;NUMERO_PANIER;\r\n
    public final static String TYPE_TRAME_STOP   = "STOP";
    public final static String TYPE_TRAME_RESET  = "RESET";
    public final static String TYPE_TRAME_PAUSE  = "PAUSE";
    public final static String TYPE_TRAME_FIN  = "FIN";
    public final static int CHAMP_COULEUR_EQUIPE = 2;
    public final static int CHAMP_NUMERO_PANIER  = 3;
    public final static int NB_CHAMPS_TRAME_TIR  = 4;
    public final static int NB_CHAMPS_TRAME_FIN  = 3;
    public final static String COULEUR_EQUIPE_ROUGE = "ROUGE";
    public final static String COULEUR_EQUIPE_JAUNE = "JAUNE";

    public enum Type
    {
        SEANCE,
        START,
        TIR,
        STOP,
        RESET
    }
    public enum CouleurEquipe
    {
        ROUGE,
        JAUNE
    }
}
