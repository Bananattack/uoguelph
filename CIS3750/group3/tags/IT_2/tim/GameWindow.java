// CIS*3750
// GameWindow.java
// Andy (2008-10-06):
// 	Renamed from WidgetPanelTest.java to GameWindow.
//	GameWindow is a Singleton, because a lot of outside classes will
//	rely on pieces of global information which is less work and less memory overhead
// 	than holding onto the pointer in every object that relies on it.
//	This will hopefully become the main base for running the program.

// Andy (2008-10-01): Refactored class structure to be in package.
package tim;


import java.awt.*;
import java.awt.event.*;

import java.io.*;
import java.util.*;

import Widgets.*;

import javax.swing.*;
import javax.swing.border.*;

import net.phys2d.raw.*;
import net.phys2d.raw.shapes.*;
import net.phys2d.math.*;

// TODO: Auto-generated Javadoc
/**
 * The Class GameWindow.
 */
public class GameWindow extends JFrame implements CollisionListener
{
	/* Phil: can we specify if X is grav or Y??
	 * I am going to assume for now that X is AirPressure and Y gravity */
	/** Default settings for the World. */
	private static final Vector2f WORLD_GRAVITY = new Vector2f(0.0f, 10f);
	private static final int WORLD_ITERATIONS = 10;

	/** The GameWindow singleton instance. */
	private static GameWindow instance = null;

	/** the air pressure, changeable */
	private int userAirPressure = ( int )WORLD_GRAVITY.getX();

	/** the gravity, changable */
	private int userGravity = ( int ) WORLD_GRAVITY.getY();

	/* Phil: Andy can you put your globals on top, I spent like an hour oggling where these were, lol */
	/* Andy: But I did have a convention. I separated static from nonstatic members. But I digress. */
	/** The active widget. */
	private Widget activeWidget = null;

	/** A list of the on-map widgets */
	/* Phil: I think activatedWidgets is more descriptive, no? */
	/* Andy: Maybe level widgets? Because they're the widgets on the level? */
	private ArrayList<Widget> levelWidgets = null;

	/** The phys2d world. */
	private World world = null;

	/** Whether the game is running its level or not. */
	private boolean runningLevel = false;

	/** The visible playfield */
	private PlayfieldPanel playPanel = null;

	/** The toolbar */
	GameToolBar tool = null;

	/** The top menu bar for the game */
	GameMenuBar menuBar = null;

	/** The panel for the widget selection */
	WidgetPanel widgetPanel = null;

	/** Timer stuff */
	long startTime = System.nanoTime();
	long timeLast = startTime;
	long frameGap = 0;

	/** Used for rendering stuff under the mouse */
	Point mousePosition;

	/**
	 * Gets the singleton instance of GameWindow.
	 *
	 * @return the singleton instance of GameWindow
	 */
	public static GameWindow getInstance()
	{
		if(instance == null)
		{
			instance = new GameWindow();
		}
		return instance;
	}


	/**
	 *
	 * Get rid of the extension of a filename, in some case
	 * where it is more or less unnecessary.
	 *
	 * @param filename - The string to read.
	 * @return filename, sans extension.
	 */
	private static String stripExtension(String filename)
	{
		String strippedName = "";

		int pos = filename.lastIndexOf (".");

		if (pos >= 0)
		{
			strippedName = filename.substring(0, pos);
		}
		return strippedName;
	}

	/**
	 * The main method.
	 *
	 * @param args the arguments
	 */
	public static void main(String[] args)
	{
		GameWindow window = GameWindow.getInstance();
	}

