package rogueModel;
import javax.swing.*;

/**
 * A story element represents the things that are in the dungeon.
 * Don't change this, because that's bad and stuff.
 */
public abstract class StoryElement
{
	/**
	 * This method returns the text character that is used to represent the element in the
	 * text display.
	 * @return a Character that should be used to represent the element.
	 */ 
	public abstract ImageIcon getImageIcon();

}