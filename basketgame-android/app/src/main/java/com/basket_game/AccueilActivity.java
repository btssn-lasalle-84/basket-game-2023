package com.basket_game;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

/**
 * @file AccueilActivity.java
 * @brief Déclaration de la classe AccueilActivity
 * @author Guillaumet Florent
 */

/**
 * @class AccueilActivity
 */
public class AccueilActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        afficherNouvellePartie();
    }

    /**
     * @brief Méthode appelée pour afficher la nouvelle partie
     */
    private void afficherNouvellePartie()
    {
        Button boutonNouvellePartie = findViewById(R.id.boutonNouvellePartie);
        boutonNouvellePartie.setOnClickListener(new View.OnClickListener()
        {
            @Override
            public void onClick(View v)
            {
                Intent intent = new Intent(AccueilActivity.this, ParametresPartieActivity.class);
                startActivity(intent);
            }
        });
    }
}