package rogueModel;

import rogueView.*;

/**
 * A bunch of useful static methods that are handy for the game logic.
 */
public class Helper
{	
	/**
	 * Generates a random integer in between and including
	 * a specificied range of numbers
	 * 
	 * @param low - the lower number in range
	 * @param high - the upper number in range
	 * @return the resulting random number
	 * 
	 */
    public static int Random(int low, int high)
    {
        return (int) (Math.random () * (high - low + 1)) + low;
    }
	
	/**
	 * Returns the direction opposite to the one passed
	 * 
	 * @param direction - direction to invert.
	 * @return direction after being turned around.
	 */
	public static MapView.Direction getOppositeDirection(MapView.Direction direction)
	{
		switch(direction)
		{
			case NORTH:
				return MapView.Direction.SOUTH;
			case SOUTH:
				return MapView.Direction.NORTH;
			case EAST:
				return MapView.Direction.WEST;
			case WEST:
				return MapView.Direction.EAST;
			default:
				return null;
		}
	}
	
	/**
	 * Returns a random direction.
	 * 
	 * @return the direction randomly chosen.
	 */
	public static MapView.Direction getRandomDirection()
	{
		int directionIndex = (int) (Math.random() * 4); 
		if (directionIndex == 0)
		{
			return MapView.Direction.NORTH;
		}
		else if (directionIndex == 1)
		{
			return MapView.Direction.SOUTH;
		}
		else if (directionIndex == 2)
		{
			return MapView.Direction.EAST;
		}
		else if (directionIndex == 3)
		{
			return MapView.Direction.WEST;
		}
		return null;
	}
	
	/**
	 * Returns the x offset of the a certain direction
	 * 
	 * @return the change in x position
	 */
	public static int getDirectionHorizontalOffset(MapView.Direction direction)
	{
		switch(direction)
		{
			case EAST:
				return 1;
			case WEST:
				return -1;
			default:
				return 0;
		}
	}
	
	/**
	 * Returns the y offset of the a certain direction
	 * 
	 * @return the change in y position
	 */
	public static int getDirectionVerticalOffset(MapView.Direction direction)
	{
		switch(direction)
		{
			case SOUTH:
				return 1;
			case NORTH:
				return -1;
			default:
				return 0;
		}
	}
	
}