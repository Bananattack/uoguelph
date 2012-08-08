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

	private GameController controller = null;
	private WidgetBuilder builder = null;
	/**
	 *
	 *
	 */
	public PlayfieldMouse(GameController controller, WidgetBuilder builder)
	{
		this.controller = controller;
		this.builder = builder;
	}

	public void mouseClicked(MouseEvent e)
	{
	}

	public void mouseReleased(MouseEvent e)
	{
		int x = e.getX();
		int y = e.getY();
		if(controller.isRunning())
		{
			return;
		}


		GamePiece clicked = clickedOn(e.getPoint());
		if(clicked != null
			&& !controller.isMovingOldPiece()
			&& !controller.isNewPiece()
			&& (GameWindow.getInstance().getGameType() || !clicked.getWidget().isLocked())
			)
		{
			Widget widget = clicked.getWidget();
			if(widget == controller.getSelectedPiece())
			{
				System.out.println("Moving: " + widget.getName());
				controller.setMovingOldPiece(true);
				controller.setMousePosition(e.getPoint());
			}
			else
			{
				System.out.println("Selected: " + widget.getName());
				controller.setSelectedPiece(widget);
				controller.setMousePosition(e.getPoint());
			}
		}
		else
		{
			// If we left clicked with an item selected and the cursor was on-screen,
			// try to place an item.
			if (e.getButton() == MouseEvent.BUTTON1
					&& controller.getSelectedPiece() != null
					&& x >= 0 && x < 500
					&& y >= 0 && y < 500
				)
			{
				/* Phil (2008-10-26): Added collision detection */
				if(canPlace(controller.getSelectedPiece()))
				{
					if(controller.isNewPiece())
					{
						GamePiece widget = builder.loadWidget(e, controller.getSelectedPiece());
						controller.addWidget(widget);
						controller.setNewPiece(false);

						// Placement complete!
						System.out.println("Placed item " + widget + " at (" + x + ", " + y + ")!");

						// If you hold down shift, the active item is remembered for next click.
						// To do this, we use a bitwise AND to check if the shift modifier is on.
						if((e.getModifiersEx() & MouseEvent.SHIFT_DOWN_MASK) == MouseEvent.SHIFT_DOWN_MASK)
						{
							System.out.println("Shift!");
							controller.setNewPiece(true);
						}
						// Otherwise, the active item is wiped.
						else
						{
							controller.setSelectedPiece(null);
							controller.setMousePosition(null);
						}
					}
					else if(controller.isMovingOldPiece())
					{
						Widget update = controller.getSelectedPiece();
						update.setPosition(new Vector2f(e.getX() - ((int) update.getBoundary()[2].getX()
											- (int) update.getBoundary()[0].getX()) /2,
										e.getY() - ((int) update.getBoundary()[2].getY()
											- (int) update.getBoundary()[0].getY())/2));
						controller.setSelectedPiece(null);
						controller.setMovingOldPiece(false);
						System.out.println("FINISHED MOVEMENT OF " + update.getName());
					}
					else if(!controller.isMovingOldPiece())
					{
						System.out.println("Deselecting: " + controller.getSelectedPiece().getName());
						controller.setSelectedPiece(null);
					}
				}
			}	
		}
	}

	private GamePiece clickedOn(Point p)
	{
		ArrayList<GamePiece> list = controller.getLevelPieces();
		if(list == null)
		{
			return null;
		}
		for (int i=0; i<list.size(); i++)
		{
			if (list.get(i).clickedOnAWidget(p))
			{
				System.out.println("Clicked on: " + list.get(i).getWidget().getName());
				return list.get(i);
			}
		}

		return null;
	}

	private boolean notInAlready(Widget w)
	{
		ArrayList<GamePiece> list = controller.getLevelPieces();
		for(int i=0; i<list.size();i++)
		{
			if(list.get(i).getWidget() == w)
			{
				System.out.println("Widget is in already.");
			}
		}

		return true;
	}

	private boolean canPlace(Widget w)
	{
		ArrayList<GamePiece> list = controller.getLevelPieces();
		if (list == null)
		{
			return true;
		}

		GamePiece active = new GamePiece(w);
		for (GamePiece piece : list)
		{
			if (piece.collidesWith(active) && piece.getWidget() != active.getWidget())
			{
				return false;
			}
		}
		return true;
	}

	public void mouseMoved(MouseEvent e)
	{
		if(controller.isMovingOldPiece() || controller.isNewPiece())
		{
			controller.setMousePosition(e.getPoint());
		}
	}

	public void mouseEntered(MouseEvent e)
	{
		if(controller.isMovingOldPiece() || controller.isNewPiece())
		{
			controller.setMousePosition(e.getPoint());
		}
	}

	public void mouseExited(MouseEvent e)
	{
		if(controller.isMovingOldPiece() || controller.isNewPiece())
		{
			controller.setMousePosition(null);
		}
	}

	public void mousePressed(MouseEvent e)
	{

	}

	public void mouseDragged(MouseEvent e)
	{

	}

}
