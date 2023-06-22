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
    public int     scorePanier;
    public int     scoreManche;

    /**
     * @brief Constructeur de la classe Equipe
     */
    public Equipe()
    {
        Log.d(TAG, "Equipe()");
        this.nomEquipe = "";
        this.scorePanier     = 0;
        this.scoreManche     = 0;
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
     * @brief Accesseur Equipe
     * @fn getScorePanier
     * @details Retourne le nombre de panier inscrits
     * @return int
     */
    public int getScorePanier()
    {
        return scorePanier;
    }

    /**
     * @brief Méthode appelée pour réinitialiser le nombre de panier des équipes
     */
    public void reinitialiserScorePanier()
    {
        this.scorePanier = 0;
    }

    /**
     * @brief Méthode appelée pour incrémenter de 1 le scorePanier
     */
    public void incrementerScorePanier()
    {
        this.scorePanier++;
    }

    /**
     * @brief Accesseur Equipe
     * @fn getScoreManche
     * @details Retourne le nombre de manche gagnée par l'équipe
     * @return int
     */
    public int getScoreManche()
    {
        return scoreManche;
    }

    /**
     * @brief Méthode appelée pour réinitialiser le nombre de manche gagnée
     */
    public void reinitialiserScoreManche()
    {
        this.scoreManche = 0;
    }

    /**
     * @brief Méthode appelée pour incrémenter de 1 le nombre de manche gagnée
     */
    public void incrementerScoreManche()
    {
        this.scoreManche++;
    }
}