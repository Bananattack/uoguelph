/*
 * GameController.java
 *
 * Written by Filip Anguelov, Andrew Crowell, Darren O'Shea, Jamie McDade, Kevin Strype
 * E-mail: fanguelo@uoguelph.ca
 *
 * This class controls the game functionality by either calling on helper classes
 * or doing the job itself. Various other classes depend on it to do their job,
 * for example all the listeners for buttons and menus.
 *
 */
package tim;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Point;
import java.io.File;
import java.io.FilenameFilter;
import java.io.IOException;
import java.util.ArrayList;

import javax.swing.BorderFactory;
import javax.swing.BoxLayout;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JSeparator;


import net.phys2d.raw.Body;
import net.phys2d.raw.CollisionEvent;
import net.phys2d.raw.CollisionListener;
import net.phys2d.raw.Joint;
import Widgets.*;

/**
 * @author Group Three
 *
 */
public class GameController implements CollisionListener
{
	private Point mousePosition = null;
	private WidgetBuilder builder = null;
	private GameEngine engine = null;
	private FileIO fileReader = null;
	private PlayfieldPanel panel = null;
	private GameDrawer drawer = null;

	private IconMaker iconMaker;
	private ImageIcon emptyIcon = null;
	private static final String ICON_PATH = "resources/icons/";
	
	private Boolean runningLevel = false;

	/** A list of the on-map widgets */
	/* Phil: I think activatedWidgets is more descriptive, no? */
	/* Andy: Maybe level widgets? Because they're the widgets on the level? */
	/* Phil: works for me */
	private ArrayList<GamePiece> levelPieces = null;

	private ArrayList<ActiveButton> levelButtons = null;

	private ArrayList<LevelTask> levelTasks = new ArrayList<LevelTask>();

	private Widget selectedPiece = null;

	private boolean isNewPiece = false;
	private boolean isMovingOldPiece = false;



	/**
	 * Instantiates a new game controller.
	 *
	 * @param panel The panel where the game is to be played must be passed it in order to be able to
	 * draw and manipulate widgets.
	 */
	public GameController( PlayfieldPanel panel )
	{

		/* Instantiates the widget builder */
		this.setBuilder( new WidgetBuilder() );

		/* Instantiates the GameEngine passing this class to  it */
		this.engine = new GameEngine( this );

		engine.addCollisionListener( this );

		this.panel = panel;

		/* Instantiates the drawer class for drawing objects */
		this.drawer =  new GameDrawer(this);

		/* Creates a new file handler class to open and save files for the GUI*/
		try
		{

			File current = new File(new File(".").getCanonicalPath());
			this.fileReader = new FileIO(current);

		} catch (IOException e1) {}

		this.iconMaker = new IconMaker();
		this.emptyIcon = iconMaker.makeIcon( fileReader.getGameDir() + ICON_PATH + "/widgets/none.png");
		/* Instantiate the arrays */
		this.levelPieces = new ArrayList<GamePiece>();
		this.levelButtons = new ArrayList<ActiveButton>();


	}


	/**
	 * Start game.
	 *
	 * This method starts the 2D game simulation and disables the
	 * buttons in the widget panel
	 *
	 */
	public void startGame()
	{

		/* Check to see if we are not running alrady */
		if(!runningLevel)
		{

			/* if not run the game */
			runningLevel = true;

			/* go through the list of widgets and activate them */
			for(GamePiece levelPiece : levelPieces)
			{
				levelPiece.getWidget().resetWidget();
				levelPiece.getWidget().activateWidget();
			}

			/* Disable the widgets in the widget panel */
			for(ActiveButton button : levelButtons)
			{
				button.getButton().setEnabled(false);
			}
		}

		/* then call to start the engine */
		engine.setRunning(true);

	}

