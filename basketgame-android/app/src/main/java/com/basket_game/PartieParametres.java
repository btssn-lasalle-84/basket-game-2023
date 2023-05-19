/**
 * @file PartieParametres.java
 * @brief Déclaration de la classe PartieParametres
 * @author Guillaumet Florent
 */

package com.basket_game;

import static com.basket_game.Partie.TEMPS_MAX_TOUR;
import static com.basket_game.Partie.TEMPS_TOUR_PAR_DEFAUT;

import androidx.appcompat.app.AppCompatActivity;

import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.os.Bundle;
import android.text.Editable;
import android.text.TextWatcher;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Spinner;

import java.io.IOException;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.io.Serializable;
import java.io.Writer;
import java.nio.charset.StandardCharsets;

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
    public static final int NUMERO_ID_PANIERS_DEFAUT = 2;  //!< le nombre max de paniers gérables

    /**
     * Attributs
     */
    private Intent intentDonneesPartieSuivi;
    private String nomEquipe1;
    private String nomEquipe2;
    private int tempsMaxTour;
    private int nbPaniers;
    private int nbManches;

    /**
     * @brief Méthode appelée à la création de l'activité
     */
    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.partie_parametres);
        Log.d(TAG, "onCreate()");
        initialiserActivite();
        afficherSuiviPartie();
    }

    /**
     * @brief Méthode qui intialise la vue de l'activité
     */
    private void initialiserActivite()
    {
        Spinner choixNbPaniers = findViewById(R.id.choixNbPaniers);
        choixNbPaniers.setSelection(NUMERO_ID_PANIERS_DEFAUT); //!< Par défaut 4 paniers
        intentDonneesPartieSuivi = new Intent(PartieParametres.this, PartieSuivi.class);
        editerTempsTour();
    }

    /**
     * @brief Méthode appelée pour récupérer le nom d'une équipe
     */
    private void recupererNomEquipe(int idEquipe, int numeroEquipe)
    {
        EditText editionEquipe = findViewById(idEquipe);
        String   nomEquipe     = editionEquipe.getText().toString();
        Equipe   equipe        = new Equipe(nomEquipe);

        if(numeroEquipe == 1)
        {
            nomEquipe1 = nomEquipe;
        }
        else if (numeroEquipe == 2)
        {
            nomEquipe2 = nomEquipe;
        }

        Log.d(TAG, "recupererNomEquipe() equipe = " + nomEquipe);
        intentDonneesPartieSuivi.putExtra("equipe" + numeroEquipe, (Serializable)equipe);
    }

    /**
     * @brief Méthode appelée pour récupérer le temps pour un tour d'une équipe
     */
    private void recupererTempsMaxTour()
    {
        EditText editionTempsTour   = findViewById(R.id.editionTempsTour);
        String   tempsMaxTourChoisi = editionTempsTour.getText().toString();

        if(!tempsMaxTourChoisi.isEmpty())
        {
            tempsMaxTour = Integer.parseInt(tempsMaxTourChoisi);
            Log.d(TAG, "recupererTempsTour() tempsMaxTour = " + tempsMaxTour);
        }
        else
        {
            tempsMaxTour = TEMPS_TOUR_PAR_DEFAUT;
            Log.d(TAG, "recupererTempsMaxTour() tempsMaxTour = " + tempsMaxTour);
        }
        intentDonneesPartieSuivi.putExtra("tempsMaxTour", tempsMaxTour);
    }

    /**
     * @brief Méthode appelée pour récupérer le nombre de paniers
     */
    private void recupererNbPaniers()
    {
        Spinner choixNbPaniers  = findViewById(R.id.choixNbPaniers);
        String  nbPaniersChoisi = choixNbPaniers.getSelectedItem().toString();

        nbPaniers = Integer.parseInt(nbPaniersChoisi);
        Log.d(TAG, "recupererNbPaniers() nbPaniers = " + nbPaniers);
        intentDonneesPartieSuivi.putExtra("nbPaniers", nbPaniers);
    }

    /**
     * @brief Méthode appelée pour récupérer le nombre de manches à jouer
     */
    private void recupererNbManches()
    {
        Spinner choixNbManches  = findViewById(R.id.choixNbManches);
        String  nbManchesChoisi = choixNbManches.getSelectedItem().toString();

        nbManches = Integer.parseInt(nbManchesChoisi);
        Log.d(TAG, "recupererNbManches() nbManches = " + nbManches);
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
                envoyerTrame();
                Log.d(TAG,
                      "afficherSuiviPartie() " +
                        ((EditText)(findViewById(R.id.editionEquipe1))).getText().toString() +
                        " vs " +
                        ((EditText)(findViewById(R.id.editionEquipe2))).getText().toString());
                Log.d(TAG,
                      "afficherSuiviPartie() tempsMaxTour = " +
                        ((EditText)(findViewById(R.id.editionTempsTour))).getText().toString());
                Log.d(
                  TAG,
                  "afficherSuiviPartie() nbPaniers = " +
                    ((Spinner)(findViewById(R.id.choixNbPaniers))).getSelectedItem().toString());
                Log.d(
                  TAG,
                  "afficherSuiviPartie() nbManches = " +
                    ((Spinner)(findViewById(R.id.choixNbManches))).getSelectedItem().toString());
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

    /**
     * @brief Méthode appelée pour envoyer la trame avec les informations de la partie
     */
    private void envoyerTrame()
    {
        BluetoothSocket socket = null;
        /**
         * @todo Obtenir BluetoothSocket
         */

        String trame = "$BASKET;DONNEES;" + nomEquipe1 + ";" + nomEquipe2 + ";" + tempsMaxTour + ";" + nbPaniers + ";" + nbManches + ";\r\n";

        try {
            OutputStream outputStream = socket.getOutputStream();
            Writer writer = new OutputStreamWriter(outputStream, StandardCharsets.UTF_8);

            writer.write(trame);
            writer.flush();

            Log.d(TAG, "envoyerTrame() Trame = " + trame);
        } catch (IOException erreur) {
            Log.e(TAG, "envoyerTrame() Erreur = " + erreur.getMessage());
        }

    }
}