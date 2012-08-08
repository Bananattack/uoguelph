package rogueModel;

import java.util.*;
import rogueView.*;

/**
 * An rectangular area within a dungeon which connected to others by hallways.
 *   
 * @author Andrew Crowell 
 */
public class Room extends DungeonArea
{
	/**
	 * An exception raised when you try and make two doors on the same wall.
	 */
	public static class DoorOccupiedException extends Exception
	{
		public DoorOccupiedException()
		{
			this("Attempted to add a doorway to a room in a direction that is already occupied by another doorway!");
		}
		
		public DoorOccupiedException(String s)
		{
			super(s);
		}
	}

	private ArrayList<MapView.Direction> doors;
	private int length, width;
	
	/**
	 * Instantiate a room with a pair of rectangular dimensions.
	 * 
	 * @param length - the vertical size, in rows, of this room
	 * @param width - the horizontal size, in columns, of this room
	 * 
	 */
	public Room(int length, int width)
	{
		this.length = length;
		this.width = width;
		this.doors = new ArrayList<MapView.Direction>();
	}
	
    /**
     * This method must return an array containing directions that indicate
     * which walls the room has doors in.  For example, if the room has a door going east and
     * one going west the array should contain EAST and WEST.  Other elements of the array should be empty or null
     * 
     * @return an array of each door
     */
    public MapView.Direction[] getDoorDirections()
    {
        MapView.Direction[] d = new MapView.Direction[4];
        doors.toArray(d);
        return d;
    }
    
    /**
     * Adds a new doorway to a Room, provided that a specific direction is not taken already.
     * @return the room as a string
     * @throws Room.DoorOccupiedException when the direction specified is already occupied by another door
     */
    public void addDoor(MapView.Direction direction) throws Room.DoorOccupiedException
    {
    	if(!doors.contains(direction))
    	{
    		doors.add(direction);
    	}
    	else
    	{
    		throw new Room.DoorOccupiedException();
    	}
    }
    
   /**
    * Returns the length of the Room
    * @return the length of the Room in rows
    */
    public int getLength()
    {
        return length;
    }
    
    /**
     * Returns the width of the Room
     * @return the width of the Room in columns
     */
    public int getWidth()
    {
        return width;
    }
    
    /**
     * A useful string representation of a Room.
     * @return the room as a String
     */
    public String toString()
    {
    	String s = "";
    	boolean separator = false;
    	for(MapView.Direction door: doors)
    	{
    		if (separator)
    		{
    			s += ", ";
    		}
    		else
    		{
    			separator = true;
    		}
    		s += door.name();
    	}
    	return "Room (length = " + length + ", width = " + width + ", doors = { " + s + " })";
    }


}
