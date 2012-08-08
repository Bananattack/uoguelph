package rogueModel;

/**
 * A stab attack, has some chance of doing critical (extra) damage 
 * @author Andrew G. Crowell 
 */
public class Stab implements CombatStrategy
{
	/**
	 * Engages two creatures in combat, one attacking, one defending
	 * @param attacker the person performing the attack
	 * @param attackee the person defending against the attack
	 */
	public void attack(Creature attacker, Creature attackee)
	{
		int attack = Helper.Random(attacker.getAttack() / 2, attacker.getAttack() * 2);
		int damage = attacker.getDamage(attack, attackee);
		 
		attacker.getGameWindow().setMessage(attacker.getCapitalizedName() + " stabs " + attackee.getName() + " with his mighty short sword for " + damage + " damage.");
		attackee.hurt(damage);
		
	}
}