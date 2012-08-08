/*
 * GameWindow.java
 *
 * Written by Filip Anguelov, Andy Crowell, Darren O'Shea and Trevor Clark
 * ID: 0223807
 * E-mail: fanguelo@uoguelph.ca
 *
 * This class displays the various game pieces and initiates the various
 * helper classes
 *
 */

package tim;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.Image;
import java.awt.Toolkit;
import java.io.File;
import java.io.IOException;
import java.util.ArrayList;

import javax.swing.BorderFactory;
import javax.swing.Icon;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JSplitPane;
import javax.swing.border.EtchedBorder;

import sun.tools.tree.ThisExpression;

import net.phys2d.math.Vector2f;
import net.phys2d.raw.World;

public class GameWindow extends JFrame
{
	private static final long serialVersionUID = 1L;

	/*  The GameWindow singleton instance. */
	private static GameWindow instance = null;

	/*  The visible playfield */
	private PlayfieldPanel playPanel = null;

	/*  The toolbar */
	private GameToolBar tool = null;

	/*  The top menu bar for the game */
	private GameMenuBar menuBar = null;

	/* The panel for the widget selection */
	private WidgetPanel widgetPanel = null;

	private GameController controller = null;

	/* The panel for the score */
	private ScorePanel scorePanel = null;

	/* The split pane to house the widgets/gamepanel */
	private JSplitPane splitPane = null;

	private String levelName = "Test level";

	private String hint = "This is the hint...";

	/* The game type flag for free-play (true) or puzzle (false) */
	private boolean gameType;

	/* Stores the title of the game */
	private final String windowTitle = "Awesome Mechanical Supercolliding Simulator and Apple Peeler";

	/**
	 * Instantiates a new game window.
	 */
	private GameWindow()
	{
		super( );

		this.setTitle( this.windowTitle );

		this.gameType = true;
		this.setLayout(new BorderLayout());
		this.menuBar = new GameMenuBar(this);
		this.widgetPanel = new WidgetPanel();
		this.playPanel = new PlayfieldPanel();

		this.controller = new GameController( this.playPanel );
		this.playPanel.setController( controller );
		this.playPanel.show();

		/* Phil (2008-10-19): Added Listener */
		this.menuBar.setActionListener( new GameMenuListener( controller ) );
		this.menuBar.setAll();

		/* Phil (2008-10-19): Trying to make it look better */
		JPanel frameIt = new JPanel(new BorderLayout());
		frameIt.setBorder(BorderFactory.createEmptyBorder(0, 5, 5, 5));

		this.tool = new GameToolBar(controller);
		this.add(tool, BorderLayout.NORTH);

		/* Phil (2008-10-19): Create a split pane. */
		splitPane = new JSplitPane( JSplitPane.HORIZONTAL_SPLIT, playPanel, widgetPanel );

		splitPane.setOneTouchExpandable(  true );
		splitPane.setDividerLocation(505);

		splitPane.setDividerSize( 10 );

		frameIt.add(splitPane, BorderLayout.CENTER);

		/* ADDED SCORE PANEL */
		scorePanel = new ScorePanel(9999);
		scorePanel.startDecrementing();

		frameIt.add(scorePanel.getPanel(), BorderLayout.SOUTH);

		add(frameIt, BorderLayout.CENTER);

		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		/* Phil (2008-10-23): adjusted size in order to make playfied 500x500 */
		setBounds(0, 0, 800, 620);
		setJMenuBar(menuBar);

		/* Phil (2008-10-19): Added a way to center the frame on the screen */
		Dimension screen = Toolkit.getDefaultToolkit().getScreenSize();
		int x = ((int)screen.getWidth() - this.getWidth()) / 2;
		int y = ((int)screen.getHeight() - this.getHeight()) / 2;

		this.setLocation(x, y);

		gameType = true;
		setVisible(true);
	}

	/**
	 * Gets the single instance of GameWindow.
	 *
	 * @return single instance of GameWindow
	 */
	public static GameWindow getInstance()
	{
		if(instance == null)
		{
			instance = new GameWindow();
		}
		return instance;
	}

	public GameController getController()
	{
		return controller;
	}

	public ActiveButton createActiveButton(GamePiece piece)
	{
		return this.controller.createActiveButton(widgetPanel, piece);
	}

	public void clearWidgetPanel()
	{
		widgetPanel.clear();
	}

	public void clear(boolean bForce)
	{
		this.controller.clearGame(bForce);
	}

	public void addWidget(GamePiece piece)
	{
		this.controller.addWidget(piece);
	}

	public void addTask(LevelTask task)
	{
		this.controller.addTask(task);
	}

	public ArrayList<LevelTask> getTasks()
	{
		return this.controller.getTasks();
	}

	public ArrayList<GamePiece> getGamePieces()
	{
		return this.controller.getGamePieces();
	}

	public String getLevelName()
	{
		return levelName;
	}

	public void setLevelName(String inLevelName)
	{
		levelName = inLevelName;
	}

	public String getHint()
	{
		return hint;
	}

	public void setHint(String inHint)
	{
		hint = inHint;
	}

	public int getTargetTime()
	{
		return scorePanel.getInitialScore();
	}

	public int getGravity()
	{
		return this.controller.getGravity();
	}

	public int getAirPressure()
	{
		return this.controller.getAirPressure();
	}

	/**
	 * Set the type of game (free-play or puzzle)
	 * @param type the boolean flag for free-play (true) or puzzle (false)
	 */
	public void setGameType(boolean type)
	{
		gameType = type;
		/* Free play, the super flexible sandbox mode */
		if(type == true)
		{
			this.scorePanel.getPanel().setVisible(false);
			this.scorePanel.stopDecrementing();
			this.scorePanel.resetScore();
			/* enable the lock button for free play */
			this.tool.enableLock();
			/* Load ALL widgets in free play */
			this.controller.fillWidgetPanelInFreePlay(this.widgetPanel);
		}
		/* Puzzle mode, the restricted mode for premade levels */
		else
		{
			this.menuBar.getMenu(0).getItem(2).setEnabled(false); /*disable "Save"*/
			this.menuBar.getMenu(0).getItem(3).setEnabled(false); /*disable "Save As"*/
			this.menuBar.getMenu(1).getItem(1).setEnabled(false); /*disable "Set Score"*/

			/* Jamie (2008-11-21) set the initial level score and start decrementing*/
			this.scorePanel.getPanel().setVisible(true);
			this.scorePanel.setInitialScore(this.menuBar.getScoreInfo().getLevelScore());
			this.scorePanel.setDecrement(this.menuBar.getScoreInfo().getLevelDecrement());
			this.scorePanel.initializeTimer();
			this.scorePanel.startDecrementing();

			/* disable the lock button in the toolbar for puzzle mode */
			this.tool.disableLock();
		}
	}

	/**
	 * Get the type of game (free-play or puzzle)
	 * @return gameType the type of game (free-play = true, puzzle = false)
	 */
	public boolean getGameType()
	{
		return gameType;
	}

	/**
	 * The main method.
	 *
	 * @param args the arguments
	 */
	public static void main(String[] args)
	{
		ModeSelectionGUI gui = new ModeSelectionGUI();
		gui.popupGUI();
		getInstance().setVisible(true);
		/*Jamie (2008-11-20) Setting the gameType variable for use later when we need to distinguish between
							 the modes*/
		getInstance().setGameType( gui.getType() );
	}


}
