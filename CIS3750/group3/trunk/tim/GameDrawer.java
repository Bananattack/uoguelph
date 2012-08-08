/**
 *
 */
package tim;

import java.awt.AlphaComposite;
import java.awt.Color;
import java.awt.Composite;
import java.awt.Font;
import java.awt.Graphics2D;
import java.awt.Point;
import java.awt.RenderingHints;
import java.awt.geom.AffineTransform;
import java.awt.geom.Rectangle2D;
import java.awt.image.BufferedImage;

import java.io.*;

import java.util.ArrayList;

import javax.imageio.*;

import net.phys2d.math.Vector2f;

import Widgets.Widget;

// TODO: Auto-generated Javadoc
/**
 * The Class GameDrawer.
 * 
 * @author epicbard
 */
public class GameDrawer
{
	
	/** The good placement color. */
	private static Color goodPlacementColor = new Color(0, 255, 0, 127);
	
	/** The bad placement color. */
	private static Color badPlacementColor = new Color(255, 0, 0, 127);
	
	/** The active widget color */
	private static Color activeColor = new Color(255, 127, 0, 127);
	
	/** The controller. */
	private GameController controller = null;
	private BufferedImage lockOverlay;

	/**
	 * Instantiates a new game drawer.
	 * 
	 * @param controller the controller
	 */
	public GameDrawer(GameController controller)
	{
		this.controller = controller;
		try
		{
			lockOverlay = ImageIO.read(new File("resources/icons/slock.png"));
		}
		catch(IOException e)
		{
			lockOverlay = new BufferedImage(1, 1, BufferedImage.TYPE_INT_ARGB);
			lockOverlay.setRGB(0, 0, 0);
		}
	}

	/**
	 * A static method for drawing an image (adapted from my first assignment)
	 * @param x horizontal position
	 * @param y vertical position
	 * @param w the scaled image width
	 * @param h the scaled image height
	 * @param angle the angle of rotation of the image
	 * @param hflip whether or not to flip the image horizontally
	 * @param vflip whether or not to flip the image vertically
	 * @param canvas the image to draw
	 * @param screen the destination to render to
	 */
	public static void drawImage(double x, double y, double w, double h, double angle,
		boolean hflip, boolean vflip, BufferedImage canvas, Graphics2D screen)
	{
		AffineTransform oldTransform = screen.getTransform();
		double widthRatio = (double) w / (double) canvas.getWidth();
		double heightRatio = (double) h / (double) canvas.getHeight();

		AffineTransform affine = new AffineTransform();
		affine.translate(x, y);
		affine.scale(widthRatio, heightRatio);
		affine.scale((hflip ? -1 : 1) * 1.0, (vflip ? -1 : 1) * 1.0);
		affine.rotate(angle);
		affine.translate((double) -canvas.getWidth() / 2.0, (double) -canvas.getHeight() / 2.0);

		screen.transform(affine);

		screen.drawImage(canvas, 0, 0, null);
		screen.setTransform(oldTransform);
	}

	/**
	 * Draws the selected widget to the screen.
	 * 
	 * @param g the g
	 * @param activeWidget the active widget
	 * @param mousePosition the mouse position
	 */
	public void drawSelectedWidget(Graphics2D g, Widget activeWidget, Point mousePosition)
	{
		this.enableAA( g );
		
		if( activeWidget != null && mousePosition != null )
		{
			Color c = null;
			/* Moving something, center on mouse and color depending on placement capability*/
			if(controller.isNewPiece() || controller.isMovingOldPiece())
			{
				/* Phil (2008-10-24): Trying to center widget on mouse */
				activeWidget.setPosition(new Vector2f(mousePosition.x - (int) (activeWidget.getBoundary()[2].getX()
										- activeWidget.getBoundary()[0].getX()) / 2,
									mousePosition.y - (int) (activeWidget.getBoundary()[2].getY()
										- activeWidget.getBoundary()[0].getY()) / 2));
				g.setColor(canPlace(activeWidget)? goodPlacementColor : badPlacementColor);
			}
			// JUST selected, don't center on mouse, because we're not moving anything right now
			// This will draw differently depending on lock status.
			else
			{
				g.setColor(activeColor);
			}
			Vector2f[] bounds = activeWidget.getBoundary();
			int width = (int) (bounds[2].getX() - bounds[0].getX());
			int height = (int) (bounds[2].getY() - bounds[0].getY());
			
			g.fillRect((int) activeWidget.getPositionX(), (int) activeWidget.getPositionY(), width, height);
			drawHud(g, activeWidget);
			drawWidget(g, activeWidget);
		}
	}
	
	private void drawHud( Graphics2D g, Widget widget)
	{
		Vector2f[] bounds = widget.getBoundary();
		int offset = 14;
		//Vector2f[] bounds2 = activeWidget.getBoundary();
		int width = (int) ( bounds[2].getX() - bounds[0].getX() ) + offset;
		int height = (int) ( bounds[2].getY() - bounds[0].getY() ) + offset;
		
		int width_one = width;
		int height_one = height - offset;
		
		int width_two = width - offset;
		int height_two = height;
		
		g.setColor( Color.GREEN );
		g.drawRect( (int) widget.getPositionX() - ( offset / 2 ), (int) widget.getPositionY() - ( offset / 2 ), width, height);
		
		g.setColor( Color.WHITE );
		g.drawRect( (int) widget.getPositionX() - ( offset / 2 ) , (int) widget.getPositionY(), width_one, height_one);
		
		g.setColor( Color.WHITE );
		g.drawRect( (int) widget.getPositionX(), (int) widget.getPositionY() - ( offset / 2 ) , width_two, height_two);
		
		
		
	}