	/**
	 * Instantiates a new game window.
	 */
	private GameWindow()
	{
		super();
		setLayout(new BorderLayout());

		world = new World(WORLD_GRAVITY, WORLD_ITERATIONS);
		world.addListener(this);

		levelWidgets = new ArrayList<Widget>();

		tool = new GameToolBar();
		add(tool, BorderLayout.NORTH);

		menuBar = new GameMenuBar();
		/* Phil (2008-10-19): Added Listener */
		menuBar.setActionListener(new GameMenuListener());
		menuBar.setAll();

		widgetPanel = new WidgetPanel();
		initWidgetPanel();

		playPanel = new PlayfieldPanel();

		/* Phil (2008-10-19): Trying to make it look better
		 *
		 * Added a panel that would frame the whole thing.
		 *
		 */
		JPanel frameIt = new JPanel( new BorderLayout() );
		frameIt.setBorder(BorderFactory.createEmptyBorder(0, 5, 5, 5));

		frameIt.add(widgetPanel, BorderLayout.EAST);
		frameIt.add(playPanel, BorderLayout.CENTER);

		/* Phil (2008-10-19): Trying to make it look better
		 *
		 * Added a Panel for game status or what not...
		 *
		 */
		JLabel test = new JLabel("Put something here for the scores or whatever.");
		JPanel statusPanel = new JPanel();
		statusPanel.setBorder(new EtchedBorder());
		statusPanel.add(test);

		frameIt.add(statusPanel, BorderLayout.SOUTH);

		add(frameIt, BorderLayout.CENTER);

		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		setBounds(0, 0, 800, 600);
		setJMenuBar(menuBar);

		/* Phil (2008-10-19): Added a way to center the frame on the screen */
		/* get the screen resolutions */
		Dimension screen = Toolkit.getDefaultToolkit().getScreenSize();
		/* convert the size to integers
		 * subtract from the window size and div by 2
		 * to get the center position of the screen
		 */
		int x = ((int)screen.getWidth() - this.getWidth()) / 2;
		int y = ((int)screen.getHeight() - this.getHeight()) / 2;

		this.setLocation(x, y);

		javax.swing.Timer thread = new javax.swing.Timer(1,
			new ActionListener()
			{
				public void actionPerformed(ActionEvent e)
				{
					processTimer();
				}
			});
		thread.start();

		setVisible(true);
	}

	/** Does the chore of loading all widgets we need into our WidgetPanel */
	void initWidgetPanel()
	{
		/* Darren (2008-10-19): No need to spam buttons now. Lets add in our widgets. */
		/* Andy (2008-10-20): Added reflection so we don't need to DECLARE a new widget, here
		 * but rather load them all by Strings (through directory listing so
		 * you can just paste a widget class in and it'll autoload it). */
		String[] filenames = new File("Widgets/").list(new FilenameFilter() {
									public boolean accept(File dir, String name)
									{
										if(name.endsWith(".class"))
										{
											return true;
										}
										return false;
									}
								});
		for(String filename : filenames)
		{
			try
			{
				Class c = Class.forName("Widgets." + stripExtension(filename));
				Object o = c.getConstructor(new Class[] {}).newInstance(new Object[] {});

				if(o instanceof Widget && o.getClass() != Widget.class)
				{
					System.out.println("Loaded '" + c.getSimpleName() + "'");
					widgetPanel.add(makeAButton((Widget) o));
				}
			}
			catch(Exception e)
			{
			}
		}
	}


	/** The timer step stuff */
	void processTimer()
	{
		playPanel.repaint();
		frameGap = (System.nanoTime() / 1000000) - (timeLast / 1000000);
		if (frameGap > 0)
		{
			if(runningLevel)
			{
				for(int i = 0; i < frameGap; i++)
				{
					world.step();
				}
			}
			timeLast = System.nanoTime();
		}
	}

	/**
	 * Starts running a level
	 */
	void play()
	{
		runningLevel = true;
		widgetPanel.setEnabled(false);

		for(Widget widget : levelWidgets)
		{
			// Resets before activation just to be sure.
			widget.resetWidget();

			widget.activateWidget();
		}
	}

	/**
	 * Stops running a level
	 */
	void stop()
	{
		runningLevel = false;
		widgetPanel.setEnabled(true);
		for(Widget widget : levelWidgets)
		{
			widget.resetWidget();
		}
	}

	/**
     * Returns if the level is currently running.
     * @return whether or not the level is running
     */
	boolean isRunningLevel()
	{
		return runningLevel;
	}

	/**
	 * Gets the active widget.
	 *
	 * @return the active widget
	 */
	public Widget getActiveWidget()
	{
		return activeWidget;
	}

	/**
	 * Sets the active widget.
	 *
	 * @param activeWidget the new active widget
	 */
	public void setActiveWidget(Widget activeWidget)
	{
		this.activeWidget = activeWidget;
	}

	/*
	 * Adds a widget to the world.
	 * @param widget The widget to append to the world's list of processing widgets.
	 */
	public void addWidget(Widget widget)
	{
		widget.resetWidget();
		Body[] bodies = widget.getBodiesForSimulation();
		for(Body body : bodies)
		{
			world.add(body);
		}
		Joint[] joints = widget.getJointsForSimulation();
		if(joints !=  null)
		{
			for(Joint joint : joints)
			{
				world.add(joint);
			}
		}
		levelWidgets.add(widget);
	}

