/**
 * @file PartieArretee.java
 * @brief Déclaration de la classe PartieArretee
 * @author Guillaumet Florent
 */

package com.basket_game;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;

/**
 * @class PartieArretee
 * @brief L'activité qui indique l'arrêt de la partie
 */
public class PartieArretee extends AppCompatActivity
{
    /**
     * Constantes
     */
    private static final String TAG = "_PartieArretee"; //!< TAG pour les logs (cf. Logcat)

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
        setContentView(R.layout.partie_arretee);
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
                Intent intent = new Intent(PartieArretee.this, BasketGame.class);
                startActivity(intent);
            }
        });
    }
}