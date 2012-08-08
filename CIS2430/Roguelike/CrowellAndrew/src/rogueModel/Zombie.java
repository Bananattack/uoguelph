package rogueModel;

import javax.swing.ImageIcon;

import rogueView.DungeonWindow;

/**
* Mentally limited undead creatures that punch you
* @author Andrew G. Crowell
*/
public class Zombie extends Monster
{
	public Zombie(GameWindow gameWindow, DungeonWindow dungeonWindow, Room room, int x, int y)
	{
		super(gameWindow, dungeonWindow, room, x, y, new ImageIcon("hero2.png"), "zombie", new Punch());
	}
}
