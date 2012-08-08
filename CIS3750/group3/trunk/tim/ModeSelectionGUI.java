/*Interation 2 modes selection GUI class, created by Kstrype on 14/10*/
package tim;

import java.awt.Toolkit;
import java.awt.Dimension;
import javax.swing.JDialog;
import javax.swing.JButton;
import javax.swing.ImageIcon;
import java.awt.event.ActionListener;
import javax.swing.ImageIcon;
import java.awt.event.ActionEvent;
import javax.swing.JPanel;
import java.awt.FlowLayout;
import java.awt.BorderLayout;
import javax.swing.SwingConstants;

/**
	This is the GUI frame that should popup and show you which mode to pick;
	It is desgined to be popped up externally, and will return which mode is selected.
	@author Kevin Strype
*/
public class ModeSelectionGUI implements ActionListener
{
	/** this is the base frame that is popuped with the relevant buttons on it*/
	private JDialog mainFrame;

	/** The button for the puzzel play mode */
	private JButton MyPuzzle;

	/** The button for the freeplay mode */
	private JButton MyFreePlay;
	
	/*Jamie (2008-11-20) Was going to use enum, but it was being annoying.  Using boolean
	instead to represent game modes.  True = free-play, false = puzzle*/
	private boolean gameType;

	/**
	 * Constructor; takes no arguments and configures all the window componets.
	 *
	 *
	 */
	public ModeSelectionGUI()
	{
		JPanel base;
		ImageIcon puzzle;
		ImageIcon free;
		BorderLayout basepane;
		gameType = true;
		
		/*change the 'filename' to load a different image for the button text*/
		puzzle = new ImageIcon("./resources/icons/puzzelmodebutton.png");
		/*Change the 'filename' to change the button image for free play*/
		free = new ImageIcon("./resources/icons/freeplay.png");
		//mainFrame.setResizable(false);
		basepane = new BorderLayout();
		base = new JPanel();
		mainFrame = new JDialog();
		mainFrame.setTitle("Please Choose Your Mode");
		mainFrame.setSize(800,350);
		//base.setLayout(basepane);
		mainFrame.add(base);
		/*Jamie (2008-11-20) Make the user select a mode*/
		mainFrame.setDefaultCloseOperation(JDialog.DO_NOTHING_ON_CLOSE);
		mainFrame.setModal(true);
		MyFreePlay = new JButton(free);
		MyPuzzle = new JButton(puzzle);
		//base.add(basepane);
		/*Here is what sets the tool tip text! HUZA!*/
		MyFreePlay.setToolTipText("Free Mode");
		MyPuzzle.setToolTipText("Puzzle Mode");
		/*This is what sets the name of the button, I cant seem to set the action command to be something 			unless this is set..*/
		MyFreePlay.setText("Free Mode");
		MyPuzzle.setText("Puzzle Mode");
		MyFreePlay.setVerticalTextPosition(SwingConstants.BOTTOM);
		MyPuzzle.setVerticalTextPosition(SwingConstants.BOTTOM);
		base.add(BorderLayout.WEST,MyFreePlay);
		base.add(BorderLayout.EAST,MyPuzzle);
		MyPuzzle.addActionListener(this);
		MyFreePlay.addActionListener(this);
		/* Time to keep Phil appeased.
		 * The following method centers the frame on the screen
		 * 
		 * Reference
		 * ---------------------------
		 * Author: Code Beach ( Name of actual person could not be located ) 
		 * URL: http://blog.codebeach.com/2008/02/center-dialog-box-frame-or-window-in.html
		 * 
		 */
		
		/* Phil (2008-10-23): LOL */
		
		/* get the screen resolutions */
		Dimension screen = Toolkit.getDefaultToolkit().getScreenSize();


		/* convert the size to integers 
		 * subtract from the window size and div by 2
		 * to get the center position of the screen
		 */
		int x = ((int)screen.getWidth() - mainFrame.getWidth()) / 2;
		int y = ((int)screen.getHeight() - mainFrame.getHeight()) / 2; 

		mainFrame.setLocation(x, y);
	}


	/**
	 * Popup GUI; takes no arguments; Pop's up the window, it will be hidden when you click a button
	 *
	 */
	public void popupGUI()
	{
		mainFrame.setVisible(true);
	}
	
	/*Jamie (2008-11-20) Adding a getter for the mode type.*/
	
	/**
	 * Game mode getter
	 * @return free-play or puzzle mode
	 */
	 
	 public boolean getType()
	 {
		return gameType;
	 }

	/**
	 * Action Listener, does things based on which button was pressed
	 *
	 * @param buttonpressed the action event that trigged the listener
	 *
	 *
	 */
	public void actionPerformed(ActionEvent buttonpressed)
	{
		/*THis will setup the mode based on how you clicked the buttons*/
		if(buttonpressed.getActionCommand().equals("Free Mode"))
		{
			System.out.println(buttonpressed.getActionCommand());
			mainFrame.dispose();
			gameType = true;
		}
		if(buttonpressed.getActionCommand().equals("Puzzle Mode"))
		{
			System.out.println(buttonpressed.getActionCommand());
			gameType = false;
			mainFrame.dispose();
		}
	}

}

