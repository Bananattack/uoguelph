/*
 * WidgetBuilder.java
 *
 * Written by Filip Anguelov, Andrew Crowell
 * ID: 0223807
 * E-mail: fanguelo@uoguelph.ca
 *
 * This class is responsible for building widgets once the
 * user has selected them from the widget panel.
 *
 *
 */

package tim;

import java.awt.event.MouseEvent;

import net.phys2d.math.Vector2f;
import Widgets.Widget;


/**
 * The Class WidgetBuilder.
 * @author Filip Anguelov
 */
public class WidgetBuilder
{

	/**
	 * Instantiates a new widget builder.
	 */
	public WidgetBuilder()
	{

	}


	/**
	 * Load widget.
	 * Written by Andrew Crowell
	 * 
	 * @param e The MouseEvent
	 * @param selectedWidget the selected widget
	 * 
	 * @return the game piece
	 * 
	 * @see MouseEvent
	 * @see Widget
	 */
	public GamePiece loadWidget(MouseEvent e, Widget selectedWidget)
	{


		/* Phil (2008-10-19): Testing out the widgets boundary */
		/* Andy (2008-10-20): Uses reflection to instantiate selected widget type. */
		Widget widget = null;

		try
		{
			Class c = selectedWidget.getClass();

			widget = (Widget) c.getConstructor(new Class[] {}).newInstance(new Object[] {});
			/* Phil (2008-10-26): Fixes widget boundry/paint error because i am accessing boundary while setting position later*/
			widget.setPosition( new Vector2f( 999999, 999999) );
			/* Phil (2008-20-24): Trying to center widget on mouse */
			widget.setPosition( new Vector2f( e.getX() - (( int ) widget.getBoundary()[2].getX() - ( int ) widget.getBoundary()[0].getX()) /2, e.getY() - ( ( int ) widget.getBoundary()[2].getY() - ( int ) widget.getBoundary()[0].getY() )/2 ) );
			/* Andrew (2008-11-20): Direction wasn't copying over before */
			widget.setDirection(selectedWidget.getDirection());


			GamePiece w = new GamePiece(widget);

			return w;
		}
		catch(Exception ex)
		{
			return null;
		}
	}
}