	/**
	 * Stop game.
	 *
	 * This method stops the running game and resets the widgets to their
	 * original state
	 */
	public void stopGame()
	{
		/* stop the simulation */
		runningLevel = false;
		engine.setRunning(false);

		/* Go through the widgets to stop and reset them */
		for( GamePiece levelPiece : levelPieces )
		{
			levelPiece.getWidget().resetWidget();
		}

		/* Re-enable the widget panel buttons */
		for( ActiveButton button : levelButtons )
		{
			/* if the button does not have any widgets left do not re-enable */
			if( button.getRemainingAmount() != 0)
				button.getButton().setEnabled( true );
		}
	}

	/**
	 * Pause game.
	 *
	 * This method pauses a running game
	 */
	public void pauseGame()
	{
		/* If the game is running, pause it */
		if(engine.isRunning())
		{
			engine.setRunning(false);
		}
		else
		{
			startGame();
		}
	}

	public void clearGame()
	{
		clearGame(false);
	}

	public void clearGame(boolean bForce)
	{
		if(bForce == false)
		{
			/* pop up a dialog asking for confirmation */
			PromptBox errBox = new PromptBox();
			
			/* If the list of widgets is 0 no reason to do anything */
			if( this.levelPieces.size() == 0 && this.isRunning() )
			{
				return;
			}
			
			String toDelete = "\n";
			for(GamePiece piece : levelPieces)
			{
				toDelete += piece.getWidget().getName() + " locked: " + piece.getWidget().isLocked() + "\n";
			}
			
			/* ask the user to confirm action */
			boolean choice = errBox.showPrompt( iconMaker.makeIcon( fileReader.getGameDir() + ICON_PATH + "warning.png" ), 
					"Delete Widgets?", "You are about to delete all unlocked widgets. Would you like to continue?\n" + toDelete, 350, 250);
			
			errBox.dispose();
			
			if( !choice ) return;
			else
			{
				this.selectedPiece = null;

				
				ArrayList<GamePiece> tempPieces = new ArrayList<GamePiece>();
				
				for(GamePiece piece : levelPieces)
				{
				/* Go through the widgets to delete all unlocked widgets */
					
					/* is the widget in the list locked? */
					if( ! piece.getWidget().isLocked() )
					{
						/*Increases the count for the removed widget*/
						for(ActiveButton button : levelButtons)
						{
							if(button.getButton().getWidget().getClass() == piece.getWidget().getClass())
							{

								button.incrementAmount();
								button.getButton().setEnabled( true );
							}
						}
						
					}else
						tempPieces.add( piece );
				}
				
				this.engine.resetWorld();
				this.levelPieces.clear();
				this.levelPieces = tempPieces;
				/* now reload back into the engine those that were not deleted */
				this.reloadPieces();
			}
		}
		else
		{
			this.stopGame();
			this.selectedPiece = null;
			this.runningLevel = false;
			this.levelPieces.clear();
			levelTasks.clear();
			this.engine.resetWorld();
		}
	}
	
	/**
	 * Reload pieces into the engine.
	 */
	public void reloadPieces()
	{
		for( GamePiece levelPiece : levelPieces )
		{
			/* get a list of bodies from the widget and add them to the world */
			Body[] bodies = levelPiece.getWidget().getBodiesForSimulation();

			for(Body body : bodies)
			{
				this.engine.addObject( body );
			}


			/* get a list of joints from the widget and add them to the world */
			Joint[] joints = levelPiece.getWidget().getJointsForSimulation();

			if(joints !=  null)
			{
				for(Joint joint : joints)
				{
					this.engine.addObject( joint );
				}
			}

			/* update the screen */
			this.render();
		}
	}

	/**
	 * Render.
	 *
	 * This method renders the objects to screen by using the game panels
	 * Graphics object in order to draw.
	 */
	public void render()
	{
		/* call the render method with the object from the panel */
		if(isWon())
		{
			JOptionPane.showMessageDialog(GameWindow.getInstance(), "You win!", "Congratulations!", JOptionPane.INFORMATION_MESSAGE);
			levelTasks.clear();
		}
		this.panel.repaint();
	}


