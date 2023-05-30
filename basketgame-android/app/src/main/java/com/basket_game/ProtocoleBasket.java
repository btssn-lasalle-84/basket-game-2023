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
public class ProtocoleBasket {

    /**
     * Constantes
     */
    public final static String DELIMITEUR_DEBUT_TRAME         = "$BASKET"; // délimiteur début trame
    public final static String DELIMITEUR_FIN_TRAME         = "\r\n"; // délimiteur fin trame
    public final static String DELIMITEUR_CHAMPS_TRAME         = ";"; // délimiteur champs trame

    /**
     * Attributs
     */
    public enum Type {
        SEANCE, START, TIR, STOP, RESET
    }
    public enum CouleurEquipe {
        ROUGE, JAUNE
    }
}
