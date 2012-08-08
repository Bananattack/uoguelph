/**
 *
 */
package tim;

import java.awt.Color;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Point;
import java.awt.Rectangle;
import java.awt.geom.Rectangle2D;
import java.util.ArrayList;

import net.phys2d.math.Vector2f;

import Widgets.Widget;

/**
 * @author epicbard
 *
 */
public class GameDrawer
{
	private static Color boundsColor = new Color(0, 255, 0, 127);

	public GameDrawer()
	{
		
	}
	/**
	 * Draws the widgets to the screen.
	 */
	public static void drawAWidget(Graphics g, Widget activeWidget, Point mousePosition)
	{
		if( activeWidget != null && mousePosition != null )
		{

			Vector2f[] bounds;

			/* Phil (2008-10-26): Fixes widget boundry/paint error because i am accessing boundary while setting position later*/
			activeWidget.setPosition( new Vector2f( 999999, 999999) );
			/* Phil (2008-10-24): Trying to center widget on mouse */
			activeWidget.setPosition(new Vector2f(mousePosition.x - (int) (activeWidget.getBoundary()[2].getX() - activeWidget.getBoundary()[0].getX()) / 2,
					mousePosition.y - (int) (activeWidget.getBoundary()[2].getY() - activeWidget.getBoundary()[0].getY()) / 2));

			bounds = activeWidget.getBoundary();

			int width = (int) (bounds[2].getX() - bounds[0].getX());
			int height = (int) (bounds[2].getY() - bounds[0].getY());

			g.setColor(boundsColor);
			g.fillRect((int)bounds[0].getX(), (int)bounds[0].getY(), width, height);


			activeWidget.draw((Graphics2D) g);
		}

	}
	
	private void clearScreen( Graphics2D g )
	{
		g.setColor(Color.WHITE);
		g.setClip(new Rectangle2D.Float(0, 0, 500, 500));
	    
	   // g.fillRect(0, 0, 500, 500);
	}

	public void render( Graphics2D g, ArrayList<GamePiece> levelWidgets )
	{
		clearScreen( g );
		
		if ( levelWidgets != null )
		{
			for(GamePiece widget : levelWidgets)
			{
				//System.out.println( widget.getWidget().getName() );
				g.setColor( Color.BLACK );
				widget.getWidget().draw( (Graphics2D) g );
				widget.update();
			}
		}
	}
}
