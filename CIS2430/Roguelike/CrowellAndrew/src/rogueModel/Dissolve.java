package rogueModel;

/**
 * A dissolve attack, which steals health from the attack and gives some of it to the attacker 
 * @author Andrew G. Crowell 
 */
public class Dissolve implements CombatStrategy
{
	/**
	 * Engages two creatures in combat, one attacking, one defending
	 * @param attacker the person performing the attack
	 * @param attackee the person defending against the attack
	 */
	public void attack(Creature attacker, Creature attackee)
	{
		int attack = Helper.Random(attacker.getAttack() / 2, attacker.getAttack());
		int damage = attacker.getDamage(attack, attackee);
		int absorption = damage / 2;
		 
		attacker.getGameWindow().setMessage(attacker.getCapitalizedName() + " dissolves " + attackee.getName() + " flesh for " + damage + " damage.");
		attacker.getGameWindow().setMessage(attacker.getCapitalizedName() + " absorbs " + absorption + " HP.");
		attackee.hurt(damage);
		
	}
}