package rogueModel;

import javax.swing.ImageIcon;

import rogueView.DungeonWindow;

/**
* Durable bamboo protection
* @author Andrew G. Crowell
*/
public class Armor extends Item
{
	public Armor(GameWindow gameWindow, DungeonWindow dungeonWindow, Room room, int x, int y)
	{
		super(gameWindow, dungeonWindow, room, x, y, new ImageIcon("armor.png"), "itchy bamboo armor");
	}
	
	public void use(Creature user)
	{
		getGameWindow().setMessage("You blow your nose on the " + getName());
		getGameWindow().setMessage("Bamboo is not an ideal nose-blowing surface. Your nose begins to bleed.");
		getGameWindow().setMessage("You lose 5 HP.");
		user.hurt(5);
	}
	
	public void wear(Creature user)
	{
		getGameWindow().setMessage("You equip the " + getName() + ". It's very uncomfortable, but you feel protected wearing it.");
		getGameWindow().setMessage("You gain 3 armor and 15 evasion");
		user.alterStats(0, 0, 3, 0, 15);
	}
}
