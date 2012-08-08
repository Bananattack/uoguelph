package Widgets;

import net.phys2d.math.*;
import net.phys2d.raw.*;
import net.phys2d.raw.shapes.*;

import java.awt.Graphics2D;
import java.awt.Color;
import java.awt.image.BufferedImage;
import java.awt.geom.AffineTransform;

import java.io.*;

import java.util.*;

import javax.swing.ImageIcon;
import javax.imageio.*;

/**
 * Happy Magical Rainbow Bee!
 * A super hyperactive cheerful bee that constantly bounces back and forth between walls.
 * @author Andrew G. Crowell
 */
public class HappyMagicalRainbowBee implements Widget
{
	public static final String NAME = "Happy Magical Rainbow Bee";
	public static final String DESCRIPTION = "A super hyperactive cheerful bee that constantly bounces back and forth between walls.";
	public static final float MASS = 2.0f;
	public static final float WIDTH = 43;
	public static final float HEIGHT = 37;
	public static final String IMAGE_PREFIX = HappyMagicalRainbowBee.class.getResource("./").getFile().toString() + "buzz";
	public static final int IMAGE_COUNT = 2;
	public static final int CENTISECOND = 10000000;
	public static final float MOVEMENT_SPEED = 1500.f;

	// Optimization for many of the same sprite image
	private static ArrayList<BufferedImage> frames = null;

