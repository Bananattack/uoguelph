package Widgets;

import java.awt.Color;
import java.awt.Graphics2D;

import javax.swing.ImageIcon;

import net.phys2d.math.*;
import net.phys2d.raw.*;
import net.phys2d.raw.shapes.*;

/**
 *	ConveyorBelt is a widget that can be used in the TIM game.
 *	When a collision occurs between the ConveyorBelt and an object
 *	a force is added in the direction the ConveyorBelt is facing.
 *
 *	@author Trevor Clark, 0511119
 *	@.date October 19, 2008
 */

public class ConveyorBelt implements Widget
{
	private static final int WIDTH=70, HEIGHT=16;
	private static final int MAG=10;
	private Vector2f position;
	private ActionType type;
	private Vector2f size; /*x = width, y = height*/
	private Direction direct;
	private Body box;
	private boolean locked; 
	private boolean active;
	private boolean reset;
	
	public ConveyorBelt()
	{
		this.type = ActionType.IMPACT;
		this.size = new Vector2f(WIDTH, HEIGHT);
		this.position = new Vector2f(350, 200);
		this.direct = Direction.WEST;
		setLock(false);
		this.active = false;
		this.reset = false;
		
		this.box = new StaticBody(new Box(getWidth(), getHeight()));
		this.box.setPosition(getPositionX(), getPositionY());
	}
	
	public void setPosition(Vector2f f)
	{
		if (this.locked == false && this.active == false)
		{
			this.position = new Vector2f(f.getX(), f.getY());
			this.box.setPosition(getPositionX()+WIDTH/2, getPositionY()+HEIGHT/2);
			
		}
	}
	
	public void setPositionX(float x)
	{
		setPosition(new Vector2f(x, position.y));
	}

	public void setPositionY(float y)
	{
		setPosition(new Vector2f(position.x, y));
	}
	
	public Vector2f getPosition()
	{
		return (this.position);
	}
	
	public float getPositionX()
	{
		return (this.position.getX());
	}
	
	public float getPositionY()
	{
		return (this.position.getY());
	}
	
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
	
	private void subDraw(Graphics2D g, Body myBody, Color c)
	{
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
		}
	}

	public void draw(Graphics2D g)
	{
		subDraw(g, box, Color.red);
	}
	
	public void activateWidget()
	{
		if (reset = true)
		{
			this.active = true;
		}
	}
	
	public void resetWidget()
	{
		this.active = false;
		this.reset = true;
	}
	
	public void rotateClockwise()
	{
		switch(this.direct)
		{
			case EAST:
				this.direct = Direction.WEST;
				break;
			case WEST:
				this.direct = Direction.EAST;
				break;
		}
	}
	
	public void rotateCounterClockwise()
	{
		rotateClockwise();
	}

	public void setDirection(Direction d)
	{
		if (d != this.direct)
		{
			switch (d)
			{
				case NORTH: break;
				case SOUTH: break;
				case WEST: rotateClockwise(); break;
				case EAST: rotateClockwise(); break;
			}
		}
	}
	
	public Direction getDirection()
	{
		return (this.direct);
	}
	
	public boolean isActive()
	{
		return (active);
	}
	
	public String getName()
	{	
		return ("Conveyor Belt");
	}
	
	public ActionType getType()
	{
		return (this.type);
	}
	
	public void reactToTouchingBody(CollisionEvent e)
	{
		Body b1 = e.getBodyA();
		Body b2 = e.getBodyB();
		
		if (b1 == box)
		{
			if (direct == Direction.WEST)
			{
				b2.adjustVelocity(new Vector2f(-MAG, 0));
			}
			else
			{
				b2.adjustVelocity(new Vector2f(MAG, 0));
			}
		}
		else
		{
			if (direct == Direction.EAST)
			{
				b1.adjustVelocity(new Vector2f(-MAG, 0));
			}
			else
			{
				b1.adjustVelocity(new Vector2f(MAG, 0));
			}
		}
			

	}
	
	public boolean isConnectable()
	{
		return (false);
	}
	
	public void setConnectionPoints(Vector2f[] points)
	{
		/* MY WIDGET IS NOT CONNECTABLE
		   SO NOTHING IS DONE HERE */
	}
	
	public boolean isLocked()
	{
		return (this.locked);
	}
	
	public void setLock(boolean locked)
	{
		this.locked = locked;
	}
	
	public Vector2f attachJoint (Vector2f point)
	{
		return (null);
	}
	
	public Body[] getBodiesForSimulation ()
	{
		Body [] boxArray = new Body[1];
		boxArray[0] = this.box;
		
		return (boxArray);
		
	}
	
	public Joint[] getJointsForSimulation ()
	{
		return (null);
	}
	
	public void receiveImpulse(Vector2f anchor_point)
	{
		/* MY WIDGET DOES NOT USE JOINTS
		   SO NOTHING IS DONE HERE */
	}
	
	public Vector2f[] getBoundary()
	{
		
		return new Vector2f[] {
				new Vector2f(position.x+1, position.y), 
				new Vector2f(position.x+(int)getWidth()+1, position.y), 
				new Vector2f(position.x+(int)getWidth()+1, position.y+(int)getHeight()), 
				new Vector2f(position.x+1,position.y+(int)getHeight())};

	}
	
	public String getDescription()
	{
		return ("This Widget Is A Conveyor Belt");
	}
	
	public ImageIcon getIcon()
	{
		return (new ImageIcon ("resources/icons/widgets/ConveyorBelt.png"));
	}
	
/**
 * Returns the width of the ConveyorBelt.
 */
	public float getWidth()
	{
		return (this.size.getX());
	}

/**
 * Returns the height of the ConveyorBelt.
 */	
	public float getHeight()
	{
		return (this.size.getY());
	}

}


