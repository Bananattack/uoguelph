package rogueModel;

/**
 * An interface that defines the behaviour for an specific attack
 * @author Andrew G. Crowell 
 */
public interface CombatStrategy
{
	/**
	 * Engages two creatures in combat, one attacking, one defending
	 * @param attacker the person performing the attack
	 * @param attackee the person defending against the attack
	 */
	public void attack(Creature attacker, Creature attackee);
}