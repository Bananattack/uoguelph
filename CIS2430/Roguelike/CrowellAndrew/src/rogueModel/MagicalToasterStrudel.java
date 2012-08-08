package rogueModel;

import javax.swing.ImageIcon;

import rogueView.*;

/**
* A toaster strudel with "magical" properties.
*/
public class MagicalToasterStrudel extends Item
{
	public MagicalToasterStrudel(GameWindow gameWindow, DungeonWindow dungeonWindow, Room room, int x, int y)
	{
		super(gameWindow, dungeonWindow, room, x, y, new ImageIcon("tart.png"), "magical toaster strudel");
	}
	
	public void use(Creature user)
	{
		getGameWindow().setMessage("You eat the magical toaster strudel.");
		getGameWindow().hallucinate();
	}
	
	public void wear(Creature user)
	{
		getGameWindow().setMessage("You smear the toaster strudel on your chest and it disintegrates into a fruity mass of filth.");
		getGameWindow().setMessage("You gain 5 accuracy.");
		user.alterStats(0, 0, 0, 5, 0);
	}
}
