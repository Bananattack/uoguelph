import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Toolkit;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;

import java.util.Map;
import java.util.HashMap;

import javax.swing.JFrame;
import javax.swing.JSplitPane;
import javax.swing.JPanel;
import javax.swing.Timer;

import net.phys2d.math.*;
import net.phys2d.raw.*;
import net.phys2d.raw.shapes.*;

class Demo extends JFrame implements KeyListener
{
	private static final String DEMO_TITLE = "CIS*3750 Assignment 1 Demo";
	private static final Vector2f DEMO_GRAVITY = new Vector2f(0.0f, 9.81f);
	private static final int DEMO_ITERATIONS = 10;
	private static final int MAX_FRAME_GAP = 10;
	public static void main(String[] args)
	{
		Demo demo = new Demo();
	}

	public class WorldPanel extends JPanel
	{
		public void paintComponent(Graphics g)
		{
			trackFunctionCall("paintComponent");
			super.paintComponent(g);
			render(g);
		}
	}

	private boolean isRefreshing;
	private World world;
	private JSplitPane splitPane;
	private JPanel worldPanel;
	private JPanel debugPanel;
	private int updateCount;
	private HashMap<String, Integer> functionCalls;
	public Demo()
	{
		super(DEMO_TITLE);
		world = new World(DEMO_GRAVITY, DEMO_ITERATIONS);
		functionCalls = new HashMap<String, Integer>();
		trackFunctionCall("new Demo");

		worldPanel = new WorldPanel();
		worldPanel.setPreferredSize(new Dimension(400, 480));
		worldPanel.setSize(new Dimension(400, 480));
		worldPanel.validate();
		debugPanel = new JPanel();
		splitPane = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT, worldPanel, debugPanel);
		add(splitPane);

		setSize(640, 480);
		validate();

		Dimension screen = Toolkit.getDefaultToolkit().getScreenSize();
		setLocation((screen.width - getWidth()) / 2, (screen.height - getHeight()) / 2);

		setDefaultCloseOperation(EXIT_ON_CLOSE);
		setVisible(true);
		addKeyListener(this);
		clear();

		int fps = 0;
		long timeLast = System.nanoTime();
		long secondLast = System.nanoTime();
		long frameGap = 0;
		while(true)
		{
			repaint();
			if (System.nanoTime() - secondLast >= 1000000000) // If a second has passed
			{
				secondLast = System.nanoTime();
				dump(secondLast);
			}
			frameGap = Math.min (MAX_FRAME_GAP, (System.nanoTime() / 1000000) - (timeLast / 1000000));
			if (frameGap > 0)
			{
				for(int i = 0; i < frameGap; i++)
				{
					update();
				}
				timeLast = System.nanoTime();
			}
		}
	}

	public void clear()
	{
		trackFunctionCall("clear");
		world.clear();

		Body b;

		b = new StaticBody(new Box(10000.0f, 100.0f));
		b.setPosition(0.0f, 400.0f);
		world.add(b);

		b = new StaticBody(new Box(20.0f, 10000.0f));
		b.setPosition(0.0f, 0.0f);
		world.add(b);

		b = new StaticBody(new Box(20.0f, 10000.0f));
		b.setPosition(400.0f, 0.0f);
		world.add(b);


		/*b = new Body(new Box(200.0f, 40.0f), 5.0f);
		b.setRotation((float) (Math.PI / 4));
		b.setPosition(325.0f, 100.0f);
		world.add(b);*/
	}

	public void keyTyped(KeyEvent e)
	{

	}

	public void keyPressed(KeyEvent e)
	{
	}

	public void keyReleased(KeyEvent e)
	{
		if(e.getKeyCode() == KeyEvent.VK_SPACE)
		{
			dropCube();
		}
	}

	public void dropCube()
	{
		ExplodingSprite cube = new ExplodingSprite("icecube.png", 200.0, -64.0, 128, 128, 20.0f);
		cube.adjustAngularVelocity((float) (2 + Math.random() * 5));
		cube.setWorld(world);
		cube.setMaxVelocity(10.0f, 50.0f);
		world.add(cube);
	}

	public void update()
	{
		trackFunctionCall("update");
		world.step();
		BodyList bodies = world.getBodies();
		for (int i = 0; i < bodies.size(); i++)
		{
			Body body = bodies.get(i);
			if (body instanceof Sprite)
			{
				((Sprite) body).update();
			}
		}
		
	}

	public void trackFunctionCall(String name)
	{
		Integer count = functionCalls.get(name);
		if(count != null)
		{
			functionCalls.put(name, count + 1);
		}
		else
		{
			functionCalls.put(name, 1);
		}
	}

	public void dump(long timestamp)
	{
		System.out.println("Elapsed time: " + ((double) timestamp / 1000000000.0) + " seconds.");
		Integer fps = functionCalls.get("render");
		if(fps != null)
		{
			System.out.println("FPS: " + fps.toString() + ".");
		}
		else
		{
			System.out.println("FPS: 0.");
		}

		System.out.println("Bodies: " + world.getBodies().size());
		
		System.out.println("Function calls:");
		for(Map.Entry<String, Integer> pair : functionCalls.entrySet())
		{
			System.out.println("\t" + pair.getKey() + "(): " + pair.getValue().toString() + " call(s).");
		}
		functionCalls.clear();
	}

	public void render(Graphics g)
	{
		trackFunctionCall("render");
		BodyList bodies = world.getBodies();
		for (int i = 0; i < bodies.size(); i++)
		{
			Body body = bodies.get(i);
			Shape shape = body.getShape();
			if (body instanceof Sprite)
			{
				((Sprite) body).render(g);
			}
			else if (shape instanceof Box)
			{
				renderBox(g, body, (Box) shape);
			}
		}
	}

	public void renderBox(Graphics g, Body body, Box box)
	{
		trackFunctionCall("renderBox");
		Vector2f[] points = box.getPoints(body.getPosition(), body.getRotation());
		int[] pointX = new int[points.length];
		int[] pointY = new int[points.length];

		for (int i = 0; i < points.length; i++)
		{
			pointX[i] = (int) points[i].x;
			pointY[i] = (int) points[i].y;
		}
		
		g.setColor(Color.black);
		g.drawPolygon(pointX, pointY, points.length);
		g.setColor(new Color(0, 0, 255, 64));
		g.fillPolygon(pointX, pointY, points.length);
	}
}