	private void drawWidget(Graphics2D g, Widget widget)
	{
		Vector2f[] bounds = widget.getBoundary();
		int width = (int) (bounds[2].getX() - bounds[0].getX());
		int height = (int) (bounds[2].getY() - bounds[0].getY());

		
		widget.draw(g);
		
		
		
		if(widget.isLocked() && !controller.isRunning() && GameWindow.getInstance().getGameType())
		{
			Composite old = g.getComposite();
			Composite composite = AlphaComposite.getInstance(AlphaComposite.SRC_OVER, 0.5f);
			g.setComposite(composite);
		//	drawImage((int)bounds[0].getX() + width / 2, (int)bounds[0].getY() + height / 2, width, height, 0, false, false, lockOverlay, g);
			
			drawImage((int) bounds[3].getX(), (int) bounds[3].getY() + 10, lockOverlay.getWidth(), lockOverlay.getHeight(), 0, false, false, lockOverlay, g);
			g.setComposite(old);
		}
	}

	/**
	 * Clear screen.
	 * 
	 * @param g the g
	 */
	private void clearScreen(Graphics2D g)
	{
		g.setColor(Color.WHITE);
		g.setClip(new Rectangle2D.Float(0, 0, 500, 500));
	    g.fillRect(0, 0, 500, 500);
	}

	/**
	 * Draw boundary.
	 * 
	 * @param mousePoint the mouse point
	 * @param g the g
	 * @param c the c
	 * @param points the points
	 */
	private void drawBoundary(Point mousePoint, Graphics2D g, Color c, Vector2f[] points)
	{
		this.enableAA( g );

		
		int x = (int) (mousePoint.getX() + points[0].x);
		int y = (int) (mousePoint.getY() + points[0].y);
		int width = (int) (points[2].getX() - points[0].getX());
		int height = (int) (points[2].getY() - points[0].getY());
		g.setColor(c);
		g.fillRect(x, y, width, height);
	}

	/**
	 * Can place.
	 * 
	 * @param w the w
	 * 
	 * @return true, if successful
	 */
	private boolean canPlace(Widget w)
	{

		ArrayList<GamePiece> list = controller.getLevelPieces();

		if (list == null)
		{
			return true;
		}

		GamePiece active = new GamePiece(w);

		for (int i=0; i<list.size(); i++)
		{
			if (list.get(i).collidesWith(active) && list.get(i).getWidget() != w)
			{
				return false;
			}
		}
		return true;
	}

	/**
	 * Render.
	 * 
	 * @param g the g
	 * @param levelWidgets the level widgets
	 * @param selectedPiece the selected piece
	 */
	public void render(Graphics2D g, ArrayList<GamePiece> levelWidgets, Widget selectedPiece)
	{
		this.enableAA( g );
		
		clearScreen( g );

		if(levelWidgets != null)
		{
			for(GamePiece piece : levelWidgets)
			{
				if(piece.getWidget() != selectedPiece)
				{
					g.setColor(Color.BLACK);
					drawWidget(g, piece.getWidget());
				}
				piece.update();
			}
		}
		if(selectedPiece != null)
		{
			drawSelectedWidget(g, selectedPiece, controller.getMousePosition());
		}
	}
	
	/**
	 * Enable Anti Aliasing in the graphics.
	 * 
	 * @param g the Graphics2D object
	 * 
	 * @see Graphics2D
	 * @see RenderingHints
	 */
	private void enableAA( Graphics2D g )
	{
		/* This method makes use of antialiasing available in the Graphics2D API and its was derived from:
		 * 
		 * Author: Bill Day, JavaWorld.com, 08/01/98
		 * URL: http://www.javaworld.com/javaworld/jw-08-1998/jw-08-media.html
		 */
		if( !g.getRenderingHints().containsValue( RenderingHints.VALUE_ANTIALIAS_ON ) )
		{
			g.setRenderingHints(new RenderingHints(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON ));
		}
			
	}
	
	/**
	 * This function Draws text to the screen
	 * 
	 * http://java.sun.com/j2se/1.5.0/docs/api/java/awt/Graphics.html#drawString(java.lang.String,%20int,%20int)
	 * 
	 * @param text The text to draw
	 * @param x x position
	 * @param y y position
	 * @param c the color
	 * @param g the g
	 */
	public void drawText( Graphics2D g, String text, int x, int y, Color c)
	{
		g.setColor(c);

		/* set the style and size of the font
		 * http://java.sun.com/j2se/1.5.0/docs/api/java/awt/Font.html
		 */
		Font font = new Font( "sansserif", Font.TRUETYPE_FONT, 9 );
		g.setFont( font );
		g.drawString(text, x, y);

	}
}