	/**
	 * A static method for initialing all the images associated with this widget
	 */
	private static void loadImages()
	{
		// Already in cache, save us from file IO time.
		if(frames != null)
		{
			return;
		}

		// Okay, it's not in cache, load the images.
		try
		{
			frames = new ArrayList<BufferedImage>();
			for(int i = 0; i < 2; i++)
			{
				System.out.println(IMAGE_PREFIX);
				frames.add(fixTransparency(ImageIO.read(new File(IMAGE_PREFIX + (i + 1) + ".png"))));
			}
		}
		catch (IOException e)
		{
		    throw new RuntimeException(e);
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
	 * A static method for replacing death magenta with transparent pixels.
	 * @param canvas the image to use
	 * @return a copy of the image with the colors fixed
	 */
	public static BufferedImage fixTransparency(BufferedImage canvas)
	{
		return replaceColor(canvas, new Color(255, 0, 255, 255), new Color(0, 0, 0, 0));
	}

	/**
	 * A static method for replacing one ARGB color with another in a picture.
	 * Adapted from Trouser Engine, which is an engine I made in grade 12 of high school.
	 * You can find it here: http://ovk.verge-rpg.com/Trouser%20Engine.zip
	 * @param canvas the image to use
	 * @param colorFind the color to find
	 * @param colorReplace the color to replace with
	 * @return a copy of the image with the colors replaced
	 */
	public static BufferedImage replaceColor(BufferedImage canvas, Color colorFind, Color colorReplace)
	{
		int w = canvas.getWidth();
		int h = canvas.getHeight();
		int find = createColorWithARGB(colorFind);
		int replace = createColorWithARGB(colorReplace);

		BufferedImage copy = new BufferedImage(w, h, BufferedImage.TYPE_INT_ARGB);

		for (int x = 0; x < w; x++)
		{
			for (int y = 0; y < h; y++)
			{
				int c = canvas.getRGB(x, y);
				if (c == find)
				{
					copy.setRGB(x, y, replace);
				}
				else
				{
					copy.setRGB(x, y, c);
				}
			}
		}
		return copy;
	}

	/**
	 * A static method for creating an ARGB color index from a java.awt.Color.
	 * Adapted from Trouser Engine, which is an engine I made in grade 12 of high school.
	 * You can find it here: http://ovk.verge-rpg.com/Trouser%20Engine.zip
	 * @param c a color to use
	 * @return an ARGB color index
	 */
	public static int createColorWithARGB(Color c)
	{
		return c.getAlpha() << 24 | c.getRed() << 16 | c.getGreen() << 8 | c.getBlue();
	}


	private String name = "Box of Awesome";
	private String description = "It's a box, and it's <i>awesome</i>.";
	private ActionType actionType = ActionType.IMPACT;
	private Direction direction = Direction.EAST;
	private Direction currentDirection = Direction.EAST;
	private float appliedForce = 1.0f;
	private Vector2f position = null;
	private Box shape;
	private Body body = null;
	private boolean locked = false;
	private boolean reset = false;
	private boolean active = false;
	private boolean collided = true;
	private ImageIcon icon = null;
	private BufferedImage image = null;
	private long timestamp = 0;
	private int bounces = 0;
	/**
	 * Creates a new bee widget.
	 */
	public HappyMagicalRainbowBee()
	{
		shape = new Box(WIDTH, HEIGHT);
		body = new Body(shape, MASS);
		loadImages();
		icon = new ImageIcon();
		icon.setImage(frames.get(0));
		setPosition(new Vector2f(0,0));
	}


	/**
	 * Set the position of the widget.
	 * @param f  The x,y coordinates within the
	 * container.
	 */
	public void setPosition(Vector2f f)
	{
		body.setPosition(f.x + WIDTH / 2, f.y + HEIGHT /2);
		position = f;
	}

	/**
	 * Set the x-coordinate of the widget.
	 * @param f  The x coordinate within the container.
	 */
	public void setPositionX(float x)
	{
		setPosition(new Vector2f(x, getPositionY()));
	}

	/**
	 * Set the y-coordinate of the widget.
	 * @param f  The y coordinate within the container.
	 */
	public void setPositionY(float y)
	{
		setPosition(new Vector2f(getPositionX(), y));
	}

	/**
	 * Get the position of the widget.
	 * @return   The x,y coordinates within the container.
	 */
	public Vector2f getPosition()
	{
		return new Vector2f(body.getPosition());
	}

	/**
	 * Get the x-coordinate of the widget.
	 * @return   The x coordinate within the container.
	 */
	public float getPositionX()
	{
		return getPosition().getX();
	}

	/**
	 * Get the x-coordinate of the widget.
	 * @return   The x coordinate within the container.
	 */
	public float getPositionY()
	{
		return getPosition().getY();
	}

	/**
     * Draws the widget.
	 * @param g  The object that draws things.
	 */
	public void draw(Graphics2D screen)
	{
		// Animate!
		if (System.nanoTime() - timestamp >= CENTISECOND * 50)
		{
			timestamp = System.nanoTime();
			if(image == frames.get(0))
			{
				image = frames.get(1);
			}
			else
			{
				image = frames.get(0);
			}
		}
		ROVector2f position = body.getPosition();
		float angle = body.getRotation();
		boolean hflip = false;
		boolean vflip = false;
		if (currentDirection == Direction.WEST)
		{
			hflip = false;
		}
		else if (currentDirection == Direction.EAST)
		{
			hflip = true;
		}
		else if (currentDirection == Direction.NORTH)
		{
			angle += Math.PI / 2;
			vflip = false;
		}
		else if (currentDirection == Direction.SOUTH)
		{
			angle += Math.PI / 2;
			vflip = true;
		}
		drawImage(position.getX(), position.getY(),
				getWidth(), getHeight(), angle,
				hflip, vflip, image, screen);

	}

	/**
	 * Activates the widget.
	 */
	public void activateWidget ()
	{
		// Enforce that reset is called at least once before activation.
		if(!reset)
		{
			return;
		}
		// Make active.
		active = true;
		body.setEnabled(true);
	}

	/**
	 * Resets the widget to start state.
	 */
	public void resetWidget()
	{
		// Reset has been made, so we CAN activate this later.
		reset = true;
		// Make inactive
		active = false;
		// Not collided.
		collided = false;

		bounces = 0;
		currentDirection = direction;
		image = frames.get(0);
		body.setEnabled(false);
		// Move to initial position.
		body.set(shape, MASS);
		body.setPosition(position.x + WIDTH / 2, position.y + HEIGHT /2);

		// Make sure the gameplay didn't mess with any initial properties.
		body.setCanRest(true);
		body.setDamping(0.0f);
		body.setFriction(0.01f);
		body.setGravityEffected(false);
		body.setIsResting(true);
		body.setMoveable(true);
		body.setRestitution(1.5f);
		body.setRotatable(true);
		body.setRotation(0.0f);
		body.setRotDamping(0.0f);
		body.setMaxVelocity(50f, 50f);
		body.setForce(-body.getForce().getX(), -body.getForce().getY());
		if(currentDirection == Direction.WEST)
		{
			body.setForce(-MOVEMENT_SPEED, 0);
		}
		else if(currentDirection == Direction.EAST)
		{
			body.setForce(MOVEMENT_SPEED, 0);
		}
		if(currentDirection == Direction.NORTH)
		{
			body.setForce(0, -MOVEMENT_SPEED);
		}
		else if(currentDirection == Direction.SOUTH)
		{
			body.setForce(0, MOVEMENT_SPEED);
		}
		body.adjustVelocity(new Vector2f(-body.getVelocity().getX(), -body.getVelocity().getY()));
		body.adjustAngularVelocity(-body.getAngularVelocity());
	}

	/**
	 * Rotates the Widget 90 Degrees in a
	 * clockwise direction.
	 *
	 * Here is a compass, with numberings showing the order of rotation:
	 *
	 *        NORTH 2
	 *          |
	 * WEST 1 - o - EAST 3
	 *          |
	 *        SOUTH 4
	 *
	 *
	 * In other words, this rotation should under "conventional" circumstances go:
	 * WEST -> NORTH -> EAST -> SOUTH -> WEST -> ...
	 *
	 * Note: For widgets that may only be flipped horizontally or vertically,
	 * you could have it do the following:
	 * NORTH -> SOUTH -> NORTH -> ...
	 * or WEST -> EAST -> WEST -> ...
	 * and for widgets with no notion of "direction", do nothing.
	 */
	public void rotateClockwise()
	{
		if(direction == Direction.WEST)
		{
			currentDirection = direction = Direction.NORTH;
		}
		else if(direction == Direction.NORTH)
		{
			currentDirection = direction = Direction.EAST;
		}
		else if(direction == Direction.EAST)
		{
			currentDirection = direction = Direction.SOUTH;
		}
		else if(direction == Direction.SOUTH)
		{
			currentDirection = direction = Direction.WEST;
		}
	}

	/**
	 * Rotates the Widget 90 Degrees in a
	 * counter-clockwise direction.
	 *
	 * Here is a compass, with numberings showing the order of rotation:
	 *
	 *        NORTH 4
	 *          |
	 * WEST 1 - o - EAST 3
	 *          |
	 *        SOUTH 2
	 *
	 * In other words, this rotation should under "conventional" circumstances go:
	 * WEST -> SOUTH -> EAST -> NORTH -> WEST -> ...
	 *
	 * Note: For widgets that may only be flipped horizontally or vertically,
	 * you could have it do the following:
	 * NORTH -> SOUTH -> NORTH -> ...
	 * or WEST -> EAST -> WEST -> ...
	 * and for widgets with no notion of "direction", do nothing.
	 *
	 */
	public void rotateCounterClockwise ()
	{
		if(direction == Direction.WEST)
		{
			currentDirection = direction = Direction.SOUTH;
		}
		else if(direction == Direction.SOUTH)
		{
			currentDirection = direction = Direction.EAST;
		}
		else if(direction == Direction.EAST)
		{
			currentDirection = direction = Direction.NORTH;
		}
		else if(direction == Direction.NORTH)
		{
			currentDirection = direction = Direction.WEST;
		}
	}

	/**
	 * Manually set the direction of which
	 * the Widget is facing.
	 *
	 * Note: For widgets that may only be flipped horizontally or vertically
	 * (or don't have a notion of "direction") the widget can ignore the request.
	 * This means that no change to direction should occur in the case of an invalid direction for the widget.
	 *
	 * Basically, depending on the widget, setDirection may not do anything when supplied a direction.
	 *
	 * @param d Direction to rotate the Widget
	 */
	public void setDirection(Direction d)
	{
		currentDirection = direction = d;
	}

	/**
	 * Gets the direction in which the Widget is currently facing
	 * @return The direction that the widget is facing.
	 */
	public Direction getDirection ()
	{
		return direction;
	}

	/**
	 * Determines if the widget is active.
	 * @return whether or not the widget is active.
	 */
	public boolean isActive()
	{
		return active;
	}

	/**
	 * Gets the name of the widget.
	 * @return The name of the widget.
	 */
	public String getName ()
	{
		return NAME;
	}

	/**
	 * Gets the type of the widget. This should hopefully
	 * be part of ActionType.
	 * @return The ActionType that the widget is categorized as.
	 */
	public ActionType getType()
	{
		return actionType;
	}

	/**
	 * What happens with the widget when another widget touches it.
	 * @param other_body The body that touched it.
	 * @param f The x,y coordinate that is the contact point.
	 */
	public void reactToTouchingBody(CollisionEvent e)
	{
		float speed = MOVEMENT_SPEED + (MOVEMENT_SPEED * bounces * 0.05f);

		if(currentDirection == Direction.WEST)
		{
			currentDirection = direction.EAST;
			body.setForce(2 * speed, 0);
		}
		else if(currentDirection == Direction.EAST)
		{
			currentDirection = Direction.WEST;
			body.setForce(-2 * speed, 0);
		}
		else if(currentDirection == Direction.NORTH)
		{
			currentDirection = Direction.SOUTH;
			body.setForce(0, 2 * speed);
		}
		else if(currentDirection == Direction.SOUTH)
		{
			currentDirection = direction.NORTH;
			body.setForce(0, -2 * speed);
		}
		bounces++;
	}

	/**
	 * This widget is not connectable.
	 * @return false
	 */
	public boolean isConnectable()
	{
		return false;
	}

	/**
	 * Sets the points where joints will connect to this widget.
	 * @param points The points within the widget that designate
	 * where joints connect to.
	 */
	public void setConnectionPoints(Vector2f [] points)
	{
	}

	/**
	 * Whether or not the widget is locked and cannot be moved.
	 * @return If the widget is locked (true) or not (false).
	 */
	public boolean isLocked()
	{
		return locked;
	}

	/**
	 * Sets the lock state of the widget.
	 * @param locked If the widget is to be locked (true) or
	 * not (false).
	 */
	public void setLock(boolean locked)
	{
		this.locked = locked;
	}

	/**
	 * The point that a joint is attempting to connect to the
	 * widget. What is returned is the closest connection point.
	 * @param point The x,y coordinates where the joint is attempting to
	 * connected to.
	 * @return The connection point that the joint needs to be connected to.
	 */
	public Vector2f attachJoint (Vector2f point)
	{
		return null;
	}

	/**
	 * Constructs an array Bodies object for use in Phys2D engine.
	 * @return A body object.
	 */
	public Body[] getBodiesForSimulation ()
	{
		return new Body[] { body };
	}

	/**
	 * Constructs an array Joints for use in Phys2D engine.
	 * @return A joint object.
	 */
	public Joint[] getJointsForSimulation ()
	{
		return new Joint[] {};
	}

	/**
	 * Remote widget activation based from a connected Joint.
	 * @param anchor_point The x,y coordinates of the connected Joint.
	 */
	public void receiveImpulse(Vector2f anchor_point)
	{

	}

	/**
	 * Gets the boundary of the widget, should be used as a way
	 * to detect overlapping widgets.
	 * @return The four corners x,y coordinates.
	 */
	public Vector2f[] getBoundary()
	{
		Box box = (Box) body.getShape();

		Vector2f[] bounds = new Vector2f[4];
		bounds[0] = new Vector2f(position.x, position.y);
		bounds[1] = new Vector2f(position.x + WIDTH, position.y);
		bounds[2] = new Vector2f(position.x + WIDTH, position.y + HEIGHT);
		bounds[3] = new Vector2f(position.x, position.y + HEIGHT);
		return bounds;
	}

	 /**
	  * Retrieves the description of the widget.
	  * @return The small blurb describing what the widget does.
	  */
	 public String getDescription ()
	 {
	 	return DESCRIPTION;
	 }

	 /**
	  * Retrieves the width of the widget based on the specified boundary.
	  * @return The width of the widget (based on the boundary).
	  */
	 private float getWidth ()
	 {
	 	return ((Box) body.getShape()).getSize().getX();
	 }

	 /**
	  * Retrieves the height of the widget based on the specified boundary.
	  * @return The height of the widget (based on the boundary).
	  */
	 private float getHeight ()
	 {
	 	return ((Box) body.getShape()).getSize().getY();
	 }

	 /**
	  * Retrieves the icon for the widget
	  * @return The icon for the widget.
	  */
	 public ImageIcon getIcon()
	 {
	 	return icon;
	 }
}
