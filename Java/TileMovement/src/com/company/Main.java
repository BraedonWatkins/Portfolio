package com.company;
import java.util.*;


/*GENERAL NOTES:
 * 	- May want to look into some helper functions such as:
 * 		- Queue class + functions for processing recursive calls in BFS
 * 		- Menu function to move through other functions
 * 		- Delete / Reset functions to run multiple iterations
 *
 */


/*MAIN NOTES:
 *	- Change scanner to call from main
 *	- Review possible use of keywords
 *	-
 */
public class Main{
    //Maintain one scanner for all of the main class
    private static final Scanner scnr = new Scanner(System.in);

    private static Tile[][] tileMap;
    private static Unit player;


    public static void main(String[] args)
    {
        char choice;

        do
        {
            tileMap = generateMap();
            player = generatePlayer();
            tileMap = bfsManager();
            //Give options t
            // print map
            mapPrinter(tileMap);

            System.out.println("\nWould you like to generate a new map? (y/n)");
            choice = scnr.next().charAt(0);

        } while(choice == 'y');



        scnr.close();
    }

    /*GenerateMap Function:
     *	- Given size generate a tile map with random tile types
     *	- Put a player unit into the map at a random location
     *	- Main contains player tile and tile map
     */
    public static Tile[][] generateMap() {
        //Initialization
        int rows = 10;
        int columns = 15;
        Tile[][] tileMap = new Tile[rows][columns];

        Random rand = new Random();


        //Iterate through each tile filling in row, column, move cost, and passibility
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < columns; j++) {
                tileMap[i][j] = new Tile();
                tileMap[i][j].row = i;
                tileMap[i][j].column = j;
                tileMap[i][j].moveCost = rand.nextInt(3) + 1;

                if (tileMap[i][j].moveCost == 3) {
                    tileMap[i][j].moveCost = Integer.MAX_VALUE;
                    tileMap[i][j].leftoverSpeed = -Integer.MAX_VALUE;
                    tileMap[i][j].passible = false;
                } else
                    tileMap[i][j].passible = true;
            }
        }

        //Once each tile is complete iterate again to sort adjacency

        /*NOTES:
         *	- Can this be put in the last for loop?
         *	- It is not done when it iterates but perhaps the order we
         *	choose for adjacency allows us to use the same loop
         */
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < columns; j++) {
                if (i - 1 >= 0)
                    tileMap[i][j].up = tileMap[i - 1][j];
                else
                    tileMap[i][j].up = null;

                if (j - 1 >= 0)
                    tileMap[i][j].left = tileMap[i][j - 1];
                else
                    tileMap[i][j].left = null;

                if (j + 1 < columns)
                    tileMap[i][j].right = tileMap[i][j + 1];
                else
                    tileMap[i][j].right = null;

                if (i + 1 < rows)
                    tileMap[i][j].down = tileMap[i + 1][j];
                else
                    tileMap[i][j].down = null;

            }
        }

        //Give confirmation that the map has been generated
        System.out.println("Map " + rows + "x" + columns + " was generated:");

        return tileMap;
    }
    public static Unit generatePlayer()
    {
        //Initializations
        Random rand = new Random();
        Unit player = new Unit();

        //Give player to a random location + assign move speed
        player.row = rand.nextInt(tileMap.length);
        player.column = rand.nextInt(tileMap[0].length);
        player.moveSpeed = 12;


        //Reflect the players presence in the map
        tileMap[player.row][player.column].hasUnit = true;
        tileMap[player.row][player.column].moveCost = 0;

        return player;
    }
    public static void mapPrinter(Tile[][] tileMap)
    {
        //Initializing
        int rows = tileMap.length;
        int columns = tileMap[0].length;

        System.out.println("Enter: (1) Print Terrain (2) Print move costs (3) Done looking");
        int choice;
        do
        {
            choice = scnr.nextInt();
            switch(choice)
            {
                //Prints just terrain information
                //Note: Yes I know this is bulky
                case 1:
                    for(int i = 0; i < rows; i++)
                    {
                        for(int j = 0; j < columns; j++)
                        {
                            if(tileMap[i][j].hasUnit)
                            {
                                System.out.print(" P ");
                                continue;
                            }
                            if(!tileMap[i][j].passible)
                                System.out.print(" x ");
                            else
                            {
                                if(tileMap[i][j].moveCost == 1)
                                {
                                    System.out.print(" . ");
                                }
                                else if(tileMap[i][j].moveCost == 2)
                                {
                                    System.out.print(" t ");
                                }
                            }
                        }
                        System.out.print("\n");
                    }
                    break;

                //Prints move speed information
                case 2:
                    printMoveSpeed(tileMap, rows, columns);
                    /* OPTION TO MOVE CHARACTER: NEED TO RESEARCH DJIKSTRA'S
                     *
                     */
                    break;

                case 3:
                    break;
                default:
                    System.out.println("Invalid Entry.");
                    break;
            }
        } while(choice != 3);
    }
    public static void printMoveSpeed(Tile[][] tileMap, int rows, int columns)
    {
        //Iterates through map and prints unit (P), move speed (int), impassible (x), or too far (X)
        for(int i = 0; i < rows; i++)
        {
            for(int j = 0; j < columns; j++)
            {
                if(tileMap[i][j].hasUnit)
                {
                    System.out.print(" P ");
                    continue;
                }
                if(!tileMap[i][j].passible)
                    System.out.print(" x ");
                else if(tileMap[i][j].leftoverSpeed < 0)
                    System.out.print(" X ");
                else
                    System.out.print(" " + tileMap[i][j].leftoverSpeed + " ");
            }
            System.out.print("\n");
        }
    }


    /*FINISHED:
     * 	- Given a tile map and unit find potential move tiles
     * 	- For all potential move tiles associate numbers to represent efficient paths
     */
    public static Tile[][] bfsManager()
    {
        //Initialize lists
        ArrayList<Tile> searching = new ArrayList<>();
        ArrayList<Tile> searched = new ArrayList<>();

        searching.add(tileMap[player.row][player.column]);
        searching.get(0).leftoverSpeed = player.moveSpeed;

        return bfsRecurse(tileMap, searching, searched);
    }
    public static Tile[][] bfsRecurse(Tile[][] tileMap, ArrayList<Tile> searching, ArrayList<Tile> searched)
    {
        //This will act like a queue front to search a tile at a time
        Tile head;

        //If the search is done then no more tiles will be marked to search
        if(searching.isEmpty())
            return tileMap;
        else
        {
            //Sort the list of tiles marked for searching:
            /*	- Ensures we take efficient paths
             *	- Accounts for if an originally equal path is discovered to be more efficient
             *	- Remove highest leftover speed tile from the list
             *
             *	- Is this inefficient? What would be a better way?
             */
            sort(searching);
            head = searching.remove(searching.size() - 1);
        }


        /*	Adds adjacent tiles if they are:
         *	- passable
         *	- in bounds
         *	- not already searched
         *	- not marked from another path already
         */
        if(!searching.contains(head.up) &&  head.up != null && !searched.contains(head.up) && head.up.passible)
        {
            head.up.leftoverSpeed = head.leftoverSpeed - head.up.moveCost;
            if(head.up.leftoverSpeed > 0)
                searching.add(head.up);
        }
        if(!searching.contains(head.left) &&  head.left != null && !searched.contains(head.left) &&  head.left.passible)
        {
            head.left.leftoverSpeed = head.leftoverSpeed - head.left.moveCost;
            if(head.left.leftoverSpeed > 0)
                searching.add(head.left);
        }
        if(!searching.contains(head.right) &&  head.right != null &&  !searched.contains(head.right) &&  head.right.passible)
        {
            head.right.leftoverSpeed = head.leftoverSpeed - head.right.moveCost;
            if(head.right.leftoverSpeed > 0)
                searching.add(head.right);
        }
        if(!searching.contains(head.down) &&  head.down != null &&  !searched.contains(head.down) &&  head.down.passible)
        {
            head.down.leftoverSpeed = head.leftoverSpeed - head.down.moveCost;
            if(head.down.leftoverSpeed > 0)
                searching.add(head.down);
        }

        //Add tile we just processed to the searched category then recall
        searched.add(head);
        return (bfsRecurse(tileMap, searching, searched));
    }
    public static void sort(ArrayList<Tile> adj)
    {
        //Uses insertion sort to sort the tiles by their leftover speed
        //Note: May update with quicksort changing to insertion for low values
        int n = adj.size();

        for(int i = 1; i < n; ++i)
        {
            Tile key = adj.get(i);
            int j = i - 1;

            while(j >= 0 && adj.get(j).leftoverSpeed > key.leftoverSpeed)
            {
                adj.set(j+1, adj.get(j));
                j = j - 1;
            }
            adj.set(j+1, key);
        }
    }
}

