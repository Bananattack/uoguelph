package tim;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.Toolkit;
import java.util.ArrayList;

import javax.swing.BorderFactory;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JSplitPane;
import javax.swing.border.EtchedBorder;

import net.phys2d.math.Vector2f;
import net.phys2d.raw.World;

public class GameWindow extends JFrame
{

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;

	/** The GameWindow singleton instance. */
	private static GameWindow instance = null;
	
	/** The visible playfield */
	private PlayfieldPanel playPanel = null;

	/** The toolbar */
	GameToolBar tool = null;

	/** The top menu bar for the game */
	GameMenuBar menuBar = null;

	/** The panel for the widget selection */
	WidgetPanel widgetPanel = null;

	/** The split pane to house the widgets/gamepanel */
	JSplitPane splitPane = null;
	
	/**
	 * Instantiates a new game window.
	 * Takes in 1 string, which should be either free or puzzle, it will then configure teh GUI dependant on 
	 *What it recived.
	 *KSTRYPE: Unh.. Scratch that... doh.
	 */
	private GameWindow()
	{
		super();
		
		this.setLayout(new BorderLayout());


		

		this.menuBar = new GameMenuBar( this );
		

		this.widgetPanel = new WidgetPanel();
		//initWidgetPanel();

		this.playPanel = new PlayfieldPanel();
		
		GameController controller = new GameController( this.playPanel );
		this.playPanel.setController( controller );
		this.playPanel.show();
		
		/* Phil (2008-10-19): Added Listener */
		this.menuBar.setActionListener( new GameMenuListener( controller ) );
		this.menuBar.setAll();

		/* Phil (2008-10-19): Trying to make it look better
		 *
		 * Added a panel that would frame the whole thing.
		 *
		 */
		JPanel frameIt = new JPanel( new BorderLayout() );
		frameIt.setBorder(BorderFactory.createEmptyBorder(0, 5, 5, 5));

		this.tool = new GameToolBar( controller );
		this.add(tool, BorderLayout.NORTH);
		
		/* Phil (2008-10-19): Create a split pane. */
		splitPane = new JSplitPane( JSplitPane.HORIZONTAL_SPLIT, playPanel, widgetPanel );

		splitPane.setOneTouchExpandable(  true );
		splitPane.setDividerLocation(505);

		splitPane.setDividerSize( 10 );

		//frameIt.add(widgetPanel, BorderLayout.EAST);
		frameIt.add(splitPane, BorderLayout.CENTER);

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
		/* Phil (2008-10-23): adjusted size in order to make playfied 500x500 */
		setBounds(0, 0, 800, 620);
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

		controller.fillWidgetPanel( this.widgetPanel );
		setVisible(true);
	}
	
	/**
	 * The main method.
	 *
	 * @param args the arguments
	 */
	public static void main(String[] args)
	{
		GameWindow window = new GameWindow();
		//window.show();
	}
	
	
}
