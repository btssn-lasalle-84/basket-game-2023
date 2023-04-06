package com.basket_game;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

public class AccueilActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        afficherNouvellePartie();
    }

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