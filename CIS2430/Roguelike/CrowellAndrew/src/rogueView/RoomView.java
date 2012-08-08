package rogueView;

import rogueModel.*;
import java.awt.*;
import javax.swing.*;
import java.util.*;

/**
* The view for a room
* @author Andrew G. Crowell
*/
public class RoomView extends AreaView
{
	private int index; 
	private Tile[][] tiles;
	private HashMap<MapView.Direction, Tile> doors;
	
	/**
	* Gets a room's index
	* @return the room index
	*/
	public int getIndex()
	{
		return index;
	}
	
	/**
	* Creates a room view
	* @param room the room to base view upon
	* @param tileGrid the grid to place tiles into
	* @param index the room's index number, used to resolve its start location
	*/
	public RoomView(Room room, TileGrid tileGrid, int index)
	{
		try
		{
			setDungeonArea(room);
			this.index = index;
			this.tiles = new Tile[room.getWidth()][room.getLength()];
			Point p = tileGrid.getRoomTileStartPoint(index);
			for (int i = 0; i < room.getWidth(); i++)
			{
				for (int j = 0; j < room.getLength(); j++)
				{
					this.tiles[i][j] = tileGrid.getTile(p.x + i, p.y + j);
				}
			}
			this.doors = new HashMap<MapView.Direction, Tile>();
			setTiles();
		}
		catch(OutOfRangeException e)
		{
			System.out.println(e);
		}
	}
	
	/**
	* Sets up the tiles for the room
	* @throws OutOfRangeException if the initialization goes off-map somehow
	*/
	private void setTiles() throws OutOfRangeException
	{
		Room room = (Room) getDungeonArea();

		for(int i = 1; i < room.getLength() - 1; i++)
		{
			setTile(0, i, true, TileGrid.leftWallIcon);			
			setTile(room.getWidth() - 1, i, true, TileGrid.rightWallIcon);
		}
		
		// The corners
		setTile(0, 0, true, TileGrid.topLeftWallIcon);
		setTile(room.getWidth() - 1, 0, true, TileGrid.topRightWallIcon);
		setTile(0, room.getLength() - 1, true, TileGrid.bottomLeftWallIcon);
		setTile(room.getWidth() - 1, room.getLength() - 1, true, TileGrid.bottomRightWallIcon);

		// Top and bottom walls
		for(int i = 1; i < room.getWidth() - 1; i++)
		{	
			setTile(i, 0, true, TileGrid.horizontalWallIcon);
			setTile(i, room.getLength() - 1, true, TileGrid.horizontalWallIcon);
		}
		
		// Floor region
		for(int i = 1; i < room.getWidth() - 1; i++)
		{
			for(int j = 1; j < room.getLength() - 1; j++)
			{
				setTile(i, j, false, TileGrid.floorIcon);
			}
		}
		
		setDoors();
	}
	
	/**
	* Sets up the tiles for the doors in the room
	* @throws OutOfRangeException if the initialization goes off-map somehow
	*/
	public void setDoors() throws OutOfRangeException
	{
		Room room = (Room) getDungeonArea();
		for(MapView.Direction door : room.getDoorDirections())
		{
			if(door == MapView.Direction.NORTH)
			{
				doors.put(door, setTile(room.getWidth() / 2, 0, false, TileGrid.doorIcon));
			}
			if(door == MapView.Direction.SOUTH)
			{
				doors.put(door, setTile(room.getWidth() / 2, room.getLength() - 1, false, TileGrid.doorIcon));
			}
			if(door == MapView.Direction.WEST)
			{
				doors.put(door, setTile(0, room.getLength() / 2, false, TileGrid.doorIcon));
			}
			if(door == MapView.Direction.EAST)
			{
				doors.put(door, setTile(room.getWidth() - 1,  room.getLength() / 2, false, TileGrid.doorIcon));
			}
		}
	}
	
	/**
	* Gets the door tile corresponding to certain direction
	* @param direction the direction
	* @throws OutOfRangeException if it goes off-map somehow
	*/
	public Tile getDoor(MapView.Direction direction) throws OutOfRangeException
	{
		return doors.get(direction);
	}
	
	/**
	* Sets a tile in the room
	* @param x the horizontal position
	* @param y the vertical position
	* @param isWall whether or not this is a wall
	* @param icon the icon associated with the tile
	* @throws OutOfRangeException if it goes off-map somehow
	* @return the new tile
	*/
	public Tile setTile(int x, int y, boolean isWall, ImageIcon icon) throws OutOfRangeException
	{
		Tile tile = getTile(x, y);
	
		tile.setDefaultIcon(icon);
		tile.setObstructed(isWall);		
		tile.setAreaView(this);

		return tile;
	}
	
	/**
	* Gets a tile in the room
	* @param x the horizontal position
	* @param y the vertical position
	* @return the tile
	*/
	public Tile getTile(int x, int y)
	{
		return tiles[x][y];
	}
}
