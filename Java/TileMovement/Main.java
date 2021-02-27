package dndCombat;
import java.util.*;


/*NOTES:
 * 	- May need some helper functions such as:
 * 		- Queue class + functions for processing recursive calls in BFS
 * 		- Menu function to move through other functions
 * 		- Delete / Reset functions to run multiple iterations
 * 
 * 	- Maybe make a cursor class and give THAT the keypress things
 * 		- Then instantiate cursor once the function that needs a cursor comes up
 * 
 * 	- Took away implements Key Listener for now
 */

public class Main{
	//Maintain one scanner for all of the main class
	private static Scanner scnr = new Scanner(System.in);
	public static Scanner getScanner()
	{
		return scnr;
	}

	private static Tile[][] tileMap;
	private static Unit player;
	private static Tile cursor;
	
	
	public static void main(String[] args) 
	{
		Scanner scnr = getScanner();
		char choice = 'y';
		
		do
		{
			if(choice == 'y')
			{
				tileMap = generateMap();
				player = generatePlayer();
				tileMap = bfsManager();
				//Give options to print map
				mapPrinter(tileMap);
				cursor = generateCursor(player);
			}
			System.out.println("\nWould you like to generate a new map? (y/n)");
			choice = scnr.next().charAt(0);
			
		} while(choice == 'y');
		
		
		
		scnr.close();
	}
	
	/*FINISHED:
	 *	- Given size generate a tile map with random tile types
	 *	- Put a player unit into the map at a random location
	 *	- Main contains player tile and tile map
	 */
	public static Tile[][] generateMap()
	{		
		//Initialization
		int rows = 10;
		int columns = 15;
		Tile[][] tileMap = new Tile[rows][columns];
		
		Random rand = new Random();
	
		
		//Iterate through each tile filling in row, column, move cost, and passibility
		for(int i = 0; i < rows; i++)
		{
			for(int j = 0; j < columns; j++)
			{
				tileMap[i][j] = new Tile();
				tileMap[i][j].row = i;
				tileMap[i][j].column = j;
				tileMap[i][j].moveCost = rand.nextInt(3) + 1;
				
				if(tileMap[i][j].moveCost == 3)
				{
					tileMap[i][j].moveCost = Integer.MAX_VALUE;
					tileMap[i][j].leftoverSpeed = -Integer.MAX_VALUE;
					tileMap[i][j].passible = false;
				}
				else
					tileMap[i][j].passible = true;
			}
		}
		
		//Once each tile is complete iterate again to sort adjacency
		//Note: I know this is inefficient 
		for(int i = 0; i < rows; i++)
		{
			for(int j = 0; j < columns; j++)
			{
				if(i - 1 >= 0)
					tileMap[i][j].up = tileMap[i-1][j];
				else
					tileMap[i][j].up = null;
				
				if(j - 1 >= 0)
					tileMap[i][j].left = tileMap[i][j-1];
				else
					tileMap[i][j].left = null;
				
				if(j + 1 < columns)
					tileMap[i][j].right = tileMap[i][j+1];
				else
					tileMap[i][j].right = null;
				
				if(i + 1 < rows)
					tileMap[i][j].down = tileMap[i+1][j];
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
		Scanner scnr = getScanner();
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
	public static Tile generateCursor(Unit player)
	{
		//Initialize cursor location
		Tile cursor = new Tile();
		cursor.row = player.row;
		cursor.column = player.column;		
		
		return cursor;
	}
	public static void mapPrinter(Tile[][] tileMap)
	{
		//Initializing
		Scanner scnr = getScanner();
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
						if(tileMap[i][j].hasUnit == true)
						{
							System.out.print(" P ");
							continue;
						}
						if(tileMap[i][j].passible == false)
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
				System.out.print("Enter 'm' to move. Press any other key to exit.");
				if(scnr.next().charAt(0) == 'm')
				{
					Tile start = new Tile();
					start.column = player.column;
					start.row = player.row;
					ArrayList<Tile> path = new ArrayList<Tile>();
					path.add(start);
					ArrayList<Tile> search = new ArrayList<Tile>();
					search.add(start);
					pathManager(start, cursor, path, search);
				}
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
					if(tileMap[i][j].hasUnit == true)
					{
						System.out.print(" P ");
						continue;
					}
					if(tileMap[i][j].passible == false)
						System.out.print(" x ");
					else
					{
						if(tileMap[i][j].leftoverSpeed == -Integer.MAX_VALUE)
							System.out.print(" X ");
						else
							System.out.print(" " + tileMap[i][j].leftoverSpeed + " ");
					}				
				}
			System.out.print("\n");
		}
	}
	
	
	/*FINISHED BUT NEEDS CLEANING:
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
	
		return bfsRecurse(tileMap, player.moveSpeed, searching, searched);
	}
	public static Tile[][] bfsRecurse(Tile[][] tileMap, int playerSpeed, ArrayList<Tile> searching, ArrayList<Tile> searched)
	{	
		//This will act like a queue front to search a tile at a time
		Tile head;
		
		//If the search is done then no more tiles will be marked to search
		if(searching.isEmpty() == true)
			return tileMap;
		else
		{
			//Sort the list of tiles marked for searching:
			/*	- Ensures we take efficient paths
			 *  - Accounts for if an originally equal path is discovered to be more efficient
			 *  - Remove highest leftover speed tile from the list
			 */
			sort(searching);
			head = searching.remove(searching.size() - 1);
		}
			

