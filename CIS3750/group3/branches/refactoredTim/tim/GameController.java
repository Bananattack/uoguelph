/**
 *
 */
package tim;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.io.File;
import java.io.FilenameFilter;
import java.io.IOException;
import java.util.ArrayList;

import javax.swing.BorderFactory;
import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JSeparator;


import net.phys2d.raw.Body;
import net.phys2d.raw.Joint;
import Widgets.Widget;

/**
 * @author epicbard
 *
 */
public class GameController
{

	private WidgetBuilder builder = null;
	private GameEngine engine = null;
	private FileIO fileReader = null;
	private PlayfieldPanel panel = null;
	private GameDrawer drawer = null;
	
	private Thread world = null;
	
	private Boolean runningLevel = false;

	/** A list of the on-map widgets */
	/* Phil: I think activatedWidgets is more descriptive, no? */
	/* Andy: Maybe level widgets? Because they're the widgets on the level? */
	/* Phil: works for me */
	private ArrayList<GamePiece> levelPieces = null;

	private ArrayList<ActiveButton> levelButtons = null;

	private Widget selectedPiece = null;

	private boolean isNewPiece = false;



	public GameController(PlayfieldPanel panel)
	{
		this.setBuilder( new WidgetBuilder() );
		this.engine = new GameEngine( this );
		this.panel = panel;
		this.drawer =  new GameDrawer();

		/* Creates a new file handler class to open and save files for the GUI*/
		try
		{

			File current = new File(new File(".").getCanonicalPath());
			this.fileReader = new FileIO(current);

		} catch (IOException e1) {}

		this.levelPieces = new ArrayList<GamePiece>();
		this.levelButtons = new ArrayList<ActiveButton>();


	}

	public void startGame()
	{

        if( !runningLevel )
        {
                runningLevel = true;

        		for(GamePiece levelPiece : levelPieces)
        		{
        			levelPiece.getWidget().activateWidget();
        		}

                /*Phil: (2008-10-27) HURRAY, grayedout buttons! */
                for(ActiveButton button : levelButtons)
                {
                        button.getButton().setEnabled( false );
                }
                
                this.engine.setRunning( true );
        }
		
	}
	
	public void stopGame()
	{

		runningLevel = false;
		 this.engine.setRunning( false );
		for(GamePiece levelPiece : levelPieces)
		{
			levelPiece.getWidget().activateWidget();
			levelPiece.getWidget().resetWidget();
		}

		/*Phil: (2008-10-27) HURRAY, buttons re-enabled! */
		for(ActiveButton button : levelButtons)
		{
			button.getButton().setEnabled( true );
		}
		
		this.render();

	}
	
	public void pauseGame()
	{
		if(runningLevel)
        {
                runningLevel = false;
                this.engine.setRunning( false );
                //widgetPanel.setEnabled(false);
                //splitPane.setEnabled( false );
                //splitPane.setDividerLocation(795);

        }
        else
                this.startGame();
	}
	
	
	
	public void clearGame()
	{
		int choice = 0;
        choice = JOptionPane.showConfirmDialog(null, "Delete all widgets?");
        if(choice != JOptionPane.YES_OPTION) return;
        else 
        {
        		this.stopGame();
                /* Phil (2008-11-02): fixed the clear method so blah blah no more active objects */
                this.selectedPiece = null;
                this.runningLevel = false;
                this.levelPieces.clear();
                this.engine.resetWorld();
                this.render();
                
          
        }

	}
	
	
	public void render()
	{
		this.render( (Graphics2D) this.panel.getGraphics() );
		this.panel.repaint();
	}
	public void render( Graphics2D g )
	{
		drawer.render( g, this.levelPieces);
	}


	public void setNewPiece( boolean is )
	{
		this.isNewPiece = is;
	}

	public boolean isNewPiece()
	{
		return this.isNewPiece;
	}

	public ArrayList<GamePiece> getLevelPieces()
	{
		return this.levelPieces;
	}
	/**
	 * Gets the gravity.
	 *
	 * @return the gravity
	 */
	public int getGravity()
	{
		return this.engine.getGravity();
	}

	/**
	 * Gets the air pressure.
	 *
	 * @return the air pressure
	 */
	public int getAirPressure()
	{
		return this.engine.getAirPressure();
	}

	/**
	 * Sets the gravity.
	 *
	 * @param grav the new gravity
	 */
	public void setGravity( int grav )
	{
		this.engine.setGravity( grav );
	}

	/**
	 * Sets the air pressure.
	 *
	 * @param air the new air pressure
	 */
	public void setAirPressure( int air )
	{
		this.engine.setAirPressure( air );
	}