	/*
	 * Clears all widgets from the world
	 */
	public void clear()
	{
		levelWidgets.clear();
		world.clear();
	}

	/**
	 * The handler for when any collision happens in our world.
	 */
	public void collisionOccured(CollisionEvent e)
	{
		for(Widget widget : levelWidgets)
		{
			Body[] widgetBodies = widget.getBodiesForSimulation();
			for(Body body : widgetBodies)
			{
				if(body == e.getBodyA() || body == e.getBodyB())
				{
					widget.reactToTouchingBody(e);
				}
			}
		}
	}

	/**
	 * Draws the widgets to the screen.
	 */
	public void render(Graphics g)
	{
		if(activeWidget != null && mousePosition != null)
		{
			Color c = new Color(0, 255, 0, 127);
			Vector2f[] bounds;

			activeWidget.setPosition(new Vector2f(mousePosition.x, mousePosition.y));
			bounds = activeWidget.getBoundary();
			int width = (int) (bounds[2].getX() - bounds[0].getX());
			int height = (int) (bounds[2].getY() - bounds[0].getY());

			g.setColor(c);
			g.fillRect(mousePosition.x, mousePosition.y, width, height);
			activeWidget.draw((Graphics2D) g);
		}
		for(Widget widget : levelWidgets)
		{
			widget.draw((Graphics2D) g);
		}
	}

	/**
	 * Make a button.
	 *
	 * Phil: I am playing around with the layout and such. I dunno...
	 *
	 * @param w the w
	 *
	 * @return the j panel
	 */
	private JPanel makeAButton( Widget w )
	{
		/* Andy (2008-10-20): Now applying the tooltip to every element. */
		String toolTipText = "<html><h2>" + w.getName() + "</h2>" + w.getDescription();
		Font font = new Font( "Sanserif", Font.TRUETYPE_FONT, 9 );

		/* Want a panel so it would be easier to put the title under the button */
		/* Andy (2008-10-20): changed the variable case.
		 * Only types and constants should be capitalized, in my opinion. */
		JPanel buttonPanel = new JPanel();
		buttonPanel.setLayout(new BoxLayout(buttonPanel, BoxLayout.Y_AXIS));

		/* Setting the label under the button, we get the name from the widget */
		JLabel wLabel = new JLabel("5 x " + w.getName());

		wLabel.setFont( font );

		/* adding a border so things dont look like they are touching the edge */
		wLabel.setBorder(BorderFactory.createEmptyBorder( 0, 2, 0, 2 ) );
		wLabel.setToolTipText(toolTipText);


		/* Making the button from the WidgetPanelButton */
		JButton b = new WidgetPanelButton(w);
		b.setToolTipText(toolTipText);

		/* when the mouse is over the button/widget you get a tool tip with the description */
		buttonPanel.setToolTipText(toolTipText);
		buttonPanel.add( b );

		/* clearly separate button from widget */
		buttonPanel.add(new JSeparator(JSeparator.HORIZONTAL));

		/* add the label */
		buttonPanel.add( wLabel );

		/* add a border around the panel to differentiate widgets */
		buttonPanel.setBorder( BorderFactory.createLineBorder(Color.GRAY) );

		buttonPanel.setPreferredSize( new Dimension (100, 100) );

		return buttonPanel;

	}

	/* Phil (2008-10-21): Added getter/setters for the  gravity air pressure so that the changer can do something
	 * I don't know if you want them here or not.
	 */

	/**
	 * Gets the gravity.
	 *
	 * @return the gravity
	 */
	public int getGravity()
	{
		int gravity = this.userGravity;
		return gravity;
	}

	/**
	 * Gets the air pressure.
	 *
	 * @return the air pressure
	 */
	public int getAirPressure()
	{
		int air = this.userAirPressure;
		return air;

	}

	/**
	 * Sets the gravity.
	 *
	 * @param grav the new gravity
	 */
	public void setGravity( int grav )
	{
		this.userGravity = grav;
	}

	/**
	 * Sets the air pressure.
	 *
	 * @param air the new air pressure
	 */
	public void setAirPressure( int air )
	{
		this.userAirPressure = air;
	}

	/**
	 * Updates the mouse coordinates
	 * @param point the position of the mouse
	 */
	public void setMousePosition(Point point)
	{
		mousePosition = point;
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

}
