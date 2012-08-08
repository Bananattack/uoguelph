package rogueModel;

import java.awt.event.*;
import java.util.*;
import java.io.*;

import rogueView.*;

/**
 * The main container class that glues together the game logic.
 * @author Andrew G. Crowell
 */
public class GameWindow implements KeyListener
{
	/**
	 * The default game file to save to.
	 */
	public static String saveFile = "save.";
	
	private static int KEY_REPEAT_RATE = 3;
	
	/**
	 * The entry-point for running the game.
	 * 
	 * @param commandline arguments, which aren't used.
	 */
    public static void main(String[] args) 
    {
    	new GameWindow("Purple Castle Awesome Time XIII");
    }
	
	private DungeonWindow dungeonWindow;
    private ArrayList<Room> rooms;
    private ArrayList<Hallway> halls;
    private ArrayList<KeypressInfo> keysPressed;
    private ArrayList<Entity> entities;
    private ArrayList<Entity> deadEntities;
    private ArrayList<String> messages;
    private Player player;
    // For tracking the teleporter when spawning monsters,
    // so things don't get placed on it.
    private Room teleporterRoom;
    private int teleporterX, teleporterY; 
    
    private int floor = 0;
    private int lastFloor = 6;
    private boolean wonTheGame = false;
    private boolean finishMessage = false;

    /**
     * Creates a game window with the specified title
     */
    public GameWindow(String title)
    {
    	keysPressed = new ArrayList<KeypressInfo>();
		rooms = new ArrayList<Room>();
		halls = new ArrayList<Hallway>();
		entities = new ArrayList<Entity>();
		deadEntities = new ArrayList<Entity>();
		messages = new ArrayList<String>();
	
		dungeonWindow = new DungeonWindow(title, this);
		dungeonWindow.setDungeonFont("Verdana");
		nextLevel();
		dungeonWindow.setMessage("");
		dungeonWindow.setStats("Arrow Keys = move/attack in direction\nA = activate adjacent object");
    }
    
   public void loadGame(String filename)
    {
    	Scanner scanner = null;
    	try
    	{
    		scanner = new Scanner(new File(filename));
    		int level = scanner.nextInt();
    		int exp = scanner.nextInt();
    		int hp = scanner.nextInt();
    		int attack = scanner.nextInt();
    		int defense = scanner.nextInt();
    		int accuracy = scanner.nextInt();
    		int evasion = scanner.nextInt();
    		
    		player.setStats(level, hp, attack, defense, accuracy, evasion);
    		player.setExp(exp);
    		int size = scanner.nextInt();
    		ArrayList<Integer> indices = new ArrayList<Integer>();
    		for(int i = 0; i < size; i++)
    		{
    			indices.add(scanner.nextInt());
    		}
    		player.setInventory(new Inventory(this, indices));
    		scanner.close();
    		setMessage("Successfully loaded the player from disk!");
    	}
    	catch(IOException e)
    	{
    		setMessage("Couldn't find " + filename);
    	}
    }
    
    public void saveGame(String filename)
    {
    	try
    	{
    		PrintWriter writer = new PrintWriter(new BufferedWriter(new FileWriter(filename)));
    		writer.println(player.getLevel() + " " + player.getExp() + " " + player.getMaxHP() + " " + player.getAttack() + " " + player.getDefense() + " " + player.getAccuracy() + " " + player.getEvasion());
    		Inventory inv = player.getInventory();
    		writer.println(inv.getSize());
    		for(Integer index : inv.toIndices())
    		{
    			writer.println(index);
    		}
    		writer.close();
    		setMessage("Successfully saved the player to disk!");
    	}
    	catch(IOException e)
    	{
    		setMessage("Couldn't save " + filename);
    	}
    }


	/**
	 * Returns the entity that is this game's player.
	 * @return the player
	 */
    public Entity getPlayer()
    {
    	return player;
    }
    
    /**
	 * Returns the current floor. Useful for logic tying floors to relative difficulty.
	 * @return the floor number
	 */
    public int getFloor()
    {
    	return floor;	
    }
    
    /**
	 * Picks a free capital letter of the alphabet by random.
	 * 
	 * @return a free letter, or null if all letters are taken
	 */
    public Character selectRandomLetter()
    {
    	return (char) Helper.Random('A', 'Z');
    }
    
