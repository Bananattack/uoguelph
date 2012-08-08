package tim;



import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import net.phys2d.math.Vector2f;
import net.phys2d.raw.Body;
import net.phys2d.raw.Joint;
import net.phys2d.raw.World;
import net.phys2d.raw.strategies.QuadSpaceStrategy;

public class GameEngine
{
	/** Default settings for the World. */
	private static final Vector2f WORLD_GRAVITY = new Vector2f(0.0f, 10f);
	private static final int WORLD_ITERATIONS = 10;
	
	/** the air pressure, changeable */
	private int userAirPressure = ( int )WORLD_GRAVITY.getX();

	/** the gravity, changable */
	private int userGravity = ( int ) WORLD_GRAVITY.getY();
	
	/** The phys2d world. */
	private World world = null;
	
	private boolean isRunning = false;
	
	/** Timer stuff */
	private long startTime = System.nanoTime();
	private long timeLast = startTime;
	private long frameGap = 0;
	
	/* the panel the engine must update */
	private GameController controller = null;
	
	public GameEngine(GameController c)
	{
		/* Phil (2008-11-02): Passing in the vector w/o new allows phys2d to change our static globals which made
		 * it impossible to restore the original values.
		 */
		//this.world = new World( new Vector2f ( WORLD_GRAVITY.getX(), WORLD_GRAVITY.getY() ), WORLD_ITERATIONS);
		this.world =  new World(new Vector2f( WORLD_GRAVITY.getX(), WORLD_GRAVITY.getY() ), 30, new QuadSpaceStrategy(60, 15));
		controller = c;
		
		javax.swing.Timer thread = new javax.swing.Timer(1,
				new ActionListener()
		{
			public void actionPerformed(ActionEvent e)
			{
				run();
			}
		});
		thread.start();
		
		//world.addListener(this);
	}
	
	public void addObject( Body body )
	{
		this.world.add( body );
	}
	
	public void addObject( Joint joint )
	{
		this.world.add( joint );
	}

	private void run( )
	{	  
		frameGap = (System.nanoTime() / 1000000) - (timeLast / 1000000);
		try
		{
			controller.render( );
		if (frameGap > 0)
		{
			if( this.isRunning )
			{
				
				for(int i = 0; i < frameGap; i++)
				{
					
					this.world.step();
					
				}
			}
			timeLast = System.nanoTime();
		}
		}catch ( NullPointerException e) {}
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

	public void setRunning( boolean b )
	{
		this.isRunning = b;
		
	}
	
	public void resetWorld()
	{
		this.world.clear();
	}
	
	//public void redraw()
	//{
		//this.gamePanel.repaint();
	//}

}
