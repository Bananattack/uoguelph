/*
 * ObjectDrawer.java
 * 
 * Written by Filip Anguelov
 * ID: 0223807
 * E-mail: fanguelo@uoguelph.ca
 * 
 * A class to draw phys2d shapes on the screen using the java.awt Graphics2D
 * 
 * Inspiration for using Graphics2D to plot the phys2d shapes was derived from the
 * phys2d examples found at:
 * 
 * Author: Kevin Glass
 * URL: http://www.cokeandcode.com/phys2d/source/builds/src060408.zip
 * 
 * Additionally drawing methods are also described in a tutorial at:
 * 
 * Author: wormspy
 * URL: http://code.google.com/p/cyber2d/wiki/p2dTutorial
 * 
 */

package Widgets;

import java.awt.Color;
import java.awt.Font;
import java.awt.Graphics2D;
import java.awt.RenderingHints;

import net.phys2d.raw.Body;
import net.phys2d.raw.shapes.Box;
import net.phys2d.raw.shapes.Circle;
import net.phys2d.raw.shapes.Polygon;

// TODO: Auto-generated Javadoc
/**
 * The Class ObjectDrawer.
 * 
 * @author Filip Anguelov
 */
public class ObjectDrawer
{

	/** The height. */
	private int height;
	
	/** The width. */
	private int width;

	/** The drawer. */
	private Graphics2D drawer;


	/**
	 * Instantiates a new object drawer.
	 */
	public ObjectDrawer()
	{

	}

	/**
	 * This initializes the class passing in the Graphics2D pointer and
	 * the dimensions of the window that is to be painted.
	 * 
	 * @param g The Graphics2D object
	 * @param width the width
	 * @param height the height
	 */
	public ObjectDrawer( Graphics2D g, int width, int height )
	{

		this.drawer = g;

		/* This method makes use of antialiasing available in the Graphics2D API and its was derived from:
		 * 
		 * Author: Bill Day, JavaWorld.com, 08/01/98
		 * URL: http://www.javaworld.com/javaworld/jw-08-1998/jw-08-media.html
		 */
		this.drawer.setRenderingHints(new RenderingHints(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON));

		this.width = width;
		this.height = height;

	}


	/**
	 * Sets the graphics.
	 * 
	 * @param g The new graphics object
	 */
	public void setGraphics( Graphics2D g )
	{
		this.drawer = g;
		
		this.drawer.setRenderingHints(new RenderingHints(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON));
	}
	
	/**
	 * This sets the foreground color.
	 * 
	 * @param c The Color for the foreground objects
	 */
	public void setColor( Color c )
	{
		this.drawer.setColor(c);
		this.drawer.setBackground(Color.white);

	}

	/**
	 * This releases the graphics objects that were created in memory after
	 * the demo quits
	 * 
	 * Source:
	 * 
	 * Author: Jim Graham, Wed, 12 Jan 2000
	 * URL: http://www.mail-archive.com/java2d-interest@capra.eng.sun.com/msg00896.html
	 */
	public void dump()
	{
		this.drawer.dispose();
	}

	/**
	 * This function is called upon to redraw a rectangular box
	 * in order to remove the traces (trails) of the other objects
	 * on the screen. Sort of like an eraser...
	 * 
	 * @param c The Color for the foreground objects
	 */
	public void repaintScreen( Color c )
	{
		this.drawer.setColor(c);
		this.drawer.fillRect(0, 0, this.width, this.height);

	}

	/**
	 * This function draws a Box using the Graphics2D's
	 * draw polygon function.
	 * 
	 * @param body Takes in a phys2d Body object.
	 */
	public void drawBox( Body body )
	{

		Box box = ( Box ) body.getShape();

		int i = 0;
		int k = 3;

		/* Initiate arrays to hold the x and y coordinates of the Object */
		int x[] = new int[4];
		int y[] = new int[4];

		/* go through the 4 sides of the box connecting the points to each other
		 * that is arrange them in the array so that one side connects to the next
		 * and the final to the start
		 * 
		 * Author: wormspy
		 * URL: http://code.google.com/p/cyber2d/wiki/p2dTutorial
		 * 
		 */
		while ( i < 4 )
		{
			/* side 0 to side 3 side 1 to side 2 side 2 to side 3 side 3 to 0 */
			x[i] = ( int ) box.getPoints(body.getPosition(), body.getRotation())[i].x;
			y[k] = ( int ) box.getPoints(body.getPosition(), body.getRotation())[k].y;

			k=i;
			i++;
		}

		/* draw the shape */
		this.drawer.drawPolygon(x, y, 4);

	}


	/**
	 * This is the same function as the Box but it does not
	 * have a limit on the number of sides. Therefore any polygon can
	 * be drawn given the points on the object
	 * 
	 * @param body Takes in a phys2d Body object.
	 */
	public void drawPoly( Body body )
	{

		Polygon poly = ( Polygon ) body.getShape();

		/* get the number of sides */
		int size = poly.getVertices().length;

		int i = 0;
		int k = size - 1;

		/* initialize arrays based on length of the number of sides sent in */
		int x[] = new int[size];
		int y[] = new int[size];

		/* See: http://www.cokeandcode.com/phys2d/source/javadoc/net/phys2d/raw/shapes/Polygon.html */
		while ( i < size )
		{

			x[i] = ( int ) poly.getVertices(body.getPosition(), body.getRotation())[i].x; 
			y[k] = ( int ) poly.getVertices(body.getPosition(), body.getRotation())[k].y;

			k=i;
			i++;
		}

		this.drawer.drawPolygon(x, y, size);

	}

	/**
	 * This function graws an Oval based on the cooridantes of the circle object.
	 * 
	 * @param body Takes in a phys2d Body object.
	 */
	public void drawCircle( Body body )
	{
		Circle circ = ( Circle ) body.getShape();

		this.grapher( ( int ) ( body.getPosition().getX() - circ.getRadius() ), ( int ) ( body.getPosition().getY() - circ.getRadius() ), (int) circ.getRadius());

	}

	/**
	 * This function Draws text to the screen
	 * 
	 * Author: Sun Inc.
	 * URL: http://java.sun.com/j2se/1.5.0/docs/api/java/awt/Graphics.html#drawString(java.lang.String,%20int,%20int)
	 * 
	 * @param text The text to draw
	 * @param x The x position of the upper left corner
	 * @param y The y position of the upper left corner
	 * @param c The color
	 */
	public void drawText( String text, int x, int y, Color c)
	{
		this.drawer.setColor(c);

		/* set the style and size of the font
		 * Source:
		 * 
		 * Author: Sun Inc.
		 * URL: http://java.sun.com/j2se/1.5.0/docs/api/java/awt/Font.html
		 */
		Font font = new Font( "sansserif", Font.TRUETYPE_FONT, 9 );
		this.drawer.setFont( font );
		this.drawer.drawString(text, x, y);

	}


	/**
	 * This function draws the oval.
	 * 
	 * @param x The x position of the circle
	 * @param y The y position of the circle
	 * @param radius The radius of the Circle
	 */
	private void grapher(int x, int y, int radius)
	{
		this.drawer.setColor(Color.red);
		this.drawer.fillOval(x, y, radius*2, radius*2);

	}
}
