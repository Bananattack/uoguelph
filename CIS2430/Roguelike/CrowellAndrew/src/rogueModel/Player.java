package rogueModel;

import rogueView.*;

import java.awt.event.*;

import javax.swing.*;

/**
 * Our hero! This is a special entity that receives user input.
 * @author Andrew Crowell
 */
public class Player extends Creature
{	
	private static final long serialVersionUID = 1L;
	private static final int RECOVERY_STEPS = 25;
	private boolean inWearItemMenu;
	private boolean inUseItemMenu;
	private boolean inSaveMenu;
	private boolean inLoadMenu;
	private int steps = 0;
	private Inventory inventory;

	/**
	 * Create a new player in the specified window, in the specified dungeon,
	 * in the specified room, at the specified coordinates.
	 * 
	 * @param gameWindow - the window to spawn in
	 * @param dungeonWindow - the dungeon to spawn in
	 * @param room - the room to spawn in
	 * @param x - horizontal position
	 * @param y - vertical position
	 */
	public Player(GameWindow gameWindow, DungeonWindow dungeonWindow, Room room, int x, int y)
	{
		super(gameWindow, dungeonWindow, room, x, y, new ImageIcon("hero.png"), "our hero", new Stab());
		this.setStats(1, 10, 3, 1, 75, 30);
		inventory = new Inventory();
	}
	
	
	public boolean stayOffEntities()
	{
		return false;
	}
	
	/**
	 * Attempts to moves the player in the specified direction.
	 * Heals the player if he moves a certain amount of steps.
	 * 
	 * @param direction direction to move towards.
	 * @return whether or not the move occurred.
	 */
	public boolean move(MapView.Direction direction)
	{
		boolean moved = super.move(direction);
		if(moved)
		{
			steps++;
			if (steps % RECOVERY_STEPS == 0)
			{
				if (getHP() < getMaxHP())
				{
					heal(1);
				}
			}
		}
		return moved;
	}
	