		/*	Adds adjacent tiles if they are passible, in bounds, not already searched, 
		*or marked to be searched from another path to a list marking them to be searched
		*/
		ArrayList<Tile>adj = new ArrayList<Tile>();
		if(searching.contains(head.up) == false &&  head.up != null && searched.contains(head.up) == false && head.up.passible == true)
		{
			head.up.leftoverSpeed = head.leftoverSpeed - head.up.moveCost;
			if(head.up.leftoverSpeed >= 0)
				searching.add(head.up);
		}
		if(searching.contains(head.left) == false &&  head.left != null && searched.contains(head.left) == false &&  head.left.passible == true)
		{
			head.left.leftoverSpeed = head.leftoverSpeed - head.left.moveCost;
			if(head.left.leftoverSpeed >= 0)
				searching.add(head.left);
		}
		if(searching.contains(head.right) == false &&  head.right != null &&  searched.contains(head.right) == false &&  head.right.passible == true)
		{
			head.right.leftoverSpeed = head.leftoverSpeed - head.right.moveCost;
			if(head.right.leftoverSpeed >= 0)
				searching.add(head.right);
		}
		if(searching.contains(head.down) == false &&  head.down != null &&  searched.contains(head.down) == false &&  head.down.passible == true)
		{
			head.down.leftoverSpeed = head.leftoverSpeed - head.down.moveCost;
			if(head.down.leftoverSpeed >= 0)
				searching.add(head.down);
		}
		
		//Add tile we just procesed to the searched category then recall
		searched.add(head);
		return (bfsRecurse(tileMap, playerSpeed, searching, searched));
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
	
	/*UNFINISHED:
	 *	- Given a searched tile map and tiles A and B return most efficient path
	 *
	 *	- Possibly keep this unfinished:
	 *		1. It's too burdensome in java to have keycodes and to update a map with every keystroke
	 *		2. I have homework due and I can't work on this more for the time being
	 *		3. The purpose of this was generating the leftover speed map. 
	 *	The pathfinding is a whole other issue. 
	 *	*** LOOK INTO DJIKSTRAS USING THE SEARCHED LIST FROM BFS RECURSE FUNCTION ***
	 *
	 *
	 */
	public static void pathManager(Tile start, Tile cursor, ArrayList<Tile> path, ArrayList<Tile> search)
	{
		Scanner scnr = getScanner();
		System.out.print("Enter how many spaces you would like to move horizontally (negative for left): ");
		System.out.print("Enter how many spaces you would like to move vertically (positive for right)");
		cursor.column = player.column;
		cursor.row = player.row;
		
		path = pathFinder(start, cursor, path, search);
		cursorMapPrinter(path);
		
	}
	public static ArrayList<Tile> pathFinder(Tile start, Tile cursor, ArrayList<Tile> path, ArrayList<Tile> search)
	{
		if(start == cursor)
		 return path;
		
		if(tileMap[cursor.row][cursor.column].leftoverSpeed < 0 || search.size() == 0)
			return null;
		
		sort(search);
		start = search.remove(search.size() - 1);
		path.add(start);
		
		if(path.contains(start.up) == false && start.up != null && search.contains(start.up) == false && start.up.passible == true)
		{
			search.add(start.up);
		}
		if(path.contains(start.right) == false && start.right != null && search.contains(start.right) == false && start.right.passible == true)
		{
			search.add(start.right);
		}
		if(path.contains(start.left) == false && start.left != null && search.contains(start.left) == false && start.left.passible == true)
		{
			search.add(start.left);
		}
		if(path.contains(start.down) == false && start.down != null && search.contains(start.down) == false && start.down.passible == true)
		{
			search.add(start.down);
		}

		
		return pathFinder(start, cursor, path, search);
	}
	public static void cursorMapPrinter(ArrayList<Tile> path)
	{
		if(path == null)
		{
			System.out.println("Error: path was null");
			return;
		}
		int rows = tileMap.length;
		int columns = tileMap[0].length;

		for(int i = 0; i < rows; i++)
		{
			for(int j = 0; j < columns; j++)
			{
				if(i == cursor.row && j == cursor.column)
					System.out.print(" O ");
				else if(path.contains(tileMap[i][j]))
					System.out.print(" o ");
				else if(tileMap[i][j].hasUnit == true)
					System.out.print(" P ");
				else if(tileMap[i][j].passible == false)
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
			System.out.println("");
		}
	}
	
}
