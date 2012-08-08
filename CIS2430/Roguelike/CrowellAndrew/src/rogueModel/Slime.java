package rogueModel;

import javax.swing.ImageIcon;

import rogueView.DungeonWindow;

/**
* A mischievous slime monster!
* @author Andrew G. Crowell
*/
public class Slime extends Monster
{
	public Slime(GameWindow gameWindow, DungeonWindow dungeonWindow, Room room, int x, int y)
	{
		super(gameWindow, dungeonWindow, room, x, y, new ImageIcon("slime.png"), "sentient ooze", new Dissolve());
	}
}
