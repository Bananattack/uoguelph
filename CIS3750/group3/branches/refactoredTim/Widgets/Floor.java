package Widgets;

import java.awt.Color;
import java.awt.Graphics2D;

import javax.swing.ImageIcon;
import java.awt.geom.Rectangle2D;

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
public class Floor implements Widget
{
	Body bod;

	private float x = 0;
	private float y = 0;

	private float width = 100f;
	private float height = 50f;
	
	private Vector2f [] vecBoundaries;
	private boolean locked;
	
	private Body [] bodies;
	/**
	 * 
	 */
	public Floor()
	{
		// TODO Auto-generated constructor stub'
		this.bodies = new Body[1];
		this.bodies[0] = new StaticBody(new Box(width, height));
		bodies[0].setPosition(x+width/2, y+height/2);
		this.bodies[0].setRestitution ( 1f );
	}

	/* (non-Javadoc)
	 * @see Widget#Draw(java.awt.Graphics2D)
	 */
	public void draw(Graphics2D g)
	{
		// TODO Auto-generated method stub
		Vector2f v = (Vector2f)bodies[0].getPosition();
		g.setColor( Color.black );
		g.draw(new Rectangle2D.Float( v.getX() - width/2, v.getY() - height/2, width, height ));
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
	public void anotherBodyTouching(Body other_body, Vector2f f) {
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
	public Vector2f[] getBoundary() {
		// TODO Auto-generated method stub
		Vector2f[] b = new Vector2f[4];
		
		b[0] = new Vector2f(x,y);
		b[1] = new Vector2f(x+width,y);
		b[2] = new Vector2f(x+width,y+height);
		b[3] = new Vector2f(x,y+height);
		return b;
	}

	/* (non-Javadoc)
	 * @see Widget#getDescription()
	 */
	public String getDescription()
	{
		// TODO Auto-generated method stub
		
		String strDesc = "This is a floor.<br > Blah Blah Blah...";
		
		return ( strDesc );
	}

	/* (non-Javadoc)
	 * @see Widget#getName()
	 */
	public String getName()
	{
		// TODO Auto-generated method stub
		return ( "Floor" );
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
		// TODO Auto-generated method stub

	}

	/* (non-Javadoc)
	 * @see Widget#setAppliedForce(float)
	 */
	public void setAppliedForce(float power)
	{
		// TODO Auto-generated method stub

	}


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
		bodies[0].setPosition(f.getX(),f.getY());

	}

	/* (non-Javadoc)
	 * @see Widget#setPositionX(float)
	 */
	public void setPositionX(float x) {
		// TODO Auto-generated method stub
		this.x = x;
		bodies[0].setPosition(x+width/2, y+height/2);
	}

	/* (non-Javadoc)
	 * @see Widget#setPositionY(float)
	 */
	public void setPositionY(float y)
	{
		// TODO Auto-generated method stub
		this.y = y;
		bodies[0].setPosition(x+width/2, y+height/2);
	}

	/* (non-Javadoc)
	 * @see Widget#setType(java.lang.String)
	 */
	public void setType(String type) {
		// TODO Auto-generated method stub

	}
	
	public float getWidth ()
	{
		return ( this.width );
	}
	
	public float getHeight ()
	{
		return ( this.height );
	}

	public Body[] getBodiesForSimulation()
	{
		return ( bodies.clone() );
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
