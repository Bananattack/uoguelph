package Widgets;


import java.awt.Color;
import java.awt.Graphics2D;
import java.awt.geom.Ellipse2D;
import java.awt.geom.Rectangle2D;
import java.awt.geom.Ellipse2D;
import java.awt.geom.Ellipse2D.Float;
import java.awt.RenderingHints;

import javax.swing.ImageIcon;

import net.phys2d.math.*;
import net.phys2d.raw.*;
import net.phys2d.raw.shapes.*;

/**
 * 
 */

/**
 * @author judda
 *
 */
public class Ball implements Widget
{
	Body bod;

	private float x = 0.0f;
	private float y = 0.0f;

	private float radius = 0f;
	
	private Vector2f [] vecBoundaries;
	private boolean locked;
	
	private Body [] bodies;
	/**
	 * 
	 */
	public Ball ()
	{
		// TODO Auto-generated constructor stub
		this.radius = 10f;
		this.bodies = new Body[1];
		this.bodies[0] = new Body(new Circle(radius), 10);
		this.bodies[0].setPosition(x+radius, y+radius);
		this.bodies[0].setRestitution ( .25f );
	}

	/* (non-Javadoc)
	 * @see Widget#Draw(java.awt.Graphics2D)
	 */
	public void draw(Graphics2D g)
	{
		Body c = bodies[0];
		// TODO Auto-generated method stub
		g.setColor( Color.black );
		g.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
		g.draw(new Ellipse2D.Float(c.getPosition().getX() - radius, c.getPosition().getY() - radius, radius *2, radius*2));
		g.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_OFF);
	}

	/* (non-Javadoc)
	 * @see Widget#activateWidget()
	 */
	public void activateWidget() {
		// TODO Auto-generated method stub

	}

	/* (non-Javadoc)
	 * @see Widget#anotherBodyTouching(net.phys2d.raw.Body, net.phys2d.math.Vector2f)
	 */
	public void anotherBodyTouching(Body other_body, Vector2f f)
	{
		// TODO Auto-generated method stub

	}

	/* (non-Javadoc)
	 * @see Widget#applyForce(net.phys2d.raw.Body)
	 */
	public void applyForce(Body other_body) {
		// TODO Auto-generated method stub

	}

	/* (non-Javadoc)
	 * @see Widget#attachJoint(net.phys2d.math.Vector2f)
	 */
	public Vector2f attachJoint(Vector2f point) {
		// TODO Auto-generated method stub
		return null;
	}

	/* (non-Javadoc)
	 * @see Widget#flipLeftRight()
	 */
	public void flipLeftRight() {
		// TODO Auto-generated method stub

	}

	/* (non-Javadoc)
	 * @see Widget#flipUpDown()
	 */
	public void flipUpDown() {
		// TODO Auto-generated method stub

	}

	/* (non-Javadoc)
	 * @see Widget#getAppliedForce()
	 */
	public float getAppliedForce() {
		// TODO Auto-generated method stub
		return 0;
	}

	/* (non-Javadoc)
	 * @see Widget#getBoundary()
	 */
	public Vector2f[] getBoundary()
	{
		Vector2f[] boundary = new Vector2f[4];
		
		boundary[0] = new Vector2f(x, y);
		boundary[1] = new Vector2f(x+radius*2, y);
		boundary[2] = new Vector2f(x+radius*2, y+radius*2);
		boundary[3] = new Vector2f(x, y+radius*2);
		return boundary;
	}

	/* (non-Javadoc)
	 * @see Widget#getDescription()
	 */
	public String getDescription()
	{
		// TODO Auto-generated method stub
		
		String strDesc = "YAY! I'm a ball!<br >Boing Boing Boing...";
		
		return ( strDesc );
	}

	/* (non-Javadoc)
	 * @see Widget#getName()
	 */
	public String getName()
	{
		// TODO Auto-generated method stub
		return ( "Ball" );
	}

	/* (non-Javadoc)
	 * @see Widget#getPosition()
	 */
	public Vector2f getPosition() {
		// TODO Auto-generated method stub
		return null;
	}

	/* (non-Javadoc)
	 * @see Widget#getPositionX()
	 */
	public float getPositionX() {
		// TODO Auto-generated method stub
		return ( this.x );
	}

	/* (non-Javadoc)
	 * @see Widget#getPositionY()
	 */
	public float getPositionY() {
		// TODO Auto-generated method stub
		return ( this.y );
	}

	/* (non-Javadoc)
	 * @see Widget#getType()
	 */
	public ActionType getType() {
		// TODO Auto-generated method stub
		return null;
	}

	/* (non-Javadoc)
	 * @see Widget#isActive()
	 */
	public boolean isActive()
	{
		// TODO Auto-generated method stub
		return false;
	}

	/* (non-Javadoc)
	 * @see Widget#isLocked()
	 */
	public boolean isLocked()
	{
		// TODO Auto-generated method stub
		return ( this.locked );
	}

	/* (non-Javadoc)
	 * @see Widget#resetWidget()
	 */
	public void resetWidget()
	{
		this.bodies[0].set(new Circle(radius), 10);
		this.bodies[0].setPosition(x+radius, y+radius);
		this.bodies[0].adjustVelocity(new Vector2f(0f, 0f));
	}

	/* (non-Javadoc)
	 * @see Widget#setAppliedForce(float)
	 */
	public void setAppliedForce(float power)
	{
		// TODO Auto-generated method stub

	}

	/* (non-Javadoc)
	 * @see Widget#setConnectionPoints(net.phys2d.math.Vector2f[])
	 */
	public void setConnectionPoints(Vector2f[] points) {
		// TODO Auto-generated method stub

	}

	/* (non-Javadoc)
	 * @see Widget#setDescription(java.lang.String)
	 */
	public void setDescription(String desc) {
		// TODO Auto-generated method stub

	}

	/* (non-Javadoc)
	 * @see Widget#setLock(boolean)
	 */
	public void setLock(boolean locked) {
		// TODO Auto-generated method stub
		this.locked = locked;
	}

	/* (non-Javadoc)
	 * @see Widget#setName(java.lang.String)
	 */
	public void setName(String name) {
		// TODO Auto-generated method stub

	}

	/* (non-Javadoc)
	 * @see Widget#setPosition(net.phys2d.math.Vector2f)
	 */
	public void setPosition(Vector2f f) {
		// TODO Auto-generated method stub
		this.x = f.getX ();
		this.y = f.getY ();
		bodies[0].setPosition(x+radius, this.y+radius);
	}

	/* (non-Javadoc)
	 * @see Widget#setPositionX(float)
	 */
	public void setPositionX(float x) {
		// TODO Auto-generated method stub
		this.x = x;
		bodies[0].setPosition(x+radius, this.y+radius);
	}

	/* (non-Javadoc)
	 * @see Widget#setPositionY(float)
	 */
	public void setPositionY(float y)
	{
		// TODO Auto-generated method stub
		this.y = y;
		bodies[0].setPosition(this.x+radius, y+radius);
	}

	/* (non-Javadoc)
	 * @see Widget#setType(java.lang.String)
	 */
	public void setType(String type) {
		// TODO Auto-generated method stub

	}
	
	public float getWidth ()
	{
		return ( this.radius*2);
	}
	
	public float getHeight ()
	{
		return ( this.radius*2);
	}

	public Body[] getBodiesForSimulation()
	{
		return bodies;
	}

	public Joint[] createJointsForSimulation()
	{
		// TODO Auto-generated method stub
		return null;
	}

	public Direction getDirection() {
		// TODO Auto-generated method stub
		return ( Widget.Direction.EAST );
	}

	public Joint[] getJointsForSimulation() {
		// TODO Auto-generated method stub
		return null;
	}

	public void receiveImpulse(Vector2f anchor_point) {
		// TODO Auto-generated method stub
		
	}

	public void rotateClockwise() {
		// TODO Auto-generated method stub
		
	}

	public void rotateCounterClockwise() {
		// TODO Auto-generated method stub
		
	}

	public void setDirection(Direction d) {
		// TODO Auto-generated method stub
		
	}

	public void setType(ActionType type) {
		// TODO Auto-generated method stub
		
	}

	public ImageIcon getIcon() {
		// TODO Auto-generated method stub
		return null;
	}

	public boolean isConnectable() {
		// TODO Auto-generated method stub
		return false;
	}

	public void reactToTouchingBody(CollisionEvent e) {
		// TODO Auto-generated method stub
		
	}
}
