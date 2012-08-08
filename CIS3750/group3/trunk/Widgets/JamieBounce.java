package Widgets;

import java.awt.Graphics2D;

import javax.swing.ImageIcon;

import net.phys2d.math.*;
import net.phys2d.raw.*;
import net.phys2d.raw.shapes.*;
import java.awt.Color;
/**
 * This Widget is a stationary box, which takes all objects that collide with it, and make them bounce off
 * and also set their restitution to 1.
 *
 * Credits: Kevin Glass - the author of Phys2D.
 */
public class JamieBounce implements Widget
{
	private ActionType actionType1;
	private Direction direction1;
	private StaticBody mainBox;
	private StaticBody mainCircle;
	private String name;
	private String description;
	private ImageIcon icon;
	private boolean active;
	private boolean lock;
	private Body[] bodyArray;
	private Vector2f pos = new Vector2f(0.f, 0.f);
	private float tempX;
	private float tempY;
	private float centre;
	private float boundary;
	/**
	 *	The default constructor for the Bounce Square.
	 */
	public JamieBounce()
	{
		actionType1 = ActionType.IMPACT;
		direction1 = Direction.NORTH;
		mainBox = new StaticBody(new Box(75,75));
		mainCircle = new StaticBody(new Circle(60));
		mainCircle.setRestitution(1);
		name = new String("Bounce Ball");
		description = new String("Any object that comes into contact with this ball will bounce off (and the object will also becme slightly bouncy).");
		icon = new ImageIcon("./resources/icons/widgets/JamieBounce.png", "Bounce Ball");
		active = false;
		lock = false;
		bodyArray = new Body[2];
		bodyArray[0] = mainBox;
		bodyArray[1] = mainCircle;		
		tempX = 0;
		tempY = 0;
		centre = 30;
		boundary = 20;		
	}
	
	/**
	 * Set the position of the widget.
	 * @param f  The x,y coordinates within the
	 * container.
	 */
	public void setPosition(Vector2f f)
	{
		tempX = f.getX()+boundary+centre;
		tempY = f.getY()+boundary+centre;
		mainBox.setPosition(tempX,tempY);
		mainCircle.setPosition(tempX,tempY);
		pos = f;
	}
	
	/**
	 * Set the top-left x-coordinate of the widget.
	 * Should be equivalent to setPosition(x, getPositionY());
	 * @param x  The x coordinate within the container.
	 */
	public void setPositionX(float x)
	{
		this.setPosition(new Vector2f(x, getPositionY()));
	}
	
	/**
	 * Set the top-left y-coordinate of the widget.
	 * Should be equivalent to setPosition(getPositionX(), y);
	 * @param y  The y coordinate within the container.
	 */
	public void setPositionY(float y)
	{
		this.setPosition(new Vector2f(getPositionX(), y));
	}
	
	/**
	 * Get the top-left position of the widget.
	 * @return   The x,y coordinates within the container.
	 */
	public Vector2f getPosition()
	{
		return pos;
	}

	public float getPositionX()
	{
		return pos.x;
	}

	public float getPositionY()
	{
		return pos.y;
	}

	/**
	 * Draws the widget.
	 * Circle/Box drawing credit goes to Kevin Glass with code found at the following URL: 
	 * http://code.google.com/p/phys2d/source/browse/trunk/phys2d/test/net/phys2d/raw/test/AbstractDemo.java?spec=svn72&r=72
	 * Last updated: Oct 25, 2007
	 * @param g  The graphic to draw onto.
	 */
	public void draw(Graphics2D g)
	{
		g.setColor(Color.red);
		Box box = (Box)mainBox.getShape();
		Circle circle = (Circle)mainCircle.getShape();
		float x = mainCircle.getPosition().getX();
		float y = mainCircle.getPosition().getY();
		float r = circle.getRadius();
		g.drawOval((int) (x-r),(int) (y-r),(int) (r*2),(int) (r*2));
		Vector2f[] points = box.getPoints(mainBox.getPosition(), mainBox.getRotation());
		Vector2f v1 = points[0];
		Vector2f v2 = points[1];
		Vector2f v3 = points[2];
		Vector2f v4 = points[3];
		g.setColor(Color.black);
		g.drawLine((int) v1.x,(int) v1.y,(int) v2.x,(int) v2.y);
		g.drawLine((int) v2.x,(int) v2.y,(int) v3.x,(int) v3.y);
		g.drawLine((int) v3.x,(int) v3.y,(int) v4.x,(int) v4.y);
		g.drawLine((int) v4.x,(int) v4.y,(int) v1.x,(int) v1.y);
	}
	
	/**
	 * Activates the widget.
	 * This essentially just sets an active flag for the isActive method.
	 */  
	public void activateWidget()
	{
		active = true;
	}
	
	/**
	 * Resets the widget.
	 * This sets the active flag to false.
	 */
	public void resetWidget()
	{
		active = false;
	}
	
	/**
	 * Rotates the widget clockwise.
	 * Ignored for this widget.
	 */
	public void rotateClockwise()
	{
	}
	
	/**
	 * Rotates the widget counter-clockwise.
	 * Ignored for this widget.
	 */
	public void rotateCounterClockwise ()
	{
	}
	
