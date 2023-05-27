/**
 * @file BasketGame.java
 * @brief Déclaration de la classe BasketGame
 * @author Guillaumet Florent
 */

package com.basket_game;

import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.View;
import android.widget.Button;

import androidx.appcompat.app.AppCompatActivity;

/**
 * @class BasketGame
 * @brief L'activité principale
 */
public class BasketGame extends AppCompatActivity
{
    /**
     * Constantes
     */
    private static final String TAG = "_BasketGame"; //!< TAG pour les logs (cf. Logcat)

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
        setContentView(R.layout.basketgame);
        Log.d(TAG, "onCreate()");

        afficherNouvellePartie();
    }

    /**
     * @brief Méthode appelée au démarrage après le onCreate() ou un restart
     * après un onStop()
     */
    @Override
    protected void onStart()
    {
        super.onStart();
        Log.d(TAG, "onStart()");
    }

    /**
     * @brief Méthode appelée après onStart() ou après onPause()
     */
    @Override
    protected void onResume()
    {
        super.onResume();
        Log.d(TAG, "onResume()");
    }

    /**
     * @brief Méthode appelée après qu'une boîte de dialogue s'est affichée (on
     * reprend sur un onResume()) ou avant onStop() (activité plus visible)
     */
    @Override
    protected void onPause()
    {
        super.onPause();
        Log.d(TAG, "onPause()");
    }

    /**
     * @brief Méthode appelée lorsque l'activité n'est plus visible
     */
    @Override
    protected void onStop()
    {
        super.onStop();
        Log.d(TAG, "onStop()");
    }

    /**
     * @brief Méthode appelée à la destruction de l'application (après onStop()
     * et détruite par le système Android)
     */
    @Override
    protected void onDestroy()
    {
        super.onDestroy();
        Log.d(TAG, "onDestroy()");
    }

    /**
     * @brief Méthode appelée pour afficher la nouvelle partie
     */
    private void afficherNouvellePartie()
    {
        Button boutonNouvellePartie = findViewById(R.id.boutonNouvellePartie);
        boutonNouvellePartie.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v)
            {
                Log.d(TAG, "afficherNouvellePartie()");
                Intent intent = new Intent(BasketGame.this, PartieParametres.class);
                startActivity(intent);
            }
        });
    }
}