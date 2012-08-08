package rogueModel;

import rogueView.*;
import javax.swing.*;

/**
 * An entity that can engage in combat, and has stats associated with the entity's
 * well-being and prowess in battle.
 * @author Andrew G. Crowell
 */
public abstract class Creature extends Entity
{	
	private static final int BASE_EXPERIENCE = 20;
	
	private int level;
	private int exp, nextExp;
	private int hp, maxHP;
	private int attack;
	private int defense;
	private int accuracy;
	private int evasion;
	private CombatStrategy combatStrategy;
	
	/**
	 * Create a new creature in the specified window, in the specified dungeon,
	 * in the specified room, at the specified coordinates, with the specified name.
	 * 
	 * @param gameWindow - the window to spawn in
	 * @param dungeonWindow - the dungeon to spawn in
	 * @param room - the room to spawn in
	 * @param x - horizontal position
	 * @param y - vertical position
	 * @param name - what the element is called
	 * @param combatStrategy the combat strategy
	 */
	public Creature(GameWindow gameWindow, DungeonWindow dungeonWindow, Room room, int x, int y, ImageIcon imageIcon, String name, CombatStrategy combatStrategy)
	{
		super(gameWindow, dungeonWindow, room, x, y, imageIcon, name);
		this.combatStrategy = combatStrategy;
	}
	
	public boolean isObstruction()
	{
		return true;
	}
	
	/**
	 * Sets all initial values for the creature's stats.
	 * 
	 * @param level - the current level of the creature
	 * @param hp - base hit points (at level 1), all growth is automatically filled in
	 * @param attack - base attack (at level 1), all growth is automatically filled in
	 * @param defense - base defense (at level 1), all growth is automatically filled in
	 * @param accuracy - base accuracy (at level 1), all growth is automatically filled in
	 * @param evasion - base evasion (at level 1), all growth is automatically filled in
	 */
	public void setStats(int level, int hp, int attack, int defense, int accuracy, int evasion)
	{
		this.level = level;
		// Next experience is a geometric series that doubles.
		this.nextExp = (int) (BASE_EXPERIENCE*Math.pow(2, level - 1));
		if(level > 1)
		{
			this.exp = (int) (BASE_EXPERIENCE*Math.pow(2, level - 2));
			for(int i = 0; i < level; i++)
			{
				applyLevelingGrowth();
			}
		}
		
		this.hp = this.maxHP = hp;
		this.attack = attack;
		this.defense = defense;
		this.accuracy = accuracy;
		this.evasion = evasion;
	}
	
	/**
	 * Offsets all values for the creature's stats, by some stat modifiers.
	 * 
	 * @param level - the current level of the creature
	 * @param hp - base hit points (at level 1), all growth is automatically filled in
	 * @param attack - base attack (at level 1), all growth is automatically filled in
	 * @param defense - base defense (at level 1), all growth is automatically filled in
	 * @param accuracy - base accuracy (at level 1), all growth is automatically filled in
	 * @param evasion - base evasion (at level 1), all growth is automatically filled in
	 */
	public void alterStats(int hp, int attack, int defense, int accuracy, int evasion)
	{
		this.hp += hp;
		this.maxHP += hp;
		this.attack += attack;
		this.defense += defense;
		this.accuracy += accuracy;
		this.evasion += evasion;
	}
	
	/**
	 * Returns the creature's hit points.
	 * @return hp
	 */ 
	public int getHP()
	{
		return hp;
	}

	/**
	 * Sets player's hp to a new value
	 * @param hp - the value to assign
	 */ 
	public void setHP(int hp)
	{
		this.hp = hp;
	}
	
	/**
	 * Returns the creature's maximum hit points.
	 * @return max hp
	 */ 
	public int getMaxHP()
	{
		return maxHP;
	}
	
	/**
	 * Returns the creature's attack.
	 * @return attack
	 */ 
	public int getAttack()
	{
		return attack;
	}
	
	/**
	 * Returns the creature's attack.
	 * @param attack new attack
	 */ 
	public void setAttack(int attack)
	{
		this.attack = attack;
	}
	
	/**
	 * Returns the creature's defense.
	 * @return defense
	 */ 
	public int getDefense()
	{
		return defense;
	}
	
