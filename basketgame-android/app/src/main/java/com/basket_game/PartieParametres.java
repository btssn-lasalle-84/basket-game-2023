/**
 * @file PartieParametres.java
 * @brief Déclaration de la classe PartieParametres
 * @author Guillaumet Florent
 */

package com.basket_game;

import static com.basket_game.Partie.NB_MANCHES_GAGNANTES;
import static com.basket_game.Partie.NB_PANIERS;
import static com.basket_game.Partie.TEMPS_MAX_TOUR;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.text.Editable;
import android.text.TextWatcher;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Spinner;

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
    public static final int NUMERO_ID_PANIERS_DEFAUT = 2; //!< le nombre max de paniers gérables

    /**
     * Attributs
     */
    private Intent intentDonneesPartieSuivi;

    /**
     * @brief Méthode appelée à la création de l'activité
     */
    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.partie_parametres);
        Log.d(TAG, "onCreate()");

        Spinner choixNbPaniers = findViewById(R.id.choixNbPaniers);
        choixNbPaniers.setSelection(NUMERO_ID_PANIERS_DEFAUT); //!< Par défaut 4 paniers

        intentDonneesPartieSuivi = new Intent(PartieParametres.this, PartieSuivi.class);
        editerTempsTour();
        afficherSuiviPartie();
    }

    /**
     * @brief Méthode appelée pour récupérer le nom d'une équipe
     */
    private void recupererNomEquipe(int idEquipe, int numeroEquipe)
    {
        EditText editionEquipe = findViewById(idEquipe);
        String   nomEquipe     = editionEquipe.getText().toString();
        Equipe   equipe        = new Equipe(nomEquipe);
        Log.d(TAG, "recupererNomEquipe() equipe = " + nomEquipe);
        intentDonneesPartieSuivi.putExtra("equipe" + numeroEquipe, (Serializable)equipe);
    }

    /**
     * @brief Méthode appelée pour récupérer le temps pour un tour d'une équipe
     */
    private void recupererTempsMaxTour()
    {
        EditText editionTempsTour = findViewById(R.id.editionTempsTour);
        String tempsMaxTourChoisi = editionTempsTour.getText().toString();

        int tempsMaxTour;
        if(!tempsMaxTourChoisi.isEmpty())
        {
            tempsMaxTour = Integer.parseInt(tempsMaxTourChoisi);
            Log.d(TAG, "recupererTempsTour() tempsMaxTour = " + tempsMaxTour);
        }
        else
        {
            tempsMaxTour = TEMPS_MAX_TOUR;
            Log.d(TAG, "recupererTempsMaxTour() temps tour par défaut = " + tempsMaxTour);
        }
        intentDonneesPartieSuivi.putExtra("tempsMaxTour", tempsMaxTour);
    }

    /**
     * @brief Méthode appelée pour récupérer le nombre de paniers
     */
    private void recupererNbPaniers()
    {
        Spinner choixNbPaniers = findViewById(R.id.choixNbPaniers);
        String nbPaniersChoisi = choixNbPaniers.getSelectedItem().toString();

        int nbPaniers = Integer.parseInt(nbPaniersChoisi);
        Log.d(TAG, "recupererNbPaniers() nombre paniers = " + nbPaniers);
        intentDonneesPartieSuivi.putExtra("nbPaniers", nbPaniers);
    }

    /**
     * @brief Méthode appelée pour récupérer le nombre de manches à jouer
     */
    private void recupererNbManches()
    {
        Spinner choixNbManches = findViewById(R.id.choixNbManches);
        String nbManchesChoisi = choixNbManches.getSelectedItem().toString();

        int nbManches = Integer.parseInt(nbManchesChoisi);
        Log.d(TAG, "recupererNbPaniers() nombre manches = " + nbManches);
        intentDonneesPartieSuivi.putExtra("nbManches", nbManches);
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
                recupererTempsMaxTour();
                recupererNbPaniers();
                recupererNbManches();
                Log.d(TAG,
                      "afficherSuiviPartie() " +
                        ((EditText)(findViewById(R.id.editionEquipe1))).getText().toString() +
                        " vs " +
                        ((EditText)(findViewById(R.id.editionEquipe2))).getText().toString());
                startActivity(intentDonneesPartieSuivi);
            }
        });
    }

    /**
     * @brief Méthode appelée pour contrôler le temps entre chaque tour
     */
    private void editerTempsTour()
    {
        EditText editionTempsTour = findViewById(R.id.editionTempsTour);
        editionTempsTour.setText(String.valueOf(TEMPS_MAX_TOUR)); // par défaut
        editionTempsTour.addTextChangedListener(new TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence texteSaisi,
                                          int          positionPremierCaractere,
                                          int          nbCaracteres,
                                          int          nbCaracteresApresChangement)
            {
            }

            @Override
            public void onTextChanged(CharSequence texteSaisi,
                                      int          positionPremierCaractere,
                                      int          nbCaracteresAvantChangement,
                                      int          nbCaracteres)
            {
                Log.d(TAG, "onTextChanged() tempsTour = " + texteSaisi);
            }

            @Override
            public void afterTextChanged(Editable texteSaisi)
            {
                String valeur = texteSaisi.toString();
                if(!valeur.isEmpty())
                {
                    int tempsTour = Integer.parseInt(valeur);
                    Log.d(TAG, "afterTextChanged() tempsTour = " + tempsTour);
                    if(tempsTour > TEMPS_MAX_TOUR)
                    {
                        editionTempsTour.setText(String.valueOf(TEMPS_MAX_TOUR));
                        Log.d(TAG, "afterTextChanged() tempsTour = " + tempsTour);
                    }
                }
            }
        });
    }
}