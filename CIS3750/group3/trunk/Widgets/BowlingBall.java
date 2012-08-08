package Widgets;

import tim.*;
import javax.swing.ImageIcon;
import net.phys2d.math.*;
import net.phys2d.raw.*;
import net.phys2d.math.Vector2f;
import net.phys2d.raw.strategies.*;
import net.phys2d.raw.shapes.*;
import java.awt.*;
import java.awt.event.*;
import java.util.*;


/**
 * A bowling ball widget for use in TIM.
 *
 * @author Darren O'Shea, 0227330
 * @.date October 23, 2008
 */
public class BowlingBall implements Widget
{
	private static final boolean bDebug=false;
	private static final float RADIUS=16;
	private static ImageIcon icon;
	private Vector2f pos = new Vector2f(0,0);
	private Direction heading=Direction.WEST;
	private boolean bReset=false;
	private boolean bActive=false;
	private boolean bLocked=false;
	private Body circle;
	

	public BowlingBall()
	{ 
        circle = new Body("Ball", new Circle(RADIUS), 100.f);
	}

	public void setPosition(Vector2f f)
	{
		pos=f;
	
		Vector2f center = new Vector2f(f.getX() + RADIUS, f.getY() + RADIUS);
		circle.setPosition(center.x, center.y);		
	}

	public void setPositionX(float x)
	{
		setPosition(new Vector2f(x, pos.y));
	}

	public void setPositionY(float y)
	{
		setPosition(new Vector2f(pos.x, y));
	}

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
	 * Draws the boundary of the widget. Called from the Draw()
	 * method to draw the different parts of the BoxingGlove widget.
	 * Used in debugging.
	 *
	 * @param  g canvas to draw on
	 * @param  c myBody will be drawn with this color
	 */
	private void drawBoundary(Graphics2D g, Color c)
	{		
		//This drawing code inspired by drawJoint() method in net.phys2d.raw.test.AbstractDemo
		//by Kevin Glass, 2006
		//Source: http://www.cokeandcode.com/phys2d/source/builds/src060408.zip

		Vector2f[] points = getBoundary();

		g.setColor(c);
		for(int i=0; i<4; i++)
		{
			g.drawLine((int)points[i].x, (int)points[i].y, (int)points[(i+1)%4].x, (int)points[(i+1)%4].y);
		}		
	}

	/**
	 * Draws a given Body. Called from the Draw() method to draw the different parts
	 * of the BoxingGlove widget.
	 *
	 * @param  g canvas to draw on
	 * @param  myBody Phys2D Body to be drawn
	 * @param  c myBody will be drawn with this color
	 */
	private void subDraw(Graphics2D g, Body myBody, Color c)
	{
		/* Back to drawing the icon */
		icon.paintIcon(GameWindow.getInstance(), g, (int)(circle.getPosition().getX()-RADIUS), (int)(circle.getPosition().getY()-RADIUS));
		
		
		
		/*
		g.setColor(c);

		//This drawing code inspired by drawBody() method in net.phys2d.raw.test.AbstractDemo
		//by Kevin Glass, 2006
		//Source: http://www.cokeandcode.com/phys2d/source/builds/src060408.zip
		if (myBody.getShape() instanceof Box)
		{
			Vector2f[] pts = ((Box)myBody.getShape()).getPoints(myBody.getPosition(), myBody.getRotation());
			g.drawLine((int) pts[0].x,(int) pts[0].y,(int) pts[1].x,(int) pts[1].y);
			g.drawLine((int) pts[1].x,(int) pts[1].y,(int) pts[2].x,(int) pts[2].y);
			g.drawLine((int) pts[2].x,(int) pts[2].y,(int) pts[3].x,(int) pts[3].y);
			g.drawLine((int) pts[3].x,(int) pts[3].y,(int) pts[0].x,(int) pts[0].y);
		}
		if (myBody.getShape() instanceof Circle)
		{
			Circle myCircle;
			myCircle = (Circle)myBody.getShape();
			g.drawOval((int)myBody.getPosition().getX()-(int)myCircle.getRadius(), (int)myBody.getPosition().getY()-(int)myCircle.getRadius(), 
					(int)myCircle.getRadius()*2, (int)myCircle.getRadius()*2);
		}*/
	}

	public void draw(Graphics2D g)
	{
		subDraw(g, circle, Color.pink);
	}

	public void activateWidget()
	{
		if(bReset == true)
		{
			bActive=true;
		}
	}

	public void resetWidget()
	{
		circle.set(new Circle(RADIUS), 100.f);
		bReset=true;
		setPosition(pos);
	}

	/**
	 * Makes the glove face in the other direction.
	 */
	private void flip()
	{
		heading = Direction.NORTH;
	}

	public void rotateClockwise()
	{
		flip();
	}

	public void rotateCounterClockwise()
	{
		flip();
	}

	public void setDirection(Direction d)
	{
		heading = d;
		setPosition(pos);
	}

	public Direction getDirection()
	{
		return heading;
	}

	public boolean isActive()
	{
		return bActive;
	}

	public String getName()
	{
		return "Bowling Ball";
	}

	public ActionType getType()
	{
		return ActionType.IMPACT;
	}

	public void reactToTouchingBody(CollisionEvent e)
	{
	}

	public boolean isConnectable()
	{
		return false;
	}

	public void setConnectionPoints(Vector2f[] points)
	{
	}

	public boolean isLocked()
	{
		return bLocked;
	}

	public void setLock(boolean locked)
	{
		bLocked=locked;
	}

	public Vector2f attachJoint (Vector2f point)
	{
		return null;
	}

	public Body[] getBodiesForSimulation()
	{
		Body[] bodies = {circle};

		return bodies;
	}

	public Joint[] getJointsForSimulation()
	{
		return null;
	}

	public void receiveImpulse(Vector2f anchor_point)
	{
	}

	public Vector2f[] getBoundary()
	{
		Vector2f[] points = {new Vector2f(pos.x+1, pos.y-1), new Vector2f(pos.x+32, pos.y-1), 
								new Vector2f(pos.x+32, pos.y+31), new Vector2f(pos.x+1,pos.y+31)};

		return points;
	}

	public String getDescription()
	{
		return "Falling bowling ball.";
	}

	public ImageIcon getIcon()
	{
		if(icon == null)
		{
			icon = new ImageIcon("resources/icons/widgets/ball.png");
		}

		return icon;
	}
}