    /**
     * Screws around with the current level.
     */
    public void hallucinate()
    {
    	int size = entities.size();
    	for(int i = 0; i < size; i++)
    	{
    		Entity entity = entities.get(i);
    		int count = 1;
    		
    		if(entity instanceof Monster)
    		{
    			Room room = rooms.get(Helper.Random(0, rooms.size() - 1));
    			int x = Helper.Random(1, room.getWidth() - 1);
	    		int y = Helper.Random(1, room.getLength() - 2);
	    		entity.teleport(room, x, y);
	    		
	    		for(int j = 0; j < count; j++)
	    		{
	    			room = rooms.get(Helper.Random(0, rooms.size() - 1));
	    			x = Helper.Random(1, room.getWidth() - 1);
	    			y = Helper.Random(1, room.getLength() - 2);
	    			new HallucinationMonster(this, dungeonWindow, room, x, y, entity.getImageIcon());
	    		}
    		}
    	}
    	setMessage("You begin hallucinating, and feel very uneasy.");
    }
    
    /**
     * Generates a new dungeon level, and increases the floor number.
     */
    public void nextLevel()
    {
    	Room room;
    	entities.clear();
    	deadEntities.clear();
    	rooms.clear();
    	halls.clear();
    	
    	floor++;
    	if (floor > lastFloor)
    	{
    		wonTheGame = true;
    		nextMessage();
    		return;
    	}
    	for(int i = 0; i < 9; i++)
    	{
    		rooms.add(new Room(Helper.Random(5, 9),
    						Helper.Random(5, 9)
    					//Helper.Random(15, 20)
    						)
    				);
    	}
    	
    	try
    	{
    		int hallType = Helper.Random(0, 2);
    		if(hallType == 0)
    		{
		    	halls.add(new Hallway(rooms.get(3), MapView.Direction.SOUTH,
						rooms.get(6), MapView.Direction.NORTH));
		    	halls.add(new Hallway(rooms.get(2), MapView.Direction.SOUTH,
						rooms.get(5), MapView.Direction.NORTH));
		    	
		    	halls.add(new Hallway(rooms.get(0), MapView.Direction.EAST,
						rooms.get(1), MapView.Direction.WEST));
		    	halls.add(new Hallway(rooms.get(1), MapView.Direction.EAST,
						rooms.get(2), MapView.Direction.WEST));
		    	
		    	halls.add(new Hallway(rooms.get(3), MapView.Direction.EAST,
						rooms.get(4), MapView.Direction.WEST));
		    	halls.add(new Hallway(rooms.get(4), MapView.Direction.EAST,
						rooms.get(5), MapView.Direction.WEST));
		    	
		    	halls.add(new Hallway(rooms.get(6), MapView.Direction.EAST,
						rooms.get(7), MapView.Direction.WEST));
		    	halls.add(new Hallway(rooms.get(7), MapView.Direction.EAST,
						rooms.get(8), MapView.Direction.WEST));
    		}
    		else if(hallType == 1)
    		{
		    	halls.add(new Hallway(rooms.get(3), MapView.Direction.SOUTH,
						rooms.get(6), MapView.Direction.NORTH));
		    	halls.add(new Hallway(rooms.get(2), MapView.Direction.SOUTH,
						rooms.get(5), MapView.Direction.NORTH));
		    	halls.add(new Hallway(rooms.get(0), MapView.Direction.SOUTH,
						rooms.get(3), MapView.Direction.NORTH));
		    	halls.add(new Hallway(rooms.get(1), MapView.Direction.EAST,
						rooms.get(2), MapView.Direction.WEST));
		    	halls.add(new Hallway(rooms.get(1), MapView.Direction.SOUTH,
						rooms.get(4), MapView.Direction.NORTH));
		    	halls.add(new Hallway(rooms.get(3), MapView.Direction.EAST,
						rooms.get(4), MapView.Direction.WEST));
		    	halls.add(new Hallway(rooms.get(4), MapView.Direction.EAST,
						rooms.get(5), MapView.Direction.WEST));
		    	halls.add(new Hallway(rooms.get(5), MapView.Direction.SOUTH,
						rooms.get(8), MapView.Direction.NORTH));
		    	halls.add(new Hallway(rooms.get(7), MapView.Direction.EAST,
						rooms.get(8), MapView.Direction.WEST));
    		}
    		else if(hallType == 2)
    		{
		    	halls.add(new Hallway(rooms.get(3), MapView.Direction.SOUTH,
						rooms.get(6), MapView.Direction.NORTH));
		    	halls.add(new Hallway(rooms.get(2), MapView.Direction.SOUTH,
						rooms.get(5), MapView.Direction.NORTH));
		    	halls.add(new Hallway(rooms.get(0), MapView.Direction.SOUTH,
						rooms.get(3), MapView.Direction.NORTH));
		    	halls.add(new Hallway(rooms.get(1), MapView.Direction.EAST,
						rooms.get(2), MapView.Direction.WEST));
		    	halls.add(new Hallway(rooms.get(1), MapView.Direction.SOUTH,
						rooms.get(4), MapView.Direction.NORTH));
		    	halls.add(new Hallway(rooms.get(6), MapView.Direction.EAST,
						rooms.get(7), MapView.Direction.WEST));
		    	halls.add(new Hallway(rooms.get(5), MapView.Direction.SOUTH,
						rooms.get(8), MapView.Direction.NORTH));
		    	halls.add(new Hallway(rooms.get(4), MapView.Direction.SOUTH,
						rooms.get(7), MapView.Direction.NORTH));
    		}
    	}
    	catch(Room.DoorOccupiedException e)
    	{
    		System.out.println("Room creation failed - hallway doors overlap.");
    		System.exit(-1);
    	}
        
        try
        {
        	dungeonWindow.setNewLevel(rooms, halls);
        }
        catch(BadMapException e)
        {
        	System.out.println("Bad map exception");
        }
        catch(NoDoorThereException e)
        {
        	System.out.println("No door there exception");
        }
		if(player == null)
		{
			player = new Player(this, dungeonWindow, rooms.get(0), 1, 1);
		}
		else
		{
			player.spawn(rooms.get(0), 1, 1);
		}
		
		
		// Create a teleporter!
		teleporterRoom = rooms.get(Helper.Random(4, rooms.size() - 1));
		teleporterX = -1;
		teleporterY = -1;
    	if(floor <= lastFloor)
    	{
    		teleporterX = teleporterRoom.getWidth() / 2;
    		teleporterY = teleporterRoom.getLength() / 2;
    		new Teleporter(this, dungeonWindow, teleporterRoom, teleporterX, teleporterY);
    	}
		

    	for(int i = 0; i < 15; i++)
    	{
	    	int x, y;
	    	int itemType = Helper.Random(0, 2);
	    	room = rooms.get(Helper.Random(0, rooms.size() - 1));
			do
			{
				x = Helper.Random(1, room.getWidth() - 2);
				y = Helper.Random(1, room.getLength() - 2);
			} while(room == teleporterRoom && teleporterX == x && teleporterY == y);
			switch(itemType)
			{
				case 0:
					new MagicalToasterStrudel(this, dungeonWindow, room, x, y);
					break;
				case 1:
					new Armor(this, dungeonWindow, room, x, y);
					break;
				case 2:
					new Potion(this, dungeonWindow, room, x, y);
					break;
			}
    	}
    	for(int i = 0; i < 8; i++)
    	{
    		Monster m = null;
    		int x, y;
    		room = rooms.get(Helper.Random(0, rooms.size() - 1));
    		// Ensure monsters can't spawn over the teleporter, because that's badstuffs.
    		do
    		{
    			x = Helper.Random(1, room.getWidth() - 1);
    			y = Helper.Random(1, room.getLength() - 2);
    		} while(room == teleporterRoom && teleporterX == x && teleporterY == y);
    		//m = new Monster(this, dungeonWindow, room, x, y, new ImageIcon("hero2.png"));
    		switch(Helper.Random(0, 2))
    		{
    			case 0:
    				m = new Ghost(this, dungeonWindow, room, x, y);
    				break;
    			case 1:
    				m = new Slime(this, dungeonWindow, room, x, y);
    				break;
    			case 2:
    				m = new Zombie(this, dungeonWindow, room, x, y);
    				break;
    		}
    		
    		// Wiggle the monster back and forth, and then it'll refresh their text character.
    		if(m.move(MapView.Direction.NORTH))
    		{
    			m.move(MapView.Direction.SOUTH);
    		}
    		if(m.move(MapView.Direction.SOUTH))
    		{
    			m.move(MapView.Direction.NORTH);
    		}
    		if(m.move(MapView.Direction.EAST))
    		{
    			m.move(MapView.Direction.WEST);
    		}
    		if(m.move(MapView.Direction.WEST))
    		{
    			m.move(MapView.Direction.EAST);
    		}
    	}
    	dungeonWindow.refreshMap();
    }

