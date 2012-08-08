package rogueModel;

import rogueView.*;
import javax.swing.*;

/**
 * A teleporter allows the player to travel to the next floor.
 */
public class Teleporter extends Entity
{	
	/**
	 * Create a new teleporter in the specified window, in the specified dungeon,
	 * in the specified room, at the specified coordinates.
	 * 
	 * @param gameWindow - the window to spawn in
	 * @param dungeonWindow - the dungeon to spawn in
	 * @param room - the room to spawn in
	 * @param x - horizontal position
	 * @param y - vertical position
	 */
	public Teleporter(GameWindow gameWindow, DungeonWindow dungeonWindow, Room room, int x, int y)
	{
		super(gameWindow, dungeonWindow, room, x, y, new ImageIcon("teleporter.png"), "the teleporter");
	}
	
	/**
	 * Activates the element and transports the player to the next floor.
	 * @return Whether the element was activated (if it can't be activated, return false).
	 */
	public boolean activate()
	{
		getGameWindow().setMessage("You use the teleporter. Yay.");
		getGameWindow().nextLevel();
		return true;
	}
}
