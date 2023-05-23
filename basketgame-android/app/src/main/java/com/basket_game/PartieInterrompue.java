/**
 * @file PartieInterrompue.java
 * @brief Déclaration de la classe PartieInterrompue
 * @author Guillaumet Florent
 */

package com.basket_game;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.content.res.ColorStateList;
import android.graphics.Color;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;

/**
 * @class PartieInterrompue
 * @brief L'activité qui indique l'interruption de la partie
 */
public class PartieInterrompue extends AppCompatActivity
{
    /**
     * Constantes
     */
    private static final String TAG = "_PartieInterrompue"; //!< TAG pour les logs (cf. Logcat)

    /**
     * Attributs
     */

    /**
     * @brief Méthode appelée à la création de l'activité
     */
    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.partie_interrompue);
        Log.d(TAG, "onCreate()");

        creerBoutonRecommencerPartie();
        creerBoutonAfficherMenuPrincipal();
    }

    /**
     * @brief Méthode appelée pour recommencer une partie avec les mêmes paramètres
     */
    private void creerBoutonRecommencerPartie()
    {
        Button boutonRecommencerPartie = findViewById(R.id.boutonRecommencerPartie);
        boutonRecommencerPartie.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v)
            {
                Log.d(TAG, "recommencerPartie()");
                finish();
            }
        });
    }

    /**
     * @brief Méthode appelée pour retourner au menu principal
     */
    private void creerBoutonAfficherMenuPrincipal()
    {
        Button boutonMenuPrincipal = findViewById(R.id.boutonMenuPrincipal);
        boutonMenuPrincipal.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v)
            {
                Log.d(TAG, "afficherMenuPrincipal()");
                Intent intent = new Intent(PartieInterrompue.this, BasketGame.class);
                startActivity(intent);
            }
        });
    }
}