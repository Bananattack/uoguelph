/*
 * GameEngine.java
 *
 * Written by Filip Anguelov
 * ID: 0223807
 * E-mail: fanguelo@uoguelph.ca
 *
 * This class is responsible for running the phys2D world. It is used to run the simulation
 * pausing stopping adding bodies and joints to the world.
 *
 */

package tim;


import Widgets.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.ArrayList;

import net.phys2d.math.Vector2f;
import net.phys2d.raw.Body;
import net.phys2d.raw.CollisionEvent;
import net.phys2d.raw.CollisionListener;
import net.phys2d.raw.Joint;
import net.phys2d.raw.World;
import net.phys2d.raw.strategies.QuadSpaceStrategy;


/**
 * The Class GameEngine.
 * 
 * @author Filip Anguelov
 */
public class GameEngine
{
	
	/** Default settings for the World. */
	private static final Vector2f WORLD_GRAVITY = new Vector2f(0.0f, 10f);
	
	/** The Constant WORLD_ITERATIONS. */
	private static final int WORLD_ITERATIONS = 10;

	/** the air pressure, changeable. */
	private int userAirPressure = ( int )WORLD_GRAVITY.getX();

	/** the gravity, changeable. */
	private int userGravity = ( int ) WORLD_GRAVITY.getY();

	/** The phys2d world. */
	private World world = null;

	/** The is running. */
	private boolean running = false;

	/** Timer stuff. */
	private long startTime = System.nanoTime();
	
	/** The time last. */
	private long timeLast = startTime;
	
	/** The frame gap. */
	private long frameGap = 0;

	/** The controller. */
	private GameController controller = null;

	/**
	 * Instantiates a new game engine.
	 * 
	 * @param c the The Game Controller object
	 * @see GameController
	 */
	public GameEngine(GameController c)
	{
		/* Phil (2008-11-02): Passing in the vector w/o new allows phys2d to change our static globals which made
		 * it impossible to restore the original values.
		 */
		//this.world = new World( new Vector2f ( WORLD_GRAVITY.getX(), WORLD_GRAVITY.getY() ), WORLD_ITERATIONS);
		this.world =  new World(new Vector2f( WORLD_GRAVITY.getX(), WORLD_GRAVITY.getY() ), 30, new QuadSpaceStrategy(60, 15));
		controller = c;

		javax.swing.Timer thread = new javax.swing.Timer(20,
				new ActionListener()
		{
			public void actionPerformed(ActionEvent e)
			{
				run();
			}
		});
		thread.start();

	}

	/**
	 * Adds an object, a body, to the world
	 * for simulation.
	 * 
	 * @param body The Body object to add
	 */
	public void addObject( Body body )
	{
		this.world.add( body );
	}

	/**
	 * Adds an object, a joint, to the world
	 * for simulation.
	 * 
	 * @param joint The Joint object to add
	 */
	public void addObject( Joint joint )
	{
		this.world.add( joint );
	}

	/**
	 * Run.
	 * 
	 * Written by Andrew Crowell
	 * 
	 * This method runs the simulation
	 */
	private void run()
	{
		frameGap = (System.nanoTime() / 1000000) - (timeLast / 1000000);
		try
		{
			controller.render();
			if (frameGap > 0)
			{
				if(running)
				{
					for(int i = 0; i < frameGap; i++)
					{

						this.world.step();
					}
				}
				timeLast = System.nanoTime();
			}
		}
		catch (NullPointerException e)
		{
		}
	}

	/**
	 * Gets the gravity.
	 * 
	 * @return the gravity
	 */
	public int getGravity()
	{
		int gravity = userGravity;
		return gravity;
	}

	/**
	 * Gets the air pressure.
	 * 
	 * @return the air pressure
	 */
	public int getAirPressure()
	{
		int air = userAirPressure;
		return air;

	}

	/**
	 * Sets the gravity.
	 * 
	 * @param grav the new gravity
	 */
	public void setGravity( int grav )
	{
		userGravity = grav;
		world.setGravity( userAirPressure, grav );
	}

	/**
	 * Sets the air pressure.
	 * 
	 * @param air the new air pressure
	 */
	public void setAirPressure( int air )
	{
		userAirPressure = air;
		world.setGravity( air, this.userGravity );
	}

	/**
	 * Gets the default settings.
	 * 
	 * @return the default settings
	 */
	public int[] getDefaultSettings()
	{
		int temp[] = { ( int ) WORLD_GRAVITY.getX(), ( int ) WORLD_GRAVITY.getY() };

		return temp;
	}

	/**
	 * Get whether the engine IS running
	 * 
	 * @return the running flag of the engine.
	 */
	public boolean isRunning()
	{
		return running;
	}

	/**
	 * Tell the engine if it should run.
	 * 
	 * @param b Set to True enables the engine.
	 */
	public void setRunning(boolean running)
	{
		this.running = running;
	}

	/**
	 * Reset world, and clear all objects in it.
	 */
	public void resetWorld()
	{
		this.world.clear();
	}

	/**
	 * Adds the collision listener.
	 * 
	 * @param listener the Collision Listener
	 * @see CollisionListener
	 */
	public void addCollisionListener(CollisionListener listener)
	{
		world.addListener(listener);
	}



	/**
	 * Gets the contacts.
	 * 
	 * @param w the Widget
	 * 
	 * @return the contacts
	 */
	public ArrayList<CollisionEvent> getContacts(Widget w)
	{
		ArrayList<CollisionEvent> events = new ArrayList<CollisionEvent>();
		Body[] bods = w.getBodiesForSimulation();
		for (Body b : bods)
		{
			CollisionEvent[] contacts = world.getContacts(b);
			for(CollisionEvent c : contacts)
			{
				events.add(c);
			}
		}

		return events;
	}
}