	/**
	 * Parses the supplied keyboard input and acts on it.
	 * 
	 * @param scancode - the KeyEvent scancode to examine.
	 */
    public void parseInput(int scancode)
    {
    	// If there are still messages on the queue, remove them one-by-one.
		if(messages.size() > 0)
		{	
			nextMessage();
		}
		// Otherwise, return to game logic on each keypress.
		else
		{
			if (!wonTheGame)
			{
				player.parseInput(scancode);
			}
			nextMessage();
		}
    }
    
    /**
     * Called each refresh in which a key is held down.
     * @param key event used with the KeyListener interface
     */
    public void keyPressed(KeyEvent e)
    {   
    	KeypressInfo keyPress = new KeypressInfo(e.getKeyCode());
    	
    	// Try to see if the key's already being held down.
    	int index = keysPressed.indexOf(keyPress);
    	if(index != -1)
    	{
    		keyPress = keysPressed.get(index);
    		keyPress.increaseIterationsHeld();
    		
    		// Parse input every so many iterations.
    		// Assumes this is a nice repeat rate.
    		if (keyPress.getIterationsHeld() % KEY_REPEAT_RATE == 0)
    		{
    			parseInput(keyPress.getScancode());
    		}
    	}
    	else
    	{
    		parseInput(keyPress.getScancode());
    		
    		// Remember the key being pressed,
    		// this allows us to slow down the key repeat rate.
    		keysPressed.add(keyPress);
    	}
    }