	/**
	 * Sets the direction that the widget faces.
	 * Ignored for this widget.
	 * @param d Direction to rotate the widget.
	 */
	public void setDirection(Direction d)
	{
	}
	
	/**
	 * Returns the direction that the widget is facing.
	 * @return The direction the widget is facing.
	 */
	public Direction getDirection()
	{
		return direction1;
	}
	
	/**
	 * Check to see if the widget is active.
	 * @return True or false based on active flag.
	 */
	public boolean isActive()
	{
		if(active == true)
		{
			return true;
		}
		else return false;
	}
	
	/**
	 * Get the name of the widget.
	 * @return The name of the widget.
	 */
	public String getName()
	{
		return name;
	}
	
	/**
	 * Gets the type of the widget. This should be part of ActionType.
	 * @return The ActionType that the widget is categorized as.
	 */
	public ActionType getType()
	{
		return actionType1;
	}
	
	/**
	 * What happens with the widget when two bodies touch.
	 *
	 * Ensure that the widget carefully handles
	 * collision events between its own bodies.
	 *
	 * @param e The collision event that was triggered by a collision.
	 */
	public void reactToTouchingBody(CollisionEvent e)
	{
		if(e.getBodyA().equals(mainCircle))
		{
			e.getBodyB().setRestitution(1);
		}
		else e.getBodyA().setRestitution(1);
		return;
	}
	
	/**
	 * If this widget can connect to joints or not.
	 * This widget is false by default.
	 * @return The connectivity of the widget.
	 */
	public boolean isConnectable()
	{
		return false;
	}
	
	/**
	 * Sets the points where joints will connect to this widget.
	 * If the widget is not connectable, do nothing.
	 * @param points The points within the widget that designate
	 * where joints connect to.
	 * Ignored for this widget.
	 */
	public void setConnectionPoints(Vector2f[] points)
	{
	}
	
	/**
	 * Whether or not the widget is locked and cannot be moved.
	 * Lock state should not be a fixed value, so setLock does something
	 * @return If the widget is locked (true) or not (false).
	 */
	public boolean isLocked()
	{
		if(this.lock == true)
		{
			return true;
		}
		else return false;
	}
	
	/**
	 * Sets the lock state of the widget.
	 * Lock state returned by isLocked should reflect the change.
	 * @param locked If the widget is to be locked (true) or
	 * not (false).
	 */
	public void setLock(boolean locked)
	{
		if(locked == true)
		{
			lock = true;
		}
		else if(locked == false)
		{
			lock = false;
		}
		return;
	}
		
	/**
	 * The point that a joint is attempting to connect to the
	 * widget. What is returned is the closest connection point.
	 * If the widget is not connectable, return null.
	 * @param point The x,y coordinates where the joint is attempting to
	 * connected to.
	 * @return The connection point that the joint needs to be connected to,
	 * 		or null if Widget is not connectable.
	 */
	public Vector2f attachJoint(Vector2f point)
	{
		return null;
	}
	
	/**
	 * Returns an array of the Bodies this widget has, for use in Phys2D engine.
	 * The Body instances returned should stay the same across calls.
	 * This means create all Body objects you need when calling the constructor.
	 * @return An array of bodies.
	 */
	public Body[] getBodiesForSimulation()
	{
		return bodyArray;
	}
	
	/**
	 * Returns an array of the Joints this widget has, for use in Phys2D engine.
	 * The Joint instances returned should stay the same across calls.
	 * This means create all Joint objects you need when calling the constructor.
	 * @return An array of joints.
	 *     If you don't use any, you can return either null or an empty array.
	 */
	public Joint[] getJointsForSimulation()
	{
		return null;
	}
	
	/**
	 * Remote widget activation based from a connected Joint.
	 * Ignored for this widget.
	 * @param anchor_point The x,y coordinates of the connected Joint.
	 */
	public void receiveImpulse(Vector2f anchor_point)
	{
	}
	
	/**
	 * Gets the four boundary vertices of the widget,
	 * which chould be used as a way to detect overlapping widgets.
	 * The corner array must be in the following (clockwise) order:
	 *      corner[0]: top-left corner
	 *      corner[1]: top-right corner
	 *      corner[2]: bottom-right corner
	 *	    corner[3]: bottom-left corner
	 *	
	 *	To get width of the rectangular bound you go: corner[2].getX() - corner[0].getX()
	 *	To get height of the rectangular bound you go: corner[2].getY() - corner[0].getY()
	 * @return The four corners x,y coordinates.
	 */
	public Vector2f[] getBoundary()
	{
		Vector2f[] points = {	new Vector2f(getPositionX()+1, getPositionY()-1), 
								new Vector2f(getPositionX()+boundary+60+boundary, getPositionY()-1), 
								new Vector2f(getPositionX()+boundary+60+boundary, getPositionY()+boundary+60+boundary), 
								new Vector2f(getPositionX()+1, getPositionY()+boundary+60+boundary)};

		return points;
	}
	
	/**
	 * Retrieves the description of the widget.
	 * @return The small blurb describing what the widget does.
	 */
	public String getDescription()
	{
		return description;
	}
	
	/**
	 * Retrieves the images associated with the widget.
	 * The image should not exceed 100x100.
	 * @return The image which is linked with the widget.
	 */
	public ImageIcon getIcon()
	{
		return icon;
	}
}
