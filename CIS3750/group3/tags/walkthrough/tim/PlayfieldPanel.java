// CIS*3750
// PlayfieldPanel.java

package tim;

import Widgets.*;

import java.awt.*;
import java.awt.event.*;
import java.util.ArrayList;

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
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
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

	public void mouseReleased(MouseEvent e)
	{
		System.out.println("Drag Ended");
		//ActiveWidget activeWidget = GameWindow.getInstance().getActiveWidget();
		//if( activeWidget != null )
		//	activeWidget.getWidget().setPosition( new Vector2f( e.getX(), e.getY() ) );


		GameWindow.getInstance().setReleased( true );

	}

	public void mouseClicked(MouseEvent e)
	{
		//TODO: Forbid overlap of widgets.
		int x = e.getX();
		int y = e.getY();


		ActiveWidget clicked = clickedOn(e.getPoint());

		GameWindow.getInstance().setReleased( false );

		if( clicked != null && GameWindow.getInstance().getActiveWidget() == null )
		{

			System.out.println("Cliecked on: " + clicked.getWidget().getName() );
			GameWindow.getInstance().setActiveWidget( clicked.getWidget() );


		}else{



			// If we left clicked with an item selected and the cursor was on-screen,
			// try to place an item.
			if (e.getButton() == MouseEvent.BUTTON1
					&& GameWindow.getInstance().getActiveWidget() != null
					&& x >= 0 && x < PLAYFIELD_WIDTH
					&& y >= 0 && y < PLAYFIELD_HEIGHT
					&& GameWindow.getInstance().isNew()
			)
			{
				ActiveWidget widget = loadWidget( e );

				/* Phil (2008-10-26): Added collision detection */
				if( canPlace( widget.getWidget() ) &&  GameWindow.getInstance().isNew() )
				{

					GameWindow.getInstance().addWidget(widget);
					GameWindow.getInstance().setNew( false );

					// Placement complete!
					System.out.println("Placed item " + widget + " at (" + x + ", " + y + ")!");

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
				} 
				
				
				
			}
			else if ( GameWindow.getInstance().getActiveWidget() != null )
			{
				
				Widget update = GameWindow.getInstance().getActiveWidget();
				GameWindow.getInstance().getActiveWidget().setPosition( 
						new Vector2f( e.getX() - (( int ) update.getBoundary()[2].getX() - ( int ) update.getBoundary()[0].getX()) /2, 
								e.getY() - ( ( int ) update.getBoundary()[2].getY() - ( int ) update.getBoundary()[0].getY() )/2 ) );
				
				//GameWindow.getInstance().getActiveWidget();
				GameWindow.getInstance().setActiveWidget(null);
				GameWindow.getInstance().setMousePosition(null);

			}
		}
		repaint();
	}

	public void mouseMoved(MouseEvent e)
	{

		GameWindow.getInstance().setMousePosition( e.getPoint() );
	}

	private boolean canPlace( Widget w )
	{

		ArrayList<ActiveWidget> list = GameWindow.getInstance().getList();
		ActiveWidget active = new ActiveWidget();
		active.addWidget( w );

		for (int i=0; i<list.size(); i++)
		{
			if ( list.get( i ).collidesWith( active ) && ( list.get( i ).getWidget() != active ))
				return false;
		}

		return true;

	}

	private ActiveWidget clickedOn( Point p )
	{

	//	System.out.println("Checking if what i clicked on is a widget ");

		ArrayList<ActiveWidget> list = GameWindow.getInstance().getList();


		for (int i=0; i<list.size(); i++)
		{
			//System.out.println("List Has: " + list.get(i).getWidget().getName());
			if ( list.get( i ).clickedOnAWidget( p ) )
			{
				//System.out.println("Clicked on: " + list.get(i).getWidget().getName());
				return list.get( i );
				
			}
		}

		return null;
	}

	private boolean notInAlready(Widget w)
	{
		ArrayList<ActiveWidget> list = GameWindow.getInstance().getList();
		for(int i=0; i<list.size();i++)
		{
			if( list.get( i ).getWidget() == w)
			{
				System.out.println("Shit is the same");
			}
			else
				System.out.println("Shit is not same");
		}

		return true;
	}

	@SuppressWarnings("unchecked")
	private ActiveWidget loadWidget(MouseEvent e)
	{

		Widget activeWidget = GameWindow.getInstance().getActiveWidget();
		/* Phil (2008-10-19): Testing out the widgets boundary */
		/* Andy (2008-10-20): Uses reflection to instantiate selected widget type. */
		Widget widget = null;

		try
		{
			Class c = activeWidget.getClass();
			widget = (Widget) c.getConstructor(new Class[] {}).newInstance(new Object[] {});
			/* Phil (2008-10-26): Fixes widget boundry/paint error because i am accessing boundary while setting position later*/
			widget.setPosition( new Vector2f( 999999, 999999) );
			/* Phil (2008-20-24): Trying to center widget on mouse */
			widget.setPosition( new Vector2f( e.getX() - (( int ) widget.getBoundary()[2].getX() - ( int ) widget.getBoundary()[0].getX()) /2, e.getY() - ( ( int ) widget.getBoundary()[2].getY() - ( int ) widget.getBoundary()[0].getY() )/2 ) );
			
			ActiveWidget w = new ActiveWidget(widget);
			return w;
		}
		catch(Exception ex)
		{
			return null;
		}
	}

	public void mouseEntered( MouseEvent e )
	{
		// TODO Auto-generated method stub

	}

	public void mouseExited( MouseEvent e )
	{
		// TODO Auto-generated method stub

	}

	public void mousePressed( MouseEvent e )
	{
		// TODO Auto-generated method stub

	}

	public void mouseDragged( MouseEvent e )
	{
		// TODO Auto-generated method stub

	}
}
