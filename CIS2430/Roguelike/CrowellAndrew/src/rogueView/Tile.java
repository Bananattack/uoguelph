package rogueView;

import javax.swing.*;
import java.util.*;

/**
* A tile on the map
* @author Andrew G. Crowell
*/
public class Tile extends JLabel
{
	private int x, y;
	private ImageIcon defaultIcon;
	private AreaView areaView;
	private boolean obstructed;
	private ArrayList<ImageIcon> activeIcons;
	private HashMap<MapView.Direction, Tile> adjacent;
	
	/**
	* Creates a tile with an associated icon
	* @param icon the icon
	*/
	public Tile(ImageIcon icon)
	{
		setDefaultIcon(icon);
		obstructed = true;
		activeIcons = new ArrayList<ImageIcon>();
		this.adjacent = new HashMap<MapView.Direction, Tile>();
	}

	/**
	* Gets the tile x of the tile
	* @return tile x
	*/	
	public int getTileX()
	{
		return x;
	}

	/**
	* Gets the tile y of the tile
	* @return tile y
	*/		
	public int getTileY()
	{
		return y;
	}
	
	/**
	* Gets whether a certain tile is obstructed
	* @return obstructed
	*/	
	public boolean isObstructed()
	{
		return obstructed;
	}

	/**
	* Sets whether a certain tile is obstructed
	* @param obstructed whether a certain tile is obstructed
	*/		
	public void setObstructed(boolean obstructed)
	{
		this.obstructed = obstructed;
	}

	/**
	* Sets tile (x, y)
	* @param x the horizontal position
	* @param y the vertical position
	*/	
	public void setTileLocation(int x, int y)
	{
		this.x = x;
		this.y = y;
	}
	
	/**
	* Gets tile area view
	* @return area view
	*/	
	public AreaView getAreaView()
	{
		return areaView;
	}
	
	/**
	* Sets tile area view
	* @param area view
	*/
	public void setAreaView(AreaView areaView)
	{
		this.areaView = areaView;
	}
	
	/**
	* Gets the tile's adjacent tile in a direction
	* @param direction the direction
	* @return adjacent tile
	*/
	Tile getAdjacent(MapView.Direction direction) throws OutOfRangeException
	{
		if(adjacent.containsKey(direction))
		{
			return adjacent.get(direction);
		}
		throw new OutOfRangeException();
	}

	/**
	* Gets the tile's adjacent tile in a direction
	* @param direction the direction
	* @param tile thee adjacent tile
	*/	
	void setAdjacent(MapView.Direction direction, Tile tile)
	{
		adjacent.put(direction, tile);
	}

	/**
	* Resets the icon to its previous icon.
	*/		
	public void resetIcon()
	{
		if(activeIcons.size() == 0)
		{
			setIcon(defaultIcon);
		}
		else
		{
			setIcon(activeIcons.remove(activeIcons.size() - 1));
		}
	}
	
	/**
	* Removes an icon stored at this tile from the tile
	* @param icon the icon
	*/		
	public void removeIcon(ImageIcon icon)
	{
		if(activeIcons.contains(icon))
		{
			activeIcons.remove(activeIcons.indexOf(icon));
		}
		if(activeIcons.size() == 0)
		{
			setIcon(defaultIcon);
		}
		else
		{
			setIcon(activeIcons.get(activeIcons.size() - 1));
		}
	}
	
	/**
	* Adds an icon to the tile, and puts the added icon to the top of the display
	* @param icon the icon
	*/		
	public void addIcon(ImageIcon icon)
	{
		ImageIcon lastIcon = (ImageIcon) getIcon();
		setIcon(icon);
		activeIcons.add(lastIcon);
	}
	
	/**
	* Sets the default icon for the tile
	* @param icon the icon
	*/		
	public void setDefaultIcon(ImageIcon icon)
	{
		defaultIcon = icon;
		setIcon(icon);
	}
}
