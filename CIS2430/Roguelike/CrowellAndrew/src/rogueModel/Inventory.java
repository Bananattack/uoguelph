package rogueModel;

import java.util.*;

/**
 * The inventory storage
 * @author Andrew G. Crowell
 */
public class Inventory
{
	public static final int ITEM_STRUDEL = 0;
	public static final int ITEM_ARMOR = 1;
	public static final int ITEM_POTION = 2;
	public static class InventoryFullException extends Exception
	{
		public InventoryFullException()
		{
			super("Inventory is full.");
		}
		
		public InventoryFullException(String message)
		{
			super(message);
		}
	}
	private ArrayList<Item> items;
	/**
	 * Creates a new inventory
	 */
	public Inventory()
	{
		items = new ArrayList<Item>();
	}

	/**
	 * Creates a new inventory with preset inventory items
	 */	
	public Inventory(GameWindow gameWindow, ArrayList<Integer> indices)
	{
		this();
		for(Integer index : indices)
		{
			switch(index)
			{
				case ITEM_STRUDEL:
					items.add(new MagicalToasterStrudel(gameWindow, null, null, -1, -1));
					break;
				case ITEM_ARMOR:
					items.add(new Armor(gameWindow, null, null, -1, -1));
					break;
				case ITEM_POTION:
					items.add(new Potion(gameWindow, null, null, -1, -1));
					break;
			}
		}
	}
	
	/**
	 * Converts the inventory into a list of item indices
	 * @return the list of indices
	 */	
	public ArrayList<Integer> toIndices()
	{
		ArrayList<Integer> indices = new ArrayList<Integer>();
		for(Item item : items)
		{
			if(item instanceof Potion)
			{
				indices.add(ITEM_POTION);	
			}
			else if(item instanceof Armor)
			{
				indices.add(ITEM_ARMOR);	
			}
			else if(item instanceof MagicalToasterStrudel)
			{
				indices.add(ITEM_STRUDEL);	
			}	
		}
		return indices;
	}
	
	/**
	 * Adds an item
	 * @param item the item to add
	 */	
	public void addItem(Item item) throws InventoryFullException
	{
		if(items.size() < 10)
		{
			items.add(item);
			return;
		}
		throw new InventoryFullException();
	}
	
	/**
	 * Removes an item
	 * @param item the item to remove
	 */	
	public void removeItem(Item item)
	{
		items.remove(item);
	}
	
	/**
	 * Gets an item
	 * @param index the item index to get
	 * @return the item
	 */	
	public Item getItem(int index)
	{
		return items.get(index);
	}
	
	/**
	 * Gets the inventory size
	 * @return the size
	 */	
	public int getSize()
	{
		return items.size();
	}
	
	/**
	 * Gets a useful string representation of the inventory
	 * @return the representation
	 */	
	public String getListing()
	{
		String s = "";
		for(int i = 0; i < items.size(); i++)
		{
			Item item = items.get(i);
			s += ((i + 1) % 10) + ") " + item.getName() + "\n";
		}
		return s;
	}

	/**
	 * Gets a useful string representation of the inventory
	 * @return the representation
	 */	
	public String toString()
	{
		return getListing();
	}
}
