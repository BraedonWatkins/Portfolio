package com.company;

public class Tile
{

    int column;
    int row;

    boolean hasUnit;
    boolean passible;

    int moveCost;
    int leftoverSpeed = -(Integer.MAX_VALUE);

    public Tile up;
    private void up(Tile up)
    {
        up.column = this.column;
        up.row = this.row++;
    }

    public Tile left;
    private void left(Tile left)
    {
        left.column = this.column--;
        left.row = this.row;
    }

    public Tile right;
    private void right(Tile right)
    {
        right.column = this.column++;
        right.row = this.row;
    }

    public Tile down;
    private void down(Tile down)
    {
        down.column = this.column;
        down.row = this.row--;
    }
}

