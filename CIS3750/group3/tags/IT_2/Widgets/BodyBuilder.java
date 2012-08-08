/*
 * BodyBuilder.java
 * 
 * Written by Filip Anguelov
 * ID: 0223807
 * E-mail: fanguelo@uoguelph.ca
 * 
 * Function aiding in the creation of simple phhys2d objects
 * 
 */
package Widgets;


import net.phys2d.math.Vector2f;
import net.phys2d.raw.*;
import net.phys2d.raw.shapes.*;

// TODO: Auto-generated Javadoc
/**
 * The Class BodyBuilder.
 * 
 * @author Filip Anguelov
 *
 */
public class BodyBuilder 
{
	
	/**
	 * Instantiates a new body builder.
	 */
	public BodyBuilder()
	{
		
	}
	
	/**
	 * This function simplifies the process of making a phys2d Body object.
	 * 
	 * @param name The name of the object.
	 * @param size The radius of the circle.
	 * @param x Its x position on the axis
	 * @param y Its y position on the axis
	 * @param weight The mass of the object
	 * @param isStatic If the object is pinned to the world.
	 * 
	 * @return Returns an initialized body object
	 */
	public Body makeCircle( String name, float size, float x, float y, float weight, Boolean isStatic )
	{

		/* create a temp Body to use */
		Body circBody;

		/* create a new circle shape as defined in the phys2d javadoc
		 * 
		 */
		Circle circShape = new Circle(size);

		if ( isStatic )
		{
			circBody = new StaticBody( name, circShape );

		} else {

			circBody = new Body( name, circShape, weight );
		}

		circBody.setPosition( x, y );
		return circBody;
	}
	
	/**
	 * Make box.
	 * 
	 * This function is used to build the Box body for the world and returns a created object
	 * 
	 * Derived from:
	 * 
	 * Author: Kevin Glass
	 * 
	 * URL:
	 * http://www.cokeandcode.com/phys2d/source/javadoc/net/phys2d/raw/Body.html
	 * http://www.cokeandcode.com/phys2d/source/javadoc/net/phys2d/raw/shapes/Box.html
	 * 
	 * @param name Name of the box
	 * @param width Width of the box
	 * @param height Height of the box
	 * @param x x position
	 * @param y y position
	 * @param weight mass
	 * @param isStatic if it is static
	 * 
	 * @return the body
	 */
	public Body makeBox(String name, float width, float height, float x, float y, float weight, Boolean isStatic)
	{
		/* Set needed objects */

		Body body;
		Box aBox = new Box(width, height);

		/* if not static make non static box */
		if ( !isStatic )
		{
			body = new Body(name, aBox, weight);
			body.setPosition(x, y);

		}else{

			body = new StaticBody(name, aBox);
			body.setPosition(x, y);
		}

		return body;
	}
	
	/**
	 * Connect body.
	 * 
	 * This function uses the BasicJoint to connect two objects
	 * 
	 * Derived from:
	 * 
	 * Author: Kevin Glass
	 * URL: http://www.cokeandcode.com/phys2d/source/javadoc/net/phys2d/raw/BasicJoint.html
	 * 
	 * @param a Body A
	 * @param b Body B
	 * 
	 * @return the basic joint
	 */
	public BasicJoint connectBody( Body a, Body b)
	{
		/* joins the two bodies taking one's coordinates */
		BasicJoint basic = new BasicJoint(a, b, new Vector2f(b.getPosition()));
		return basic;
	}
}
