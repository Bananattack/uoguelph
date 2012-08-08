package rogueModel;

import rogueView.*;
import javax.swing.*;

/**
 * An evil adversary of darkness set to thwart the player's non-descript quest.
 * @author Andrew G. Crowell
 */
public class Monster extends Creature
{
	/**
	 * Create a new monster in the specified window, in the specified dungeon,
	 * in the specified room, at the specified coordinates, with the specified character representation.
	 * 
	 * @param gameWindow - the window to spawn in
	 * @param dungeonWindow - the dungeon to spawn in
	 * @param room - the room to spawn in
	 * @param x - horizontal position
	 * @param y - vertical position
	 * @param name - the name
	 * @param combatStrategy the combat strategy
	 */
	public Monster(GameWindow gameWindow, DungeonWindow dungeonWindow, Room room, int x, int y, ImageIcon imageIcon, String name, CombatStrategy combatStrategy)
	{
		super(gameWindow, dungeonWindow, room, x, y, imageIcon, name, combatStrategy);
		this.setStats(Math.max(Helper.Random(gameWindow.getFloor() - 1, gameWindow.getFloor() * 3), 1),
				Helper.Random(2, 3),
				Helper.Random(3, 5),
				Helper.Random(2, 4),
				Helper.Random(75, 95),
				Helper.Random(5, 10));
	}
	
	/**
	 * Updating method used for NPCs.
	 * A logic rotuine called once each turn that deals with the element's AI.
	 */
	public void update()
	{
		if(isDead())
		{
			return;
		}
		Entity target;
		
		// Check for the player directly beside the monster.
		for(MapView.Direction direction: MapView.Direction.values())
		{
			target = findAdjacentCreature(direction);
			// If we find the player, attack it and end the update.
			if(target == getGameWindow().getPlayer())
			{
				attack((Creature) target);
				return;
			}
		}
		// No player adjacent, try to move in a random direction.
		move(Helper.getRandomDirection());
	}
	
}