	/**
	 * Render.
	 *
	 * This method renders the objects to screen by using the Graphics object passed
	 * to it. It overload the previous method.
	 *
	 * @param g The graphics 2D object
	 */
	public void render( Graphics2D g )
	{
		this.drawer.render( g, this.levelPieces, selectedPiece);
	}

	/**
	 * Sets the new piece toggle for the controller.
	 *
	 * This is used when we click on a widget in the panel and then added it to the screen
	 * eliminating the problem of clicking on a widget on the screen and then cloning it.
	 *
	 * @param isNew Boolean setting the flag
	 */
	public void setNewPiece( boolean isNew )
	{
		System.out.println("Set new to " + isNewPiece);
		if(isNew) setMovingOldPiece(false);
		this.isNewPiece = isNew;
	}

	/**
	 * Checks if its new piece.
	 *
	 * @return true, if its new piece
	 */
	public boolean isRunning()
	{
		return this.engine.isRunning();
	}

	/**
	 * Checks if its new piece.
	 *
	 * @return true, if its new piece
	 */
	public boolean isNewPiece()
	{
		return this.isNewPiece;
	}

	/**
	 * Sets the moving old piece toggle for the controller.
	 *
	 * @param isMovingOldPiece Boolean setting the flag
	 */
	public void setMovingOldPiece(boolean isMovingOldPiece)
	{
		System.out.println("Set moving to " + isMovingOldPiece);
		if(isMovingOldPiece) setNewPiece(false);
		this.isMovingOldPiece = isMovingOldPiece;
	}


	/**
	 * Checks if we're moving a piece that is already on the playfield
	 *
	 * @return true, if the controller is moving an old piece
	 */
	public boolean isMovingOldPiece()
	{
		return this.isMovingOldPiece;
	}

	/**
	 * Gets the level pieces.
	 *
	 * @return The Array List of level pieces
	 */
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

	/**
	 * Gets the default settings for air and grav.
	 *
	 * @return Integer array containing air and gravity
	 * int[0] is the Air value, int[1] is the Gravity
	 */
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
	 * Sets the selected piece.
	 * 
	 * @param piece the new selected piece
	 */
	public void setSelectedPiece(Widget piece)
	{
		this.selectedPiece = piece;
	}


	/**
	 * Gets the selected piece.
	 *
	 * @return the selected piece
	 */
	public Widget getSelectedPiece( )
	{
		return selectedPiece;
	}


	/**
	 * Adds the widget to the list of widgets and world
	 *
	 * @param piece The function accepts a GamePiece
	 */
	public void addWidget( GamePiece piece )
	{

		piece.getWidget().resetWidget();

		for(ActiveButton button : levelButtons)
		{
			if(button.getButton().getWidget().getClass() == piece.getWidget().getClass())
			{
				button.decrementAmount();

				if(button.getRemainingAmount() == 0)
				{
					button.getButton().setEnabled( false );
				}
			}
		}


		/* get a list of bodies from the widget and add them to the world */
		Body[] bodies = piece.getWidget().getBodiesForSimulation();

		for(Body body : bodies)
		{
			this.engine.addObject( body );
		}


		/* get a list of joints from the widget and add them to the world */
		Joint[] joints = piece.getWidget().getJointsForSimulation();

		if(joints !=  null)
		{
			for(Joint joint : joints)
			{
				this.engine.addObject( joint );
			}
		}


		/* add it to our list of widgets */
		levelPieces.add(piece);

		/* update the screen */
		this.render();

		/* Andy (2008-11- 20): Probably handy while debugging, but I commented this out. */
		/*
		System.out.println("*****************");
		for(GamePiece levelPiece : levelPieces)
		{
			System.out.println("*" + levelPiece + ": " + levelPiece.getWidgetID());
		}
		System.out.println("*****************");
		*/
	}

	public ArrayList<GamePiece> getGamePieces()
	{
		return levelPieces;
	}

	public ArrayList<ActiveButton> getLevelButtons()
	{
		return levelButtons;
	}

	/**
	 * Adds the task.
	 * Written by: Darren
	 * @param task the task
	 */
	public void addTask(LevelTask task)
	{
		levelTasks.add(task);
	}

