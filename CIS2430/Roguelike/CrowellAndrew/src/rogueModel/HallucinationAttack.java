package rogueModel;

/**
 * A pointless attack, for hallunications
 * @author Andrew G. Crowell 
 */
public class HallucinationAttack implements CombatStrategy
{
	/**
	 * Engages two creatures in combat, one attacking, one defending
	 * @param attacker the person performing the attack
	 * @param attackee the person defending against the attack
	 */
	public void attack(Creature attacker, Creature attackee)
	{
		attacker.getGameWindow().setMessage(attacker.getCapitalizedName() + " plays tricks on " + attackee.getName());
	}
}