	/**
	 * Returns the creature's accuracy.
	 * @return accuracy
	 */ 
	public int getAccuracy()
	{
		return accuracy;
	}
	
	/**
	 * Returns the creature's evasion.
	 * @return evasion
	 */ 
	public int getEvasion()
	{
		return evasion;
	}
	
	/**
	 * Returns the creature's experience level.
	 * @return level
	 */ 
	public int getLevel()
	{
		return level;
	}

	/**
	 * Returns the creature's experience points.
	 * @return exp
	 */ 
	public int getExp()
	{
		return exp;
	}
	
	/**
	 * Changes the creature's experience points.
	 * @param exp the new experience points
	 */ 
	public void setExp(int exp)
	{
		this.exp = exp;
	}
	
	/**
	 * Returns the experience points needed for the creature to gain a level
	 * @return exp before next level up
	 */ 
	public int getExpToNext()
	{
		return nextExp - exp;
	}
	
	/**
	 * Deals damage in HP to the creature, provided the damage is > 0.
	 * @param damage points
	 */ 
	public void hurt(int damage)
	{
		if(damage <= 0)
		{
			return;
		}
		
		hp = Math.max(hp - damage, 0);
		if(hp == 0)
		{
			destroy();
		}
	}
	
	/**
	 * Restores lost HP to the creature, provided the amount is > 0.
	 * @param recovery amount
	 */ 
	public void heal(int amount)
	{
		if (amount <= 0)
		{
			return;
		}
	
		hp = Math.min(hp + amount, maxHP);
	}
	
	/**
	 * Internal method for dealing with level ups.
	 * Adjusts the creature's stats to reflect a level gained.
	 */ 
	private void applyLevelingGrowth()
	{
		int change;
		
		change = Helper.Random(3, 5);
		hp += change;
		maxHP += change;
		
		attack += Helper.Random(3, 5);
		defense += Helper.Random(1, 2);
		accuracy += Helper.Random(1, 2);
		evasion += Helper.Random(1, 2);
		
	}
	
	public boolean isGoingToHit(Creature target)
	{
		int chance = Helper.Random(0, 100);
	
		// Can't attack if you're dead.
		if(isDead())
		{
			return false;
		}
		if(chance > accuracy - target.getEvasion() && target.getEvasion() > 0)
		{
			return false;
		}
		return true;
	}
	
	public int getDamage(int attack, Creature target)
	{
		int defense = Helper.Random(target.defense / 2, target.defense * 2);
		// Restrict damage to >= 0.
		return Math.max(attack - defense, 0);
	}
	
	/**
	 * This combatant attempts to perform attack on a target creature.
	 * Outputs messages in the game window according to the result of the action.
	 * 
	 */ 
	public void attack(Creature target)
	{
		String message = "";
		// Can't attack if you're dead.
		if(isDead())
		{
			return;
		}
		
		// Miss!
		if(!isGoingToHit(target))
		{
			int m = Helper.Random(0, 3);
			switch(m)
			{
				case 0:
					message = getCapitalizedName() + " misses " + target.getName() + ".";
					break;
				case 1:
					message = target.getCapitalizedName() + " avoids " + getName() + "'s attack.";
					break;
				case 2:
					message = getCapitalizedName() + " was too slow to hit " + target.getName() + ".";
					break;
				case 3:
					message = getCapitalizedName() + " completely misses " + target.getName() + ", and becomes disoriented for a moment.";
					break;
			}
			getGameWindow().setMessage(message);
			return;
		}
		// Hit!
		else
		{	
			combatStrategy.attack(this, target);
			
			// Killed the target!
			if(target.getHP() == 0)
			{
				getGameWindow().setMessage(target.getCapitalizedName() + " is dead!");
				
				// Gain experience.
				exp += target.level * 5;
				
				boolean leveledUp = false;
				// If the combatant gains enough experience, level up.
				// Continue until the leveling requirement is greater than the current exp.
				while(exp >= nextExp)
				{
					level++;
					applyLevelingGrowth();
					nextExp = (int) (BASE_EXPERIENCE*Math.pow(2, level - 1));
					
					leveledUp = true;
				}
				// Announce if there was actually any leveling.
				if(leveledUp)
				{
					getGameWindow().setMessage(getCapitalizedName() + " has grown to level " + level + "!");
				}
			}
		}
	}
}
