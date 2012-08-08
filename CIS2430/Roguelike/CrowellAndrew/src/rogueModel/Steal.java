package rogueModel;

/**
 * A theft attack, which takes an item 
 * @author Andrew G. Crowell 
 */
public class Steal implements CombatStrategy
{
	/**
	 * Engages two creatures in combat, one attacking, one defending
	 * @param attacker the person performing the attack
	 * @param attackee the person defending against the attack
	 */
	public void attack(Creature attacker, Creature attackee)
	{
		if(attackee instanceof Player)
		{
			Player player = (Player) attackee;
			Inventory inventory = player.getInventory();
			if(inventory.getSize() > 0)
			{
				Item item = inventory.getItem(Helper.Random(0, inventory.getSize() - 1));
				inventory.removeItem(item);
				attacker.getGameWindow().setMessage(attacker.getCapitalizedName() + " steals " + item.getName() + " from " + player.getName() + "!");
				return;
			}
		}
		attacker.getGameWindow().setMessage(attacker.getCapitalizedName() + " glares at " + attackee.getName() + " with sneaky intent.");
	}
}