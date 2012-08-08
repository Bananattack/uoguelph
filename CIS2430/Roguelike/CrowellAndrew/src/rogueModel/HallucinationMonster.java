package rogueModel;
import rogueView.*;
import javax.swing.*;

/**
 * An evil adversary of darkness set to thwart the player's non-descript quest.
 * @author Andrew G. Crowell
 */
public class HallucinationMonster extends Monster
{
	/**
	 * Create a new monster in the specified window, in the specified dungeon,
	 * in the specified room, at the specified coordinates, with the specified character representation.
	 * 
	 * @param gameWindow - the window to spawn in
	 * @param dungeonView - the dungeon to spawn in
	 * @param room - the room to spawn in
	 * @param x - horizontal position
	 * @param y - vertical position
	 * @param textCharacter - the letter representing the monster
	 */
	public HallucinationMonster(GameWindow gameWindow, DungeonWindow dungeonWindow, Room room, int x, int y, ImageIcon imageIcon)
	{
		super(gameWindow, dungeonWindow, room, x, y, imageIcon, "the hallucination", new HallucinationAttack());
		this.setStats(0, 0, 0, 0, 0, 0);
	}
}
