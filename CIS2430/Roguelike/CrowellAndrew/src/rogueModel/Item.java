package rogueModel;

import rogueView.*;
import javax.swing.*;

/**
* An item is an thing in the inventory.
* @author Andrew G. Crowell
*/
public abstract class Item extends Entity
{	
	/**
	* Create an item
	* @param gameWindow the game window
	* @param dungeonWindow the dungeon window
	* @param x the horizontal position
	* @param y the vertical position
	* @param imageIcon the icon
	* @param name the name
	*/
	public Item(GameWindow gameWindow, DungeonWindow dungeonWindow, Room room, int x, int y, ImageIcon imageIcon, String name)
	{
		super(gameWindow, dungeonWindow, room, x, y, imageIcon, name);
	}
	
	public boolean isObstruction()
	{
		return false;
	}
	
	public abstract void use(Creature user);
	public abstract void wear(Creature user);
	
}
