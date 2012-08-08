package rogueModel;

import javax.swing.ImageIcon;

import rogueView.DungeonWindow;

/**
* It's a g-g-g-g-ghost!
* @author Andrew G. Crowell
*/
public class Ghost extends Monster
{
	public Ghost(GameWindow gameWindow, DungeonWindow dungeonWindow, Room room, int x, int y)
	{
		super(gameWindow, dungeonWindow, room, x, y, new ImageIcon("ghost.png"), "g-g-g-ghost", new Steal());
	}
}
