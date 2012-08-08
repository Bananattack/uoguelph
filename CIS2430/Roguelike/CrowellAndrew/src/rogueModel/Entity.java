package rogueModel;

import rogueView.*;
import javax.swing.*;

/**
 * A game element represents the things that are in the dungeon, with some extras.
 * It was said we couldn't change the StoryElement class, so I decided that
 * inheritance was the easiest workaround.
 * @author Andrew G. Crowell
 */
public abstract class Entity extends StoryElement
{
	private boolean dead = false;
	private GameWindow gameWindow;
	private DungeonWindow dungeonWindow;
	private String name;
	private ImageIcon imageIcon;
	
	/**
	 * Create a new game element in the specified window, in the specified dungeon,
	 * in the specified room, at the specified coordinates, with the specified name.
	 * 
	 * @param gameWindow - the window to spawn in
	 * @param dungeonView - the dungeon to spawn in
	 * @param room - the room to spawn in
	 * @param x - horizontal position
	 * @param y - vertical position
	 * @param name - what the element is called
	 */
	public Entity(GameWindow gameWindow, DungeonWindow dungeonWindow, Room room, int x, int y, ImageIcon imageIcon, String name)
	{
		this.imageIcon = imageIcon;
		this.gameWindow = gameWindow;
		this.dungeonWindow = dungeonWindow;
		this.name = name;
		gameWindow.addEntity(this);
		spawn(room, x, y);
	}
	
	/**
	 * Spawns the element in the specified room at the specified coordinates.
	 * Essential for reattaching entities to the dungeon when you switch floors.
	 * 
	 * @param room - the room to spawn in
	 * @param x - horizontal position
	 * @param y - vertical position
	 */
	public void spawn(Room room, int x, int y)
	{
		try
		{
			if(dungeonWindow == null)
			{
				return;
			}
			dungeonWindow.getMap().setElementStartPoint(this, room, x, y);
		}
		catch(MapNotSetException e)
		{
		}
		catch(OutOfRangeException e)
		{
		}
	}
	
	/**
	 * Prepares the element for deletion from the dungeon.
	 * After this call there is no guarantee that
	 * anymore dungeon specific things can be done with this element.
	 */
	public void destroy()
	{
		if(!dead)
		{
			dead = true;
			gameWindow.destroyEntity(this);
		}
	}
	
	public void teleport(Room room, int x, int y)
	{
		try
		{
			dungeonWindow.getMap().removeElement(this);
			dungeonWindow.getMap().setElementStartPoint(this, room, x, y);
		}
		catch(MapNotSetException e)
		{
		}
		catch(NoSuchElementException e)
		{
		}
		catch(OutOfRangeException e)
		{
		}
	}
	
	/**
	 * Predicate that returns whether an element is dead
	 * (unsafe for further dungeon operations)
	 */
	public boolean isDead()
	{
		return dead;
	}
	
	/**
	 * Whether or not this can be walked over.
	 */
	public boolean isObstruction()
	{
		return false;
	}
	
	public boolean stayOffEntities()
	{
		return true;
	}
	
	/**
	 * Returns the element's dungeon view.
	 * @return element's associated DungeonView
	 */
	public DungeonWindow getDungeonWindow()
	{
		return dungeonWindow;
	}
	
	/**
	 * Returns the element's game window.
	 * @return element's associated GameWindow
	 */
	public GameWindow getGameWindow()
	{
		return gameWindow;
	}
	
	/**
	 * Returns the room where the element is current situated.
	 * @return a room this element exists in.
	 */
	public DungeonArea getDungeonArea()
	{
		try
		{
			return dungeonWindow.getMap().getElementArea(this);
		}
		catch(MapNotSetException e)
		{
			return null;
		}
		catch(NoSuchElementException e)
		{
			return null;
		}
	}
	
	/**
	 * Moves the element in the specified direction if possible.
	 * @return Whether or not the element could be moved in the direction
	 */
	public boolean move(MapView.Direction direction)
	{
		if(!findObstruction(direction))
		{
			try
			{
				dungeonWindow.getMap().moveStoryElement(this, direction);
			}
			catch(AdjacentSquareException e)
			{
			}
			catch(MapNotSetException e)
			{
			}
			catch(NoSuchElementException e)
			{
			}
			return true;
		}
		return false;
	}
	
	/**
	 * Inspects a direction for an obstruction.
	 * @return Whether or not there is something blocking the specified direction
	 */
	public boolean findObstruction(MapView.Direction direction)
	{
		Entity se = findAdjacentCreature(direction);
		if (se != null && (se.isObstruction() || stayOffEntities()))
		{
			return true;
		}
		
		Tile tile = null;
		try
		{
			 tile = dungeonWindow.getMap().getContentsOfAdjacentSquare(this, direction);
		}
		catch(AdjacentSquareException e)
		{
		}
		catch(MapNotSetException e)
		{
		}
		catch(NoSuchElementException e)
		{
		}
		return tile.isObstructed();
	}
	
	/**
	 * Checks if there is an element in an adjacent square.
	 * 
	 * @return a reference to the element in the adjacent square, or null
	 */
	public Entity findAdjacentCreature(MapView.Direction direction)
	{
		if(direction == null)
		{
			return null;
		}
		try
		{
			StoryElement se = dungeonWindow.getMap().getElementInAdjacentSquare(this, direction);
			if(se != null && se instanceof Entity)
			{
				return (Entity) se;
			}
		}
		catch(AdjacentSquareException e)
		{
		}
		catch(MapNotSetException e)
		{
		}
		catch(NoSuchElementException e)
		{
		}
		return null;
	}

	/**
	 * Returns the element's name 
	 * @return the name
	 */
	public String getName()
	{
		return name;
	}

	/**
	 * Returns the element's name, with first letter capitalized
	 * @return the name
	 */
	public String getCapitalizedName()
	{
		return name.substring(0, 1).toUpperCase() + name.substring(1);
	}

	/**
	 * Returns the element's name 
	 * @return the name
	 */
	public void setName(String name)
	{
		this.name = name;
	}
	
	/**
	 * This method returns the text character that is used to represent the element in the
	 * text display.
	 * @return a Character that should be used to represent the element.
	 */
	public ImageIcon getImageIcon()
	{
		return imageIcon;
	}
	
	/**
	 * Updating method used for NPCs.
	 * A logic rotuine called once each turn that deals with the element's AI.
	 */
	public void update()
	{
	}
	
	/**
	 * A method used upon "activation" of the element.
	 * Useful for stuff like teleporters and other interactive non-hostile objects.
	 * @return Whether the element was activated (if it can't be activated, return false).
	 */
	public boolean activate()
	{
		return false;
	}
}
