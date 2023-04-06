package com.basket_game;

public class Equipe
{
    private String equipe1;
    private String equipe2;

    public Equipe(String equipe1, String equipe2)
    {
        this.equipe1 = equipe1;
        this.equipe2 = equipe2;
    }

    public String getEquipe1()
    {
        return equipe1;
    }

    public void setEquipe1(String equipe1)
    {
        this.equipe1 = equipe1;
    }

    public String getEquipe2()
    {
        return equipe2;
    }

    public void setEquipe2(String equipe2)
    {
        this.equipe2 = equipe2;
    }
}