package rogueView;

import rogueModel.*;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

import java.util.*;

/**
* The window that controls the game rendering.
* @author Andrew G. Crowell
*/
public class DungeonWindow extends JFrame
{
	private JSplitPane splitPane;
	private JSplitPane mainSplitPane;
	private JSplitPane sideSplitPane;
	private JTextArea statusBox;
	private JPanel bottomSidePanel;
	private JTextArea inventoryBox;
	private JTextArea messageBox;
	private MapView mapView;
	private JButton helpButton;
	
	/**
	* Create a new dungeon window with title and keylistener
	* @param title the title
	* @param keyListener the key listener event
	*/
	public DungeonWindow(String title, KeyListener keyListener)
	{
		super(title);
		addKeyListener(keyListener);
		
		setLayout(new BorderLayout());

		mainSplitPane = new JSplitPane(JSplitPane.VERTICAL_SPLIT);
		mainSplitPane.setPreferredSize(new Dimension(512, 480));
		sideSplitPane = new JSplitPane(JSplitPane.VERTICAL_SPLIT);
		splitPane = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT, mainSplitPane, sideSplitPane);
		splitPane.setResizeWeight(0.5);
		add(splitPane);
		
		mapView = null;
		
		messageBox = new JTextArea();
		messageBox.setLineWrap(true); 
		messageBox.setWrapStyleWord(true);
		messageBox.setFocusable(false);
		messageBox.setEditable(false);
		
		statusBox = new JTextArea();
		statusBox.setPreferredSize(new Dimension(200, 200));
		statusBox.setLineWrap(true); 
		statusBox.setWrapStyleWord(true);
		statusBox.setFocusable(false);
		statusBox.setEditable(false);
		
		bottomSidePanel = new JPanel();
		bottomSidePanel.setLayout(new BorderLayout());
		inventoryBox = new JTextArea();
		inventoryBox.setPreferredSize(new Dimension(200, 200));
		inventoryBox.setLineWrap(true); 
		inventoryBox.setWrapStyleWord(true);
		inventoryBox.setFocusable(false);
		inventoryBox.setEditable(false);
		bottomSidePanel.add(inventoryBox, BorderLayout.CENTER);
		helpButton = new JButton("Help");
		helpButton.addActionListener(new ActionListener()
				{
					public void actionPerformed(ActionEvent event)
					{
						popupMessage("Commands:\n"
								+ "Arrow keys - move\n"
								+ "W - wear item\n"
								+ "U - use item\n"
								+ "S - save game\n"
								+ "L - load game\n");
					}
				}
		);
		helpButton.setFocusable(false);
		bottomSidePanel.add(helpButton, BorderLayout.SOUTH);
		
		mainSplitPane.setTopComponent(mapView);
		mainSplitPane.setBottomComponent(messageBox);
		
		sideSplitPane.setTopComponent(statusBox);
		sideSplitPane.setBottomComponent(bottomSidePanel);
		
		
		JMenuBar menuBar = new JMenuBar();
		JMenu fileMenu = new JMenu("Game");
		JMenuItem fileMenuItemHelp = new JMenuItem("Help");
		fileMenuItemHelp.addActionListener(new ActionListener()
				{
					public void actionPerformed(ActionEvent event)
					{
						popupMessage("Commands:\n"
								+ "Arrow keys - move\n"
								+ "W - wear item\n"
								+ "U - use item\n"
								+ "S - save game\n"
								+ "L - load game\n");
					}
				}
		);		
		JMenuItem fileMenuItemAbout = new JMenuItem("About");
		fileMenuItemAbout.addActionListener(new ActionListener()
				{
					public void actionPerformed(ActionEvent event)
					{
						popupMessage("This game was made with much conflict and sleep deprivation the two days before the deadline. Enjoy!");
					}
				}
		);
		
		JMenuItem fileMenuItemExit = new JMenuItem("Exit");
		fileMenuItemExit.addActionListener(new ActionListener()
				{
					public void actionPerformed(ActionEvent event)
					{
			           System.exit(0);
					}
				}
		);
		
		menuBar.add(fileMenu);
		fileMenu.add(fileMenuItemHelp);
		fileMenu.add(fileMenuItemAbout);
		fileMenu.add(fileMenuItemExit);
		setJMenuBar(menuBar);
		
		setSize(800, 600);
		setDefaultCloseOperation(EXIT_ON_CLOSE);
		setVisible(true);
	}
	
	/**
	* Creates a new level, given a set of rooms and halls, and attaches it the dungeon window
	* @param rooms the list of rooms
	* @param hallways the list of halls
	* @throws BadMapException when map creation fails
	* @throws NoDoorThereException if a hallway door can't be located for some reason
	*/
	public void setNewLevel(ArrayList<Room> rooms, ArrayList<Hallway> hallways) throws BadMapException, NoDoorThereException
	{
		if(mapView != null)
		{
			mainSplitPane.setTopComponent(null);
		}
		mapView = new MapView(rooms, hallways);
		mainSplitPane.setTopComponent(mapView);
		repaint();
	}
	
	/**
	* Changes the font of the window
	* @param font the font
	*/
	public void setDungeonFont(String font)
	{
		messageBox.setFont(new Font(font, Font.PLAIN, 10));
		statusBox.setFont(new Font(font, Font.PLAIN, 10));
	}

	/**
	* Changes the displayed message in the message box
	* @param message the message
	*/	
	public void setMessage(String message)
	{
		messageBox.setText(message);
	}
	
	/**
	* Changes the displayed message in the stats box
	* @param message the message
	*/	
	public void setStats(String stats)
	{
		statusBox.setText(stats);
	}

	/**
	* Changes the displayed message in the inventory box
	* @param message the message
	*/	
	public void setInventory(String inventory)
	{
		inventoryBox.setText(inventory);
	}
	
	/**
	* Creates a popup with a specified message
	* @param message the message to show in a dialog
	*/	
	public void popupMessage(String message)
	{
		JOptionPane.showMessageDialog(this, message);
	}
	
	/**
	* Gets the window's active map
	* @return the map view
	* @throws MapNotSetException if map is not initialized yet
	*/
	public MapView getMap() throws MapNotSetException
	{
		if(mapView == null)
		{
			throw new MapNotSetException();
		}
		return mapView;
	}
	
	/**
	* Refreshes the window's active map, if any
	*/
	public void refreshMap()
	{
		if(mapView != null)
		{
			mapView.repaint();	
		}
		repaint();
	}
}
