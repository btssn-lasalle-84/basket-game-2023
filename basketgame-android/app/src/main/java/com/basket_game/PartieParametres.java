/**
 * @file PartieParametres.java
 * @brief Déclaration de la classe PartieParametres
 * @author Guillaumet Florent
 */

package com.basket_game;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;

import java.io.Serializable;

/**
 * @class PartieParametres
 * @brief L'activité qui permet de paramétrer une partie
 */
public class PartieParametres extends AppCompatActivity
{
    /**
     * Constantes
     */
    private static final String TAG = "_ParametresPartie"; //!< TAG pour les logs (cf. Logcat)

    /**
     * Attributs
     */
    private Intent intentDonneesPartieSuivi;

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
        setContentView(R.layout.partie_parametres);

        intentDonneesPartieSuivi = new Intent(PartieParametres.this, PartieSuivi.class);

        afficherSuiviPartie();
    }

    /**
     * @brief Méthode appelée pour récupérer le nom de l'équipe 1
     */
    private void recupererNomEquipe(int idEquipe, int numeroEquipe)
    {
        EditText editionEquipe = findViewById(idEquipe);
        String nomEquipe = editionEquipe.getText().toString();
        Equipe equipe = new Equipe(nomEquipe);
        intentDonneesPartieSuivi.putExtra("equipe" + numeroEquipe, (Serializable) equipe);
    }

    /**
     * @brief Méthode appelée pour afficher le suivi de la partie
     */
    private void afficherSuiviPartie()
    {
        Button boutonLancerPartie = findViewById(R.id.boutonLancerPartie);
        boutonLancerPartie.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v)
            {
                recupererNomEquipe(R.id.editionEquipe1, 1);
                recupererNomEquipe(R.id.editionEquipe2, 2);
                Log.d(TAG, "afficherSuiviPartie() " + ((EditText)(findViewById(R.id.editionEquipe1))).getText().toString() + " vs " + ((EditText)(findViewById(R.id.editionEquipe2))).getText().toString() );
                startActivity(intentDonneesPartieSuivi);
            }
        });
    }
}