package rogueModel;

/**
 * A bare-handed punch attack, that can hit multiple times.
 * @author Andrew G. Crowell 
 */
public class Punch implements CombatStrategy
{
	/**
	 * Engages two creatures in combat, one attacking, one defending
	 * @param attacker the person performing the attack
	 * @param attackee the person defending against the attack
	 */
	public void attack(Creature attacker, Creature attackee)
	{
		int attack;
		int repeat = Helper.Random(1, 9);
		int damage = 0;
		for(int i = 0; i < repeat; i++)
		{
			attack = Helper.Random(attacker.getAttack() / 2, attacker.getAttack() * 2); 
			damage += attacker.getDamage(attack, attackee) / ((i * 3) + 1);
		}
		 
		attacker.getGameWindow().setMessage(attacker.getCapitalizedName() + " punches " + attackee.getName() + " " + repeat + " times for " + damage + " damage.");
		attackee.hurt(damage);
		
	}
}