	public int[] getDefaultSettings()
	{
		return this.engine.getDefaultSettings();
	}

	/**
	 * @param builder the builder to set
	 */
	public void setBuilder( WidgetBuilder builder )
	{
		this.builder = builder;
	}

	/**
	 * @return the builder
	 */
	public WidgetBuilder getBuilder( )
	{
		return builder;
	}

	/**
	 * Sets the active widget.
	 *
	 * @param GamePiece the new active widget
	 */
	public void setSelectedPiece( Widget piece)
	{
		System.out.println("Setting selected to " + piece);
		this.selectedPiece = piece;
	}

	public Widget getSelectedPiece( )
	{
		System.out.println("Getting selected " + selectedPiece);
		return selectedPiece;
	}


	/*
	 * Adds a widget to the world.
	 * @param widget The widget to append to the world's list of processing widgets.
	 */
	public void addWidget(GamePiece piece)
	{
		piece.getWidget().resetWidget();

		Body[] bodies = piece.getWidget().getBodiesForSimulation();

		for(Body body : bodies)
		{
			this.engine.addObject( body );
		}


		Joint[] joints = piece.getWidget().getJointsForSimulation();

		if(joints !=  null)
		{
			for(Joint joint : joints)
			{
				this.engine.addObject( joint );
			}
		}

		piece.getWidget().setLock( false );
		levelPieces.add(piece);

		this.isNewPiece = true ;


		this.render();

		System.out.println("*****************");
		for(GamePiece levelPiece : levelPieces)
		{
			System.out.println("*" + levelPieces);
		}
		System.out.println("*****************");
	}

	/* Jamie (2008-10-28) Added the deleteWidget method as per Phil's request */
	/*
	 * Deletes a widget.
	 * @param widget The widget to be deleted.
	 */
	public void deleteWidget()
	{
		int choice;
		choice = JOptionPane.showConfirmDialog(null, "Delete this widget?");
		if(choice != JOptionPane.YES_OPTION) return;
		else
		{
			Widget tempDeleteWidget = this.selectedPiece;

			float tempX = this.selectedPiece.getPositionX();
			float tempY = this.selectedPiece.getPositionY();
			for(int i=0; i<levelPieces.size();i++)
			{
				tempDeleteWidget = levelPieces.get(i).getWidget();

				if((tempDeleteWidget.getPositionX() == tempX) && (tempDeleteWidget.getPositionY() == tempY))
				{

					levelPieces.remove(i);
					/* Phil (2008-11-02): fixed the remove so the active widget is no longer under the mouse/active */
					this.selectedPiece = null;

				}
			}
		}
	}

	public void fillWidgetPanel( WidgetPanel widgetPanel )
	{
		this.levelButtons = new ArrayList<ActiveButton>();
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
				Class c = Class.forName("Widgets." + this.fileReader.stripExtension(filename));
				Object o = c.getConstructor(new Class[] {}).newInstance(new Object[] {});

				if(o instanceof Widget && o.getClass() != Widget.class)
				{
					System.out.println("Loaded '" + c.getSimpleName() + "'");
					ActiveButton button = makeAButton((Widget) o);
					levelButtons.add( button );
					widgetPanel.add(button.getPanel() );
				//	p.setEnabled( false );

				}
			}
			catch(Exception e)
			{
			}
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
	private ActiveButton makeAButton( Widget w )
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
       // wLabel.setBorder(BorderFactory.createEmptyBorder( 0, 2, 0, 2 ) );
        wLabel.setToolTipText(toolTipText);

        JPanel bPanel = new JPanel(new FlowLayout());

        /* Making the button from the WidgetPanelButton */
        JButton b = new WidgetPanelButton(w, this);
        b.setToolTipText(toolTipText);

        /* when the mouse is over the button/widget you get a tool tip with the description */
        bPanel.setToolTipText(toolTipText);
        bPanel.add( b );

        buttonPanel.add( bPanel );

        /* clearly separate button from widget */
        buttonPanel.add(new JSeparator(JSeparator.HORIZONTAL));

        /* add the label */
        JPanel lPanel = new JPanel(new FlowLayout());
        lPanel.add( wLabel );
        buttonPanel.add( lPanel );


        /* add a border around the panel to differentiate widgets */
        buttonPanel.setBorder( BorderFactory.createLineBorder(Color.GRAY) );

        buttonPanel.setPreferredSize( new Dimension (100, 140) );

        ActiveButton temp = new ActiveButton( buttonPanel, b, wLabel);
        return temp;


	}
}
