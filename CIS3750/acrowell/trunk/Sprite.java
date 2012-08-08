import java.awt.Color;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.image.BufferedImage;
import java.awt.geom.AffineTransform;

import java.io.*;

import javax.imageio.*;

import net.phys2d.math.*;
import net.phys2d.raw.*;
import net.phys2d.raw.shapes.*;

public class Sprite extends Body
{
	private BufferedImage image;
	private World world;
	// Creates an image and slaps it onto a shape of some sort.
	public Sprite(String filename, double x, double y, double width, double height, double mass)
	{
		super(new Box((float) width, (float) height), (float) mass);
		setPosition(x, y);
		try
		{
		    image = ImageIO.read(new File(filename));
		}
		catch (IOException e)
		{
		    System.err.println(e);
		}
	}

	// The lack of individual x/y getting routines annoys me.
	public double getX()
	{
		return getPosition().getX();
	}

	public double getY()
	{
		return getPosition().getY();
	}

	// The fact that phys2d doesn't use double coordinates just ANNOYS me.
	// So, I'm overloading this.
	public void setPosition(double x, double y)
	{
		setPosition((float) x, (float) y);
	}

	public Box getHitBox()
	{
		return (Box) getShape();
	}

	public void setHitBox(double width, double height)
	{
		super.setShape(new Box((float) width, (float) height));
	}

	public World getWorld()
	{
		return world;
	}

	public void setWorld(World world)
	{
		this.world = world;
	}

	public void render(Graphics g)
	{
		Graphics2D screen = (Graphics2D) g;
		AffineTransform oldTransform = screen.getTransform();
		ROVector2f position = getPosition();
		ROVector2f size = getHitBox().getSize();
		double widthRatio = (double) size.getX() / (double) image.getWidth();
		double heightRatio = (double) size.getY() / (double) image.getHeight();


		AffineTransform affine = new AffineTransform();
		affine.translate(position.getX(), position.getY());
		affine.scale(widthRatio, heightRatio);
		affine.rotate(getRotation());
		affine.translate((double) -image.getWidth() / 2.0, (double) -image.getHeight() / 2.0);

		screen.transform(affine);
        
		screen.drawImage(image, 0, 0, null);

		screen.setTransform(oldTransform);

		//renderBox(g);
	}

	public void renderBox(Graphics g)
	{
		Box box = (Box) getShape();
		Vector2f[] points = box.getPoints(getPosition(), getRotation());
		int[] pointX = new int[points.length];
		int[] pointY = new int[points.length];

		for (int i = 0; i < points.length; i++)
		{
			pointX[i] = (int) points[i].x;
			pointY[i] = (int) points[i].y;
		}
		
		g.setColor(Color.black);
		g.drawPolygon(pointX, pointY, points.length);
		g.setColor(new Color(0, 255, 0, 64));
		g.fillPolygon(pointX, pointY, points.length);
	}

	public void update()
	{

	}
}
