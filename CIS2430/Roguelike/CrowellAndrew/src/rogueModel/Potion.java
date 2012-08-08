package rogueModel;

import javax.swing.ImageIcon;

import rogueView.DungeonWindow;

/**
* A healing potion
* @author Andrew G. Crowell
*/
public class Potion extends Item
{
	public Potion(GameWindow gameWindow, DungeonWindow dungeonWindow, Room room, int x, int y)
	{
		super(gameWindow, dungeonWindow, room, x, y, new ImageIcon("potion.png"), "chunky vanilla Listerine");
	}
	
	public void use(Creature user)
	{
		getGameWindow().setMessage("You drink the " + getName() + ". You almost vomit from the burning mouthwash sensation.");
		getGameWindow().setMessage("However, your breath is now chunky fresh! You restore 20 hp!");
		user.heal(20);
	}
	
	public void wear(Creature user)
	{
		getGameWindow().setMessage("You dump the " + getName() + " in your hair. Ick.");
		getGameWindow().setMessage("Your hair solidifies and sharpens, becoming a vicious and destructive stabbing weapon.");
		getGameWindow().setMessage("Your attack increases by 10.");
		user.alterStats(0, 10, 0, 0, 0);
	}
}
