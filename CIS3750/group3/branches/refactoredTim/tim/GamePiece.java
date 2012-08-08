package tim;
import java.awt.Point;
import java.awt.geom.Rectangle2D;

import Widgets.Widget;


// TODO: Auto-generated Javadoc
/**
 * The Class GamePiece.
 */
public class GamePiece
{
	
	/** The widget. */
	private Widget widget;
	
	/* The boundary, using Rectangle2D in order 
	 * to utilize the handy contains() function */
	private Rectangle2D.Double boundingBox;
	
	
	/**
	 * Instantiates a new active widget.
	 */
	protected GamePiece()
	{
		
	}
	
	/**
	 * Instantiates a new active widget.
	 * 
	 * @param w the w
	 */
	protected GamePiece( Widget w )
	{
		this.widget = w;
		
		double width =  ( double ) ( widget.getBoundary()[2].getX() - ( double ) widget.getBoundary()[0].getX() );
		double height =  ( double ) ( widget.getBoundary()[2].getY() - ( double ) widget.getBoundary()[0].getY() );
		
		boundingBox = new Rectangle2D.Double(widget.getBoundary()[0].getX(), widget.getBoundary()[0].getY(), width, height);
	}
	
	protected void update()
	{
		double width =  ( double ) ( widget.getBoundary()[2].getX() - ( double ) widget.getBoundary()[0].getX() );
		double height =  ( double ) ( widget.getBoundary()[2].getY() - ( double ) widget.getBoundary()[0].getY() );
		
		boundingBox = new Rectangle2D.Double(widget.getBoundary()[0].getX(), widget.getBoundary()[0].getY(), width, height);
	}
	
	protected void setWidget( Widget w )
	{
		this.widget = w;
	}
	

	/**
	 * 
	 * Adds the widget.
	 * 
	 * @param w the w
	 */
	protected void addWidget( Widget w )
	{
		this.widget = w;
		
		double width =  ( double ) ( widget.getBoundary()[2].getX() - ( double ) widget.getBoundary()[0].getX() );
		double height =  ( double ) ( widget.getBoundary()[2].getY() - ( double ) widget.getBoundary()[0].getY() );
		
		boundingBox = new Rectangle2D.Double(widget.getBoundary()[0].getX(), widget.getBoundary()[0].getY(), width, height);
	}
	
	/**
	 * Gets the widget.
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
		System.out.println( this.boundingBox.intersects( w.getBoundingBox() ) );
		return ( this.boundingBox.intersects( w.getBoundingBox() ) );
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
		return boundingBox.contains( p );
	}
	
	/**
	 * Gets the bounding box of the widget as a Rectangle2D
	 * object.
	 * 
	 * @return the bounding box
	 */
	protected Rectangle2D.Double getBoundingBox()
	{
		return this.boundingBox;
	}
	
	protected Rectangle2D.Double getBox()
	{
		return this.boundingBox;
	}

	
}