	public ArrayList<LevelTask> getTasks()
	{
		return levelTasks;
	}

	/**
	 * Checks if is won.
	 * Written by: Darren
	 * @return true, if is won
	 */
	public boolean isWon()
	{
		if(levelTasks.size() <= 0)
		{
			return false;
		}

		for(LevelTask task : levelTasks)
		{
			if(task.isComplete() == false)
			{
				return false;
			}
		}

		return true;
	}


	/**
	 * Delete widget.
	 * Written by: Jamie
	 */
	public void deleteWidget()
	{
		
		/* create a prompt box to output errors */
		PromptBox errBox = new PromptBox();
		
		/* Phil (2008-10-03): We need to keep the errors GUI'd */
		if( this.selectedPiece == null )
		{
			return;
		}
		
		//int choice;
		//choice = JOptionPane.showConfirmDialog(null, "Delete this widget?");
		
		boolean outcome = errBox.showPrompt( this.selectedPiece.getIcon(), 
				"Delete This Widget?", "You are about to delete the " + this.selectedPiece.getName() + ". Would you like to delete it?", 350, 100);
		
		errBox.dispose();
		
		if( !outcome ) return;
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
					/*Increases the count for the removed widget*/
					for(ActiveButton button : levelButtons)
					{
						if(button.getButton().getWidget().getClass() == levelPieces.get(i).getWidget().getClass())
						{

							button.incrementAmount();
							button.getButton().setEnabled( true );
							System.out.println("Incremented!" + button.getButton().getWidget().getName());
						}
					}

					levelPieces.remove(i);

					/* Phil (2008-11-02): fixed the remove so the active widget is no longer under the mouse/active */
					isNewPiece = false;
					isMovingOldPiece = false;
					this.selectedPiece = null;

				}
			}
		}
	}

	/**
	 * Fill widget panel for free play interaction
	 *
	 * This method fills the widget panel with buttons
	 *
	 * @param widgetPanel the widget panel
	 */
	public void fillWidgetPanelInFreePlay(WidgetPanel widgetPanel)
	{
		this.levelButtons = new ArrayList<ActiveButton>();
		
		ArrayList<Widget> widgets = WidgetLoader.getAllWidgets();
		for(Widget w : widgets)
		{/*
			try
			{*/
				System.out.println(w);
				createActiveButton(widgetPanel, new GamePiece(w));
				/*
			}
			catch(Exception e)
			{
				e.printStackTrace();
				System.out.println(e);
			}*/
		}
	}

	/**
	 * Make a button.
	 *
	 * This method makes an ActiveButton to be placed in the widget panel
	 *
	 * @param w The GamePiece to be added to the button
	 */
	public ActiveButton createActiveButton(WidgetPanel widgetPanel, GamePiece w)
	{
		JLabel wLabel;
		WidgetPanelButton b;

		/* Andy (2008-10-20): Now applying the tooltip to every element. */
		String toolTipText = "<html><h2>" + w.getWidget().getName() + "</h2>" + w.getWidget().getDescription();
		Font font = new Font( "Sanserif", Font.TRUETYPE_FONT, 9 );

		/* Want a panel so it would be easier to put the title under the button */
		/* Andy (2008-10-20): changed the variable case.
		 * Only types and constants should be capitalized, in my opinion. */
		JPanel buttonPanel = new JPanel();
		buttonPanel.setLayout(new BoxLayout(buttonPanel, BoxLayout.Y_AXIS));

		wLabel = new JLabel( w.getWidget().getName() );

		wLabel.setFont( font );

		/* adding a border so things dont look like they are touching the edge */
		// wLabel.setBorder(BorderFactory.createEmptyBorder( 0, 2, 0, 2 ) );
		wLabel.setToolTipText(toolTipText);

		JPanel bPanel = new JPanel(new FlowLayout());

		/* Making the button from the WidgetPanelButton */
		/* if a widget does not have an icon give it an empty image */
		if( w.getWidget().getIcon() == null  )
			b = new WidgetPanelButton(w.getWidget(), this.emptyIcon, this);
		else
			b = new WidgetPanelButton(w.getWidget(), w.getWidget().getIcon(), this);
		
	
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
		/* Setting the label under the button, we get the name from the widget */

		ActiveButton activeButton = new ActiveButton( buttonPanel, b, wLabel);

		/* Don't display the count in free play */
		if(GameWindow.getInstance().getGameType())
		{
			activeButton.setTotalAmount(-1);
			activeButton.updateLabel();
		}
		levelButtons.add(activeButton);
		widgetPanel.add(activeButton.getPanel());
		return activeButton;
	}


	public GamePiece getPieceFromBody(Body b)
	{
		for(GamePiece g : levelPieces)
		{
			Body[] widgetBodies = g.getWidget().getBodiesForSimulation();
			for(Body bod : widgetBodies)
			{
				if(bod == b)
				{
					return g;
				}
			}
		}

		return null;
	}

	public void collisionOccured(CollisionEvent e)
	{
		GamePiece a = getPieceFromBody(e.getBodyA());
		GamePiece b = getPieceFromBody(e.getBodyB());

		if(b.getWidget() instanceof ObjectiveWidget)
		{
			for(LevelTask t : levelTasks)
			{
				t.hit(b, a);
			}
		}
		else if (a.getWidget() instanceof ObjectiveWidget)
		{
			for(LevelTask t : levelTasks)
			{
				t.hit(a, b);
			}
		}
		else
		{
			a.getWidget().reactToTouchingBody(e);
			b.getWidget().reactToTouchingBody(e);
		}
	}

	/**
	 * Lock game pieces.
	 *
	 * @param locked the locked
	 */
	public void lockGamePieces( Boolean locked )
	{
		/* Go through the widgets to lock */
		for( GamePiece levelPiece : levelPieces )
		{
			levelPiece.getWidget().setLock( locked );
		}
	}



	/**
	 * Lock selected game piece.
	 * 
	 * This method toggles the lock on the selected game piece
	 */
	public void lockSelected( )
	{
		if ( this.selectedPiece != null )
		{
			if( this.selectedPiece.isLocked() )
				this.lockSelected( false );
			else
				this.lockSelected( true );
		}
	}

	/**
	 * Lock selected.
	 *
	 * @param locked the locked
	 */
	public void lockSelected( Boolean locked )
	{
		this.getPiece().getWidget().setLock( locked );
		
		//System.out.println( levelPieces.get( i ).getWidget().getName() + "is locked: " + levelPieces.get( i ).getWidget().isLocked() );
		
		this.selectedPiece = null;
	}
	
	public void rotateLeft()
	{
		this.selectedPiece.rotateCounterClockwise();
		
		//this.getPiece().setDirection( this.selectedPiece.getDirection() );
	}
	
	public void rotateRight()
	{
		this.selectedPiece.rotateClockwise();
		
		//this.getPiece().setDirection( this.selectedPiece.getDirection() );
		
	}
	
	private GamePiece getPiece()
	{
		Widget tempWidget = this.selectedPiece;

		float tempX = this.selectedPiece.getPositionX();
		float tempY = this.selectedPiece.getPositionY();

		for(int i=0; i<levelPieces.size();i++)
		{
			tempWidget = levelPieces.get(i).getWidget();

			if( ( tempWidget.getPositionX() == tempX ) && ( tempWidget.getPositionY() == tempY ) )
			{

				return levelPieces.get( i );

			}
		}
		
		return null;
	}

	/**
	 * Retreives the current mouse position
	 * @return the current mouse position on the playfield (or null if not applicable)
	 */
	public Point getMousePosition()
	{
		return mousePosition;
	}

	/**
	 * Sets the current mouse position
	 * @param pos the current position of the mouse on the playfield (or null if not applicable)
	 */
	public void setMousePosition(Point pos)
	{
		mousePosition = pos;
	}
}
