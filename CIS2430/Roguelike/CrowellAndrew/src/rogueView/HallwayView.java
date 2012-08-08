package rogueView;

import rogueModel.*;

/**
* A view corresponding to a hallway
* @author Andrew G. Crowell
*/
public class HallwayView extends AreaView
{
	private Room[] rooms;
	private RoomView[] roomViews;
	private TileGrid tileGrid;
	private MapView.Direction[] directions;
	private Tile[] doors;
	
	/**
	* Instantiates a new hallway view
	* @param hallway the hallway the view is based on
	* @param mapView the map necessary to create view
	* @param tileGrid the grid to plot tiles to.
	* @throws NoDoorThereException if the doors can't be located for some reason
	*/
	public HallwayView(Hallway hallway, MapView mapView, TileGrid tileGrid) throws NoDoorThereException
	{
		try
		{
			setDungeonArea(hallway);
			this.tileGrid = tileGrid;
			rooms = hallway.getRooms();
			roomViews = new RoomView[2];
			this.directions = new MapView.Direction[2];
			this.doors = new Tile[2];
			for(int i = 0; i < 2; i++)
			{				
				roomViews[i] = (RoomView) mapView.resolveAreaView(rooms[i]);
				
				MapView.Direction direction = Helper.getOppositeDirection(hallway.getDoorToRoom(rooms[i]));
				directions[i] = direction;
				doors[i] = roomViews[i].getDoor(direction);
			}
			setTiles();
		}
		catch(OutOfRangeException e)
		{
			throw new NoDoorThereException();
		}
	}
	
	/**
	* Connects the halls for the view
	* @throws OutOfRangeException if the doors can't be located for some reason
	*/
	private void setTiles() throws OutOfRangeException
	{
		Tile door1 = doors[0];
		Tile door2 = doors[1];
		int x = door1.getTileX();
		int y = door1.getTileY();
		int x2 = door2.getTileX();
		int y2 = door2.getTileY();
		
		while(x != x2 || y != y2)
		{
			Tile tile;
			if(x != door1.getTileX() || y != door1.getTileY())
			{
				tile = tileGrid.getTile(x, y);
				tile.setDefaultIcon(TileGrid.hallwayIcon);
				tile.setObstructed(false);
				tile.setAreaView(this);	
			}
			if(x < x2)
			{
				x++;
			}
			if(x > x2)
			{
				x--;
			}
			if(x == x2 && y == y2)
			{
				break;
			}
			if(x != door1.getTileX() || y != door1.getTileY())
			{
				tile = tileGrid.getTile(x, y);
				tile.setDefaultIcon(TileGrid.hallwayIcon);
				tile.setObstructed(false);
				tile.setAreaView(this);	
			}
			if(y < y2)
			{
				y++;
			}
			if(y > y2)
			{
				y--;
			}
		}
	}
}