	/**
	 * Parses the supplied keyboard input and acts on it.
	 * All game logic is updated directly after the each successful movement.
	 * 
	 * @param scancode - the KeyEvent scancode to examine.
	 */
	public void parseInput(int scancode)
	{	
		if(isDead())
		{
			return;
		}
		if(inLoadMenu || inSaveMenu)
		{
			int index = -1;
			switch(scancode)
			{
				case KeyEvent.VK_1: index = 1; break;
				case KeyEvent.VK_2: index = 2; break;
				case KeyEvent.VK_3: index = 3; break;
				case KeyEvent.VK_4: index = 4; break;
				case KeyEvent.VK_5: index = 5; break;
				case KeyEvent.VK_6: index = 6; break;
				case KeyEvent.VK_7: index = 7; break;
				case KeyEvent.VK_8: index = 8; break;
				case KeyEvent.VK_9: index = 9; break;
				case KeyEvent.VK_0: index = 0; break;
			}
			if(index >= 0)
			{
				if(inLoadMenu)
				{
					getGameWindow().loadGame(GameWindow.saveFile + index);
				}
				else
				{
					getGameWindow().saveGame(GameWindow.saveFile + index);
				}
			}
			else
			{
				getGameWindow().setMessage("Invalid save index.");
			}
			inLoadMenu = inSaveMenu = false;
			return;
		}
		if(inWearItemMenu || inUseItemMenu)
		{
			int itemIndex = -1;
			switch(scancode)
			{
				case KeyEvent.VK_1: itemIndex = 0; break;
				case KeyEvent.VK_2: itemIndex = 1; break;
				case KeyEvent.VK_3: itemIndex = 2; break;
				case KeyEvent.VK_4: itemIndex = 3; break;
				case KeyEvent.VK_5: itemIndex = 4; break;
				case KeyEvent.VK_6: itemIndex = 5; break;
				case KeyEvent.VK_7: itemIndex = 6; break;
				case KeyEvent.VK_8: itemIndex = 7; break;
				case KeyEvent.VK_9: itemIndex = 8; break;
				case KeyEvent.VK_0: itemIndex = 9; break;
			}
			if(itemIndex >= 0 && itemIndex < inventory.getSize())
			{
				Item item = inventory.getItem(itemIndex);
				if(inUseItemMenu)
				{
					item.use(this);
				}
				else if(inWearItemMenu)
				{
					item.wear(this);
				}
				inventory.removeItem(item);
			}
			else
			{
				getGameWindow().setMessage("Invalid item.");
			}
			inWearItemMenu = inUseItemMenu = false;
			return;
		}
		
		getDungeonWindow().setInventory(inventory.getListing());
		MapView.Direction direction = null;
		switch(scancode)
		{
				case KeyEvent.VK_L:
					getGameWindow().setMessage("Load which file? (0 - 9)");
					inLoadMenu = true;
					return;
				case KeyEvent.VK_S:
					getGameWindow().setMessage("Save which file? (0 - 9)");
					inSaveMenu = true;
					return;
				case KeyEvent.VK_U:
					if (inventory.getSize() > 0)
					{
						inUseItemMenu = true;
						getGameWindow().setMessage("Use which item? (1 - " + (inventory.getSize() % 10) + ")");
					}
					else
					{
						getGameWindow().setMessage("You have no items!");
					}
					getDungeonWindow().setInventory(inventory.getListing());
					return;
				case KeyEvent.VK_W:
					if (inventory.getSize() > 0)
					{
						inWearItemMenu = true;
						getGameWindow().setMessage("Wear which item? (1 - " + (inventory.getSize() % 10) + ")");
					}
					else
					{
						getGameWindow().setMessage("You have no items!");
					}
					getDungeonWindow().setInventory(inventory.getListing());
					return;
				case KeyEvent.VK_UP:
					direction = MapView.Direction.NORTH;
					break;
				case KeyEvent.VK_DOWN:
					direction = MapView.Direction.SOUTH;
					break;
				case KeyEvent.VK_LEFT:
					direction = MapView.Direction.WEST;
					break;
				case KeyEvent.VK_RIGHT:
					direction = MapView.Direction.EAST;
					break;
				default:
					// Can't handle the input, just return.
					return;
		}
		// Check if there's something beside you.
		Entity target = findAdjacentCreature(direction);
		
		if(target instanceof Teleporter)
		{
			target.activate();
		}
		else if(target instanceof Item)
		{
			try
			{
				inventory.addItem((Item)target);
				target.destroy();
				getGameWindow().setMessage(getCapitalizedName() + " picks the " + target.getName() + " off the ground.");
			}
			catch(Inventory.InventoryFullException e)
			{
				getGameWindow().setMessage("Inventory is full");
			}
			getDungeonWindow().setInventory(inventory.getListing());
			getGameWindow().update();
			return;
		}
		
		// Try to move, and branch if the move fails.
		if(!move(direction))
		{	
			// There was something beside you, probably was an obstruction
			if(target != null)
			{
				// If we're beside a hostile, attack it. 
				if(target instanceof Creature)
				{
					attack((Creature) target);
				}
				// Otherwise, say we bumped into it.
				else
				{
					getGameWindow().setMessage(getCapitalizedName() + " bumps into the " + target.getName() + ".");
				}
			}
			// Nothing there, must be walking into a wall
			else
			{
				getGameWindow().setMessage(getCapitalizedName() + " is blocked by a " + direction.name().toLowerCase() + " wall.");
			}
		}
		
		// Center camera on player
		try
		{
			getDungeonWindow().getMap().focusElement(this);
		}
		catch(MapNotSetException e)
		{
		}
		catch(NoSuchElementException e)
		{
		}
		// After player moves, make the game window update everyone else.
		getGameWindow().update();
		
		getDungeonWindow().setInventory(inventory.getListing());
	}
	
	public Inventory getInventory()
	{
		return inventory;
	}
	
	public void setInventory(Inventory inventory)
	{
		this.inventory = inventory;
	}
}
