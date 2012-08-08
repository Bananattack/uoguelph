package Widgets;

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
 * A boxing glove widget for use in TIM. Activated by hitting the
 * blue square on the side of the widget. Shoots a ball on a spring
 * out the other side when activated.
 *
 * @author Darren O'Shea, 0227330
 * @.date October 17, 2008
 */
public class BoxingGlove implements Widget
{
	private static final boolean bDebug=false;
	private static ImageIcon icon;
	private Vector2f pos = new Vector2f(0,0);
	private Direction heading=Direction.WEST;
	private boolean bReset=false;
	private boolean bActive=false;
	private boolean bLocked=false;
	private Body box, floor, glove, button;
	private SpringJoint spring;

	public BoxingGlove()
	{
		box = new StaticBody("Base", new Box(32.0f, 32.0f)); 
        glove = new Body("Glove", new Circle(16), 100.f);
		button = new StaticBody("Button", new Box(8.0f, 8.0f));
		floor = new StaticBody("Floor", new Box(32.0f, 8.0f));
		spring = new SpringJoint(box, glove, box.getPosition(), glove.getPosition());
		spring.setMaxSpringSize(256.f);
	}

	public void setPosition(Vector2f f)
	{
		pos=f;
	
		Vector2f center = new Vector2f(f.x+37, f.y+21);

		if(heading == Direction.WEST)
		{
			box.setPosition(center.x+13, center.y-5);
			button.setPosition(center.x+20+13, center.y-5);
			glove.setPosition(center.x-32+13, center.y-5);
			floor.setPosition(center.x-32+13, center.y+20-5);
			spring.set(box, glove, box.getPosition(), glove.getPosition());
		}
		else
		{
			heading = Direction.EAST;

			box.setPosition(center.x-13, center.y-5);
			button.setPosition(center.x-20-13, center.y-5);
			glove.setPosition(center.x+32-13, center.y-5);
			floor.setPosition(center.x+32-13, center.y+20-5);
			spring.set(box, glove, box.getPosition(), glove.getPosition());
		}
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
	 * Draws a given Body. Called from the Draw() method to draw the different parts
	 * of the BoxingGlove widget.
	 *
	 * @param  g canvas to draw on
	 * @param  myBody Phys2D Body to be drawn
	 * @param  c myBody will be drawn with this color
	 */
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

	/**
	 * Draws the spring that connects the boxing glove to the base. Called from the Draw()
	 * method to draw the different parts of the BoxingGlove widget.
	 *
	 * @param  g canvas to draw on
	 * @param  c myBody will be drawn with this color
	 */
	private void drawSpring(Graphics2D g, Color c)
	{		
		//This drawing code inspired by drawJoint() method in net.phys2d.raw.test.AbstractDemo
		//by Kevin Glass, 2006
		//Source: http://www.cokeandcode.com/phys2d/source/builds/src060408.zip
		Vector2f p1 = (Vector2f)spring.getBody1().getPosition();
		Vector2f p2 = (Vector2f)spring.getBody2().getPosition();

		g.setColor(c);
		g.drawLine((int)p1.x, (int)p1.y, (int)p2.x, (int)p2.y);
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

	public void draw(Graphics2D g)
	{
		subDraw(g, box, Color.black);
		subDraw(g, button, Color.blue);
		subDraw(g, glove, Color.red);
		subDraw(g, floor, Color.black);
		drawSpring(g, Color.pink);

		if(bDebug == true)
		{
			drawBoundary(g, Color.green);
		}
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
        glove.set(new Circle(16), 100.f);
		bReset=true;
		setPosition(pos);
	}

	/**
	 * Makes the glove face in the other direction.
	 */
	private void flip()
	{
		if(heading == Direction.WEST)
		{
			setDirection(Direction.EAST);
		}
		else
		{
			setDirection(Direction.WEST);
		}
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
		return "Boxing Glove";
	}

	public ActionType getType()
	{
		return ActionType.IMPACT;
	}

	public void reactToTouchingBody(CollisionEvent e)
	{
		if(e.getBodyA() == button || e.getBodyB() == button)
		{
			glove.adjustVelocity(new Vector2f(-100,0));
		}
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
		Body[] bodies = {button, box, floor, glove};

		return bodies;
	}

	public Joint[] getJointsForSimulation()
	{
		Joint[] joints = {spring};

		return joints;
	}

	public void receiveImpulse(Vector2f anchor_point)
	{
	}

	public Vector2f[] getBoundary()
	{
		Vector2f[] points = {new Vector2f(pos.x+1, pos.y-1), new Vector2f(pos.x+75, pos.y-1), 
								new Vector2f(pos.x+75, pos.y+41), new Vector2f(pos.x+1,pos.y+41)};

		return points;
	}

	public String getDescription()
	{
		return "Punches things on impact.";
	}

	public ImageIcon getIcon()
	{
		if(icon == null)
		{
			icon = new ImageIcon("glove.gif");
		}

		return icon;
	}
}
