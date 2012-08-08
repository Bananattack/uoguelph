// CIS*3750
// PlayfieldPanel.java

package tim;

import Widgets.*;
import java.awt.*;
import java.awt.event.*;

import javax.swing.*;
import javax.swing.border.*;

import net.phys2d.math.Vector2f;

/**
 * PlayfieldPanel is where all the action, puzzle editing
 * and so forth occurs.
 *
 * @author Andrew Crowell
 */
public class PlayfieldPanel extends JPanel implements MouseListener, MouseMotionListener
{
	/* Phil (2008-10-20): After removing the side panel, the size of the game area changed. Must fix it somehow
	 * changed the WIDTH from 500 to 687. Don't know if you still want 500x500...
	 */
	/* Andy (2008-10-20): Yes, yes I do. */
	/* Phil (2008-10-21): I like pie */
	public static final int PLAYFIELD_WIDTH = 500;
	public static final int PLAYFIELD_HEIGHT = 500;
	public PlayfieldPanel()
	{
		super();
		setSize(PLAYFIELD_WIDTH, PLAYFIELD_HEIGHT);
		setMinimumSize(new Dimension(PLAYFIELD_WIDTH, PLAYFIELD_HEIGHT));
		setPreferredSize(new Dimension(PLAYFIELD_WIDTH, PLAYFIELD_HEIGHT));
		setMaximumSize(new Dimension(PLAYFIELD_WIDTH, PLAYFIELD_HEIGHT));
		/* Phil (2008-10-19): Trying to make it look better */
		setBorder(new EtchedBorder());
		setBackground( Color.WHITE );
		addMouseListener(this);
		addMouseMotionListener(this);
	}

	public void paintComponent(Graphics g)
	{
		super.paintComponent(g);
		GameWindow.getInstance().render(g);
	}

	public void mousePressed(MouseEvent e)
	{
	}

	public void mouseReleased(MouseEvent e)
	{
		//TODO: Forbid overlap of widgets.
		int x = e.getX();
		int y = e.getY();
		Widget activeWidget = GameWindow.getInstance().getActiveWidget();

		// If we left clicked with an item selected and the cursor was on-screen,
		// try to place an item.
		if (e.getButton() == MouseEvent.BUTTON1
			&& activeWidget != null
			&& x >= 0 && x < PLAYFIELD_WIDTH
			&& y >= 0 && y < PLAYFIELD_HEIGHT)
		{
			/* Phil (2008-10-19): Testing out the widgets boundary */
			/* Andy (2008-10-20): Uses reflection to instantiate selected widget type. */
			Widget widget = null;
			try
			{
				Class c = activeWidget.getClass();
				widget = (Widget) c.getConstructor(new Class[] {}).newInstance(new Object[] {});
			}
			catch(Exception ex)
			{
			}
			widget.setPosition( new Vector2f( e.getX(), e.getY() ) );

			int width =  ( int ) widget.getBoundary()[2].getX() - ( int ) widget.getBoundary()[0].getX();
			int height =  ( int ) widget.getBoundary()[2].getY() - ( int ) widget.getBoundary()[0].getY();

			GameWindow.getInstance().addWidget(widget);

			// Placement complete!
			System.out.println("Placed item " + activeWidget + " at (" + x + ", " + y + ")!");

			// If you hold down shift, the active item is remembered for next click.
			// To do this, we use a bitwise AND to check if the shift modifier is on.
			if((e.getModifiersEx() & MouseEvent.SHIFT_DOWN_MASK) == MouseEvent.SHIFT_DOWN_MASK)
			{
				System.out.println("Shift!");
			}
			// Otherwise, the active item is wiped.
			else
			{
				GameWindow.getInstance().setActiveWidget(null);
				GameWindow.getInstance().setMousePosition(null);
			}

 			repaint();
		}
	}

	public void mouseEntered(MouseEvent e)
	{
	}

	public void mouseExited(MouseEvent e)
	{
		GameWindow.getInstance().setMousePosition(null);
	}

	public void mouseClicked(MouseEvent e)
	{
	}

	public void mouseMoved(MouseEvent e)
	{

		/* Phil: here be dragons
		 *
		 * I suggest we draw an outline of the boundary while moving
		 */
		// Andy: Slayed them dragons.
		GameWindow.getInstance().setMousePosition(e.getPoint());
	}

	public void mouseDragged( MouseEvent e )
	{
	}

}
