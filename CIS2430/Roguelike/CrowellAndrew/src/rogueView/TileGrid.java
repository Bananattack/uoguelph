package rogueView;

import java.awt.*;
import javax.swing.*;

/**
* A panel holding a grid of all of a map's tile
* @author Andrew G. Crowell
*/
public class TileGrid extends JPanel
{
	public static ImageIcon horizontalWallIcon = new ImageIcon("wallhorizontal.png");
	public static ImageIcon leftWallIcon = new ImageIcon("wallleft.png");
	public static ImageIcon rightWallIcon = new ImageIcon("wallright.png");
	public static ImageIcon bottomLeftWallIcon = new ImageIcon("wallbottomleft.png");
	public static ImageIcon bottomRightWallIcon = new ImageIcon("wallbottomright.png");
	public static ImageIcon topLeftWallIcon = new ImageIcon("walltopleft.png");
	public static ImageIcon topRightWallIcon = new ImageIcon("walltopright.png");
	public static ImageIcon floorIcon = new ImageIcon("floor.png");
	public static ImageIcon doorIcon = new ImageIcon("door.png");
	public static ImageIcon hallwayIcon = new ImageIcon("hallway.png");
	public static ImageIcon emptyIcon = new ImageIcon("empty.png");
	
	private int tileWidth, tileHeight;
	private Tile[][] tiles;
	
	/**
	* Creates a new tile grid
	* @param tileWidth the width of the grid
	* @param tileHeight the height of the grid
	*/
	public TileGrid(int tileWidth, int tileHeight)
	{
		setLayout(new GridBagLayout());
		setBackground(new Color(0x2E, 0x34, 0x41));
		this.tileWidth = tileWidth;
		this.tileHeight = tileHeight;
		this.tiles = new Tile[tileWidth][tileHeight];
		for(int i = 0; i < tileWidth; i++)
		{
			for(int j = 0; j < tileHeight; j++)
			{
				GridBagConstraints c = new GridBagConstraints();
				c.fill = GridBagConstraints.BOTH;
				c.gridx = i;
				c.gridy = j;
				Tile t = new Tile(emptyIcon);
				t.setTileLocation(i, j);
				this.tiles[i][j] = t;
				add(t, c);
			}
		}
		
		for(int i = 0; i < tileWidth; i++)
		{
			for(int j = 0; j < tileHeight; j++)
			{
					if(i > 0)
					{
						tiles[i][j].setAdjacent(MapView.Direction.WEST, tiles[i - 1][j]);
					}
					if(i < tiles.length - 1)
					{
						tiles[i][j].setAdjacent(MapView.Direction.EAST, tiles[i + 1][j]);
					}
					if(j > 0)
					{
						tiles[i][j].setAdjacent(MapView.Direction.NORTH, tiles[i][j - 1]);
					}
					if(j < tiles[i].length - 1)
					{
						tiles[i][j].setAdjacent(MapView.Direction.SOUTH, tiles[i][j + 1]);
					}
			}
		}
	}
	
	/**
	* Gets a tile at the specified coordinates
	* @param tileX the x in the grid
	* @param tileY the y in the grid
	*/
	public Tile getTile(int tileX, int tileY) throws OutOfRangeException
	{
		if(tileX >= 0 && tileX < tileWidth
				&& tileY >= 0 && tileY < tileHeight)
		{
			return tiles[tileX][tileY];
		}
		throw new OutOfRangeException();
	}
	
	/**
	* Gets the point to start the room creation
	* @return the top-left corner of the room to make
	*/
	public Point getRoomTileStartPoint(int roomIndex)
	{
		return new Point(roomIndex % 3 * 11, roomIndex / 3 * 11);
	}	
}