    /**
     * Called each time a key is let go of.
     * @param key event used with the KeyListener interface
     */
    public void keyReleased(KeyEvent e)
    {
    	KeypressInfo keyPress = new KeypressInfo(e.getKeyCode());
    	if(keysPressed.indexOf(keyPress) != -1)
    	{
    		// Now that the key has been let go of, it can be pressed again.
    		keysPressed.remove(keysPressed.indexOf(keyPress));
    	}
    }
    
    /**
     * Called each time an printable letter is typed out.
     * 
     * @param key event used with the KeyListener interface
     */
    public void keyTyped(KeyEvent e)
    {
    }
    
    /**
     * Processes the game logic for each "turn".
     */
    public void update()
    {
    	// Iterate through each entity, and update it.
    	for(int i = 0; i < entities.size(); i++)
    	{
    		Entity entity = entities.get(i);
    		entity.update();
    	}
    	
    	// Remove all entities that were flagged as destroyed.
    	// Needs to be done afterward to ensure no concurrent modification occurs.
    	cleanupDead();
    	
    	// Refresh the map display
    	dungeonWindow.refreshMap();
    	
    	// Update the player stats.
    	dungeonWindow.setStats("Floor " + floor + "\n" 
    				+ ((player.isDead()) ? "R.I.P." : player.getCapitalizedName())
    				+ "\nLV: " + player.getLevel() + "\nNext: " + player.getExpToNext()
    			+ "\nHP: " + player.getHP() + "\t/ " + player.getMaxHP()
    			+ "\nATK: " + player.getAttack()
    			+ "\nDEF: " + player.getDefense()
    			+ "\nACC: " + player.getAccuracy());
    }
    
    /**
     * Grabs a message from queue and displays it onscreen
     */
    public void nextMessage()
    {
		if(messages.size() > 0)
		{
			String message = messages.remove(0);
			dungeonWindow.setMessage(message + ((messages.size() > 0) ? " (more...)" : ""));
		}
		else
		{
			if (player.isDead())
			{
				dungeonWindow.setMessage("You lost. Game over.");
				dungeonWindow.popupMessage("You lost. Game over.");
			}
			else if(wonTheGame)
			{
				if(!finishMessage)
				{
					dungeonWindow.setMessage("You won!");
					dungeonWindow.popupMessage("After a long and difficult journey, you have at last won!");
					finishMessage = true;
				}
			}
			else
			{
				dungeonWindow.setMessage("");	
			}
		}
    }
    
    /**
     * Queues a message to eventually be displayed in the window 
     * @param message to display
     */
    public void setMessage(String message)
    {
    	if(message != "")
    	{
    		messages.add(message);
    	}
    }
    
    /**
     * Dispose of the dead creatures on-map.
     */
    public void cleanupDead()
    {
    	for(int i = 0; i < deadEntities.size(); i++)
    	{
    		Entity entity = deadEntities.get(i);
    		try
    		{
    			dungeonWindow.getMap().removeElement(entity);
    		}
    		catch(MapNotSetException e)
    		{
    		}
    		catch(rogueView.NoSuchElementException e)
    		{
    		}
    		entities.remove(entity);
    	}
    	deadEntities.clear();
    }
    
    /**
     * Adds an NPC to the map, allowing it to be updated each iteration of game turn logic.
     * 
     * @param the entity to insert to the map
     */
    public void addEntity(Entity se)
    {
    	entities.add(se);
    }
    
    /**
     * Prepares the entity for removal.
     * Need to call cleanupDead() when safely outside of NPC list iteration
     * to set the deletion in effect.
     * 
     * @param the entity to dispose of
     */
    public void destroyEntity(Entity se)
    {
    	deadEntities.add(se);
    }
}
