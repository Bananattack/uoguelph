


package tim;
import java.awt.Point;
import java.awt.geom.Rectangle2D;

import Widgets.Widget;
import Widgets.Widget.Direction;



/**
 * The Class GamePiece.
 */
public class GamePiece
{

	/** The this.widget. */
	private Widget widget;
	private int widgetID=-1;

	//private Direction currentDirection = Direction.EAST;

	/* The boundary, using Rectangle2D in order
	 * to utilize the handy contains() function */
	private Rectangle2D.Float boundingBox;


	/**
	 * Instantiates a new active this.widget.
	 *
	 * @param w the w
	 */
	public GamePiece( Widget w )
	{
		widget = w;

		float width =   w.getBoundary()[2].getX() -  w.getBoundary()[0].getX();
		float height =  w.getBoundary()[2].getY() -  w.getBoundary()[0].getY();
		
		//this.currentDirection = w.getDirection();

		boundingBox = new Rectangle2D.Float(w.getBoundary()[0].getX(), w.getBoundary()[0].getY(), width, height);

	}

	//protected void setDirection( Direction dir )
	//{
	//	//this.currentDirection = dir;
	//}
	
	protected void update()
	{
		float width =   this.widget.getBoundary()[2].getX() - this.widget.getBoundary()[0].getX();
		float height =   this.widget.getBoundary()[2].getY() - this.widget.getBoundary()[0].getY();
		
		//this.widget.setDirection( this.currentDirection );
		
		boundingBox = new Rectangle2D.Float( this.widget.getBoundary()[0].getX(), this.widget.getBoundary()[0].getY(), width, height);
	}



	/**
	 *
	 * Adds the this.widget.
	 *
	 * @param w the w
	 */
	protected void addWidget( Widget w )
	{
		this.widget = w;

		float width =  this.widget.getBoundary()[2].getX() - this.widget.getBoundary()[0].getX();
		float height = this.widget.getBoundary()[2].getY() - this.widget.getBoundary()[0].getY();

		boundingBox = new Rectangle2D.Float(this.widget.getBoundary()[0].getX(), this.widget.getBoundary()[0].getY(), width, height);
	}

	/**
	 * Gets the this.widget.
	 *
	 * @return the widget
	 */
	protected Widget getWidget()
	{
		return this.widget;
	}


	/**
	 * Checks if this widget collides with on already on the list.
	 *
	 * @param w the widget to check against
	 *
	 * @return true, if successful
	 */
	protected boolean collidesWith( GamePiece w )
	{
		return boundingBox.intersects(w.getBoundingBox());
	}

	/**
	 * Check to see if where the mouse clicked is a widget.
	 *
	 * @param p the p
	 *
	 * @return true, if successful
	 */
	protected boolean clickedOnAWidget(Point p)
	{
		return boundingBox.contains(p);
	}

	/**
	 * Gets the bounding box of the widget as a Rectangle2D
	 * object.
	 *
	 * @return the bounding box
	 */
	protected Rectangle2D.Float getBoundingBox()
	{
		return this.boundingBox;
	}

	protected Rectangle2D.Float getBox()
	{
		return this.boundingBox;
	}

	public int getWidgetID()
	{
		return widgetID;
	}

	public void setWidgetID(int inWidgetID)
	{
		widgetID = inWidgetID;
	}
}
