package rogueView;

import rogueModel.*;

/**
* A view of an area
* @author Andrew G. Crowell
*/
public abstract class AreaView
{
	DungeonArea dungeonArea;
	
	/**
	* Gets the area views's corresponding dungeon area
	* @return the dungeon area
	*/
	public DungeonArea getDungeonArea()
	{
		return dungeonArea;
	}

	/**
	* Sets the area views's corresponding dungeon area
	* @param dungeonArea the dungeon area
	*/	
	public void setDungeonArea(DungeonArea dungeonArea)
	{
		this.dungeonArea = dungeonArea;
	}
}
