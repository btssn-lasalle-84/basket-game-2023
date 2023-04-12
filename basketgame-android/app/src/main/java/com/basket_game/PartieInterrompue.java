/**
 * @file PartieInterrompue.java
 * @brief Déclaration de la classe PartieInterrompue
 * @author Guillaumet Florent
 */

package com.basket_game;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;

/**
 * @class PartieInterrompue
 * @brief L'activité qui indique l'interruption de la partie
 */
public class PartieInterrompue extends AppCompatActivity {

    /**
     * @brief Méthode appelée à la création de l'activité
     */
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.partie_interrompue);
    }
}