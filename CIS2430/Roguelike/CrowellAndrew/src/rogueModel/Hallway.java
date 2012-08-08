package rogueModel;

import rogueView.*;

/**
 * The connecting area between two rooms.
 * 
 * @author Andrew Crowell 
 */
public class Hallway extends DungeonArea
{
	private Room roomStart, roomEnd;
	private MapView.Direction directionStart, directionEnd;
	
	/**
	 * Creates a hallway between two rooms with entrances in the specified directions.
	 * 
	 * @param roomStart - the starting point of the hall
	 * @param directionStart - the direction to leave the start room to enter the hallway
	 * @param roomEnd - the end point of the hall
	 * @param directionEnd - the direction to leave the end room to enter the hallway.
	 * @throws Room.DoorOccupiedException on failure to add a passageway from a room to the hall.
	 */
	public Hallway(Room roomStart, MapView.Direction directionStart,
					Room roomEnd, MapView.Direction directionEnd) throws Room.DoorOccupiedException
	{
		this.roomStart = roomStart;
		this.directionStart = Helper.getOppositeDirection(directionStart);
		this.roomEnd = roomEnd;
		this.directionEnd = Helper.getOppositeDirection(directionEnd);
		
		try
		{
			this.roomStart.addDoor(directionStart);
			this.roomEnd.addDoor(directionEnd);
		}
		catch(Room.DoorOccupiedException e)
		{
			throw new Room.DoorOccupiedException();
		}
	}

	/**
	 * Returns the direction of the door to the specified room.  The room must be one that is connected
	 * to the hallway
	 * @param r the room to be checked
	 * @return A direction indicating which direction to travel to enter the room from the hallway,
	 * @throws java.util.NoSuchElementException if the room is not connected to the hall,
	 * since it's impossible to tell what direction to return if the room's not tied to this hall. 
	 */
	public MapView.Direction getDoorToRoom(Room r) throws java.util.NoSuchElementException
	{ 
		if (r == roomStart)
		{
			return directionStart;
		}
		else if (r == roomEnd)
		{
			return directionEnd;
		}
		throw new java.util.NoSuchElementException("This hallway is not connected to the specified room.");
	}
	
	/**
	 * @return An array of exactly two elements containing the rooms in this hallway.  Hallways may only have 
	 * two rooms.
	 */
	public Room[] getRooms()
	{
		return new Room[] { roomStart, roomEnd };
	}

    /**
     * A useful string representation of a Hallway.
     * @return the hallway as a string
     */
    public String toString()
    {
    	return "Hallway (from (" + roomStart + ") in the " + directionStart.name()
    			+ " to (" + roomEnd + ") in the " + directionEnd.name();
    	
    }
	
}
