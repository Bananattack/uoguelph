/**
 * 
 */
package tim;

import java.awt.Point;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;
import java.util.ArrayList;

import net.phys2d.math.Vector2f;
import Widgets.Widget;

/**
 * @author epicbard
 *
 */
public class PlayfieldMouse implements MouseListener, MouseMotionListener
{

	private PlayfieldPanel panel = null;
	private WidgetBuilder builder = null;
	/**
	 * 
	 */
	public PlayfieldMouse(PlayfieldPanel panel, WidgetBuilder builder)
	{
		this.panel = panel;
		this.builder = builder;
	}
	
	public void mouseReleased(MouseEvent e)
	{
		System.out.println("Drag Ended");
		//GamePiece GamePiece = GameWindow.getInstance().getGamePiece();
		//if( GamePiece != null )
		//	GamePiece.getWidget().setPosition( new Vector2f( e.getX(), e.getY() ) );


		GameWindow.getInstance().setReleased( true );

	}

	public void mouseClicked(MouseEvent e)
	{
		//TODO: Forbid overlap of widgets.
		int x = e.getX();
		int y = e.getY();


		GamePiece clicked = clickedOn(e.getPoint());

		GameWindow.getInstance().setReleased( false );

		if( clicked != null && GameWindow.getInstance().getGamePiece() == null )
		{

			System.out.println("Cliecked on: " + clicked.getWidget().getName() );
			GameWindow.getInstance().setGamePiece( clicked.getWidget() );


		}else{



			// If we left clicked with an item selected and the cursor was on-screen,
			// try to place an item.
			if (e.getButton() == MouseEvent.BUTTON1
					&& GameWindow.getInstance().getGamePiece() != null
					&& x >= 0 && x < panel.getWidth()
					&& y >= 0 && y < panel.getHeight()
					&& GameWindow.getInstance().isNew()
			)
			{
				GamePiece widget = builder.loadWidget( e );

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
						GameWindow.getInstance().setGamePiece(null);
						GameWindow.getInstance().setMousePosition(null);
					}
				} 
				
				
				
			}
			else if ( GameWindow.getInstance().getGamePiece() != null )
			{
				
				Widget update = GameWindow.getInstance().getGamePiece();
				GameWindow.getInstance().getGamePiece().setPosition( 
						new Vector2f( e.getX() - (( int ) update.getBoundary()[2].getX() - ( int ) update.getBoundary()[0].getX()) /2, 
								e.getY() - ( ( int ) update.getBoundary()[2].getY() - ( int ) update.getBoundary()[0].getY() )/2 ) );
				
				//GameWindow.getInstance().getGamePiece();
				GameWindow.getInstance().setGamePiece(null);
				GameWindow.getInstance().setMousePosition(null);

			}
		}
		panel.repaint();
	}
	
	private GamePiece clickedOn( Point p )
	{

	//	System.out.println("Checking if what i clicked on is a widget ");

		ArrayList<GamePiece> list = GameWindow.getInstance().getList();


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
		ArrayList<GamePiece> list = GameWindow.getInstance().getList();
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
	
	private boolean canPlace( Widget w )
	{

		ArrayList<GamePiece> list = GameWindow.getInstance().getList();
		GamePiece active = new GamePiece();
		active.addWidget( w );

		for (int i=0; i<list.size(); i++)
		{
			if ( list.get( i ).collidesWith( active ) && ( list.get( i ).getWidget() != active ))
				return false;
		}

		return true;

	}

	public void mouseMoved(MouseEvent e)
	{

		GameWindow.getInstance().setMousePosition( e.getPoint() );
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
