/**
 * @file ParametresPartieActivity.java
 * @brief Déclaration de la classe ParametresPartieActivity
 * @author Guillaumet Florent
 */

package com.basket_game;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;

/**
 * @class ParametresPartieActivity
 * @brief L'activité qui permet de paramétrer une partie
 */
public class ParametresPartie extends AppCompatActivity
{
    /**
     * Constantes
     */
    private static final String TAG = "_ParametresPartie"; //!< TAG pour les logs (cf. Logcat)

    /**
     * Attributs
     */

    /**
     * Ressources GUI
     */

    /**
     * @brief Méthode appelée à la création de l'activité
     */
    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.parametres_partie);
    }
}