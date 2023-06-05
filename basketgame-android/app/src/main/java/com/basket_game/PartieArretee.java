/**
 * @file PartieArretee.java
 * @brief Déclaration de la classe PartieArretee
 * @author Guillaumet Florent
 */

package com.basket_game;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.graphics.Color;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.ProgressBar;
import android.widget.TextView;

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
    private Intent                 intentDonneesPartieArretee;
    private Intent intentDonneesPartieSuivi;
    private String vainqueur;
    private int equipeGagnante;
    private String nomEquipe1;
    private String nomEquipe2;
    /**
     * Ressources GUI
     */
    TextView affichageVainqueur;

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
        recupererVainqueur();
        afficherVainqueur();
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

    /**
     * @brief Méthode appelée pour récupérer le vainqueur de la partie
     */
    private void recupererVainqueur()
    {
        intentDonneesPartieArretee = getIntent();
        equipeGagnante = intentDonneesPartieArretee.getIntExtra("equipeGagnante", -1);
        Log.d(TAG, "recupererVainqueur() equipeGagnante = " + equipeGagnante);
        nomEquipe1 = intentDonneesPartieArretee.getStringExtra("nomEquipe1");
        nomEquipe2 = intentDonneesPartieArretee.getStringExtra("nomEquipe2");
    }

    /**
     * @brief Méthode appelée pour afficher le vainqueur de la partie
     */
    private void afficherVainqueur()
    {
        affichageVainqueur = findViewById(R.id.affichageVainqueur);
        if (equipeGagnante == 1) {
            vainqueur = "Bravo " + nomEquipe1 + " !";
            affichageVainqueur.setTextColor(Color.parseColor("#D00000"));
        } else if (equipeGagnante == 2) {
            vainqueur = "Bravo " + nomEquipe2 + " !";
            affichageVainqueur.setTextColor(Color.parseColor("#FFBE0B"));
        } else if (equipeGagnante == 0){
            vainqueur = "Egalité !";
        }
        else
        {
            vainqueur = "";
        }
        affichageVainqueur.setText(vainqueur);
        Log.d(TAG, "afficherVainqueur() Vainqueur = " + vainqueur);
    }
}