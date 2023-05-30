/**
 * @file Equipe.java
 * @brief Déclaration de la classe Equipe
 * @author Guillaumet Florent
 */

package com.basket_game;

import android.util.Log;

import java.io.Serializable;

/**
 * @class Equipe
 * @brief Définit une équipe
 */
public class Equipe implements Serializable
{
    /**
     * Constantes
     */
    private static final String TAG = "_Equipe"; //!< TAG pour les logs (cf. Logcat)

    /**
     * @brief Attributs
     */
    private String nomEquipe; //!< le nom de l'équipe
    public int     score;

    /**
     * @brief Constructeur de la classe Equipe
     */
    public Equipe()
    {
        Log.d(TAG, "Equipe()");
        this.nomEquipe = "";
        this.score     = 0;
    }

    /**
     * @brief Constructeur de la classe Equipe
     */
    public Equipe(String nomEquipe)
    {
        Log.d(TAG, "Equipe(" + nomEquipe + ")");
        this.nomEquipe = nomEquipe;
    }

    /**
     *
     * @brief Accesseur Equipe
     * @fn getNomEquipe
     * @details Retourne le nom
     * @return String
     */
    public String getNomEquipe()
    {
        return nomEquipe;
    }

    /**
     *
     * @brief Mutateur Equipe
     * @fn setNomEquipe
     * @details Modifie le nom de l'Equipe
     * @param nomEquipe
     */
    public void setNomEquipe(String nomEquipe)
    {
        this.nomEquipe = nomEquipe;
    }

    /**
     *
     * @brief Accesseur Equipe
     * @fn getNomEquipe
     * @details Retourne le nom
     * @return String
     */
    public int getScore()
    {
        return score;
    }

    public void reinitialiserScore()
    {
        this.score = 0;
    }

    public void incrementerScore()
    {
        this.score++;
    }
}