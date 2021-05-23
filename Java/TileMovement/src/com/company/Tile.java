package com.company;


/*Tile Notes:
    - I realize now that hasUnit is a has-a relationship
    meaning polymorphism can be used
 */
public class Tile
{

    int column;
    int row;

    boolean passible;

    int moveCost;
    int leftoverSpeed = -(Integer.MAX_VALUE);

    public Tile up;
    public Tile left;
    public Tile right;
    public Tile down;
}

