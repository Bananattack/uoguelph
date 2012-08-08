package rogueView;

import rogueModel.*;

import java.awt.*;
import javax.swing.*;

import java.util.*;

/**
* An panel that contains the map
* @author Andrew G. Crowell
*/
public class MapView extends JPanel
{
	/**
	* An enumeration of directions.
	*/
	public enum Direction { NORTH, SOUTH, EAST, WEST };
	
	private HashMap<DungeonArea, AreaView> areaTable;
	private HashMap<StoryElement, Tile> elementLocations;
	private JScrollPane scrollPane;
	
	/**
	* Creates a new MapView
	* @param rooms the list of rooms
	* @param hallways the list of halls
	*/
	public MapView(ArrayList<Room> rooms, ArrayList<Hallway> hallways) throws BadMapException, NoDoorThereException
	{
		setLayout(new BorderLayout());
		TileGrid grid = new TileGrid(35, 35);
		scrollPane = new JScrollPane(grid);
        scrollPane.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_ALWAYS);
        scrollPane.setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_ALWAYS);
        scrollPane.setMinimumSize(new Dimension(480, 240));
        scrollPane.setPreferredSize(new Dimension(640, 480));
        add(scrollPane, BorderLayout.CENTER);
		this.elementLocations = new HashMap<StoryElement, Tile>();
		this.areaTable = new HashMap<DungeonArea, AreaView>();
		
		if(rooms.size() > 9)
		{
			throw new BadMapException();
		}

		int i = 0;
		for(Room room : rooms)
		{
			areaTable.put(room, new RoomView(room, grid, i));
			i++;
		}
		for(Hallway hallway : hallways)
		{
			areaTable.put(hallway, new HallwayView(hallway, this, grid));
		}
	}
	
	/**
	* Sets the start location of an element and adds it to the map
	* @param storyElement the element
	* @param startRoom the room
	* @param x the horizontal position
	* @param y the vertical position
	*/
	public void setElementStartPoint(StoryElement storyElement, Room startRoom, int x, int y) throws OutOfRangeException
	{
		RoomView roomView = (RoomView) resolveAreaView(startRoom);
		elementLocations.put(storyElement, roomView.getTile(x, y));
		roomView.getTile(x, y).setIcon(storyElement.getImageIcon());
	}
	
	/**
	* Gets the dungeon area of an element
	* @param storyElement the element
	* @return the dungeon area
	*/
	public DungeonArea getElementArea(StoryElement storyElement) throws NoSuchElementException
	{
		if(!elementLocations.containsKey(storyElement))
		{
			throw new NoSuchElementException();
		}
		else
		{
			return elementLocations.get(storyElement).getAreaView().getDungeonArea();
		}
	}

	/**
	* Moves an element in the given direction
	* @param storyElement the element
	* @param direction the direction to move toward
	*/
	public void moveStoryElement(StoryElement storyElement, Direction direction) throws NoSuchElementException, AdjacentSquareException
	{
		if(!elementLocations.containsKey(storyElement))
		{
			throw new NoSuchElementException();
		}
		try
		{
			Tile adj = elementLocations.get(storyElement).getAdjacent(direction);
			if(elementLocations.get(storyElement).getAdjacent(direction).isObstructed())
			{
				return;
			}
			adj.addIcon(storyElement.getImageIcon());
			elementLocations.get(storyElement).resetIcon();
			elementLocations.put(storyElement, adj);
		}
		catch(OutOfRangeException e)
		{
			throw new AdjacentSquareException();
		}
	}
	
	/**
	* Removes an element from the map
	* @param storyElement the element to remove
	*/
	public void removeElement(StoryElement storyElement) throws NoSuchElementException
	{
		if(!elementLocations.containsKey(storyElement))
		{
			throw new NoSuchElementException();
		}
		Tile tile = elementLocations.remove(storyElement);
		tile.removeIcon(storyElement.getImageIcon());
	}
	
	/**
	* Gets the contents of a tile adjacent to an element in the given direction
	* @param storyElement the element
	* @param direction the direction to inspect
	* @return the tile adjacent to the element given
	*/
	public Tile getContentsOfAdjacentSquare(StoryElement storyElement, Direction direction) throws NoSuchElementException, AdjacentSquareException
	{
		if(!elementLocations.containsKey(storyElement))
		{
			throw new NoSuchElementException();
		}
		try
		{
			return elementLocations.get(storyElement).getAdjacent(direction);
		}
		catch(OutOfRangeException e)
		{
			throw new AdjacentSquareException();
		}
	}
	
	/**
	* Gets the element of a tile adjacent to an element in the given direction
	* @param storyElement the element
	* @param direction the direction to inspect
	* @return the element adjacent to the element given
	*/
	public StoryElement getElementInAdjacentSquare(StoryElement storyElement, Direction direction) throws NoSuchElementException, AdjacentSquareException
	{
		if(!elementLocations.containsKey(storyElement))
		{
			throw new NoSuchElementException();
		}
		try
		{
			Tile adj = elementLocations.get(storyElement).getAdjacent(direction);
			
			Iterator<StoryElement> it = elementLocations.keySet().iterator();
			while(it.hasNext())
			{
				StoryElement elem = it.next();
				if(elementLocations.get(elem) == adj)
				{
					return elem;
				}
			}
		}
		catch(OutOfRangeException e)
		{
			throw new AdjacentSquareException();
		}
		return null;
	}
	
	/**
	* Gets the area view for dungeon area
	* @param dungeonArea the area
	* @return the area view
	*/
	public AreaView resolveAreaView(DungeonArea dungeonArea)
	{
		return areaTable.get(dungeonArea);
	}
	
	/**
	* Ensures the element is visible in the viewport of the map
	* @param element the element
	*/
	public void focusElement(StoryElement storyElement) throws NoSuchElementException
	{
		if(!elementLocations.containsKey(storyElement))
		{
			throw new NoSuchElementException();
		}
		Tile tile = elementLocations.get(storyElement);
		Rectangle region = scrollPane.getViewport().getViewRect();
		if(Math.max(tile.getX() - 128, 0) < region.x || tile.getX() > region.x + region.width - 128)
		{
			scrollPane.getViewport().setViewPosition(new Point(Math.max(tile.getX() - 128, 0), region.y));	
		}
		if(Math.max(tile.getY() - 128, 0) < region.y || tile.getY() > region.y + region.height - 128)
		{
			scrollPane.getViewport().setViewPosition(new Point(region.x, Math.max(tile.getY() - 128, 0)));	
		}
	}
}
