/*Interation 2 modes selection GUI class, created by Kstrype on 14/10*/
/*@Author: kevin strype*/
package tim;

import javax.swing.JFrame;
import javax.swing.JButton;
import javax.swing.ImageIcon; 
import java.awt.event.ActionListener;
import javax.swing.ImageIcon;
import java.awt.event.ActionEvent;
import javax.swing.JPanel;
import java.awt.FlowLayout;
import java.awt.BorderLayout;
import javax.swing.SwingConstants;

/*here we are, this is the GI frame that should popup and show you which mode to pick; It is desgined to be popuped up externally, and will return which mode is selected. I havent worked out teh detials of how it will do this realiably, but I'm sure I will come iteration 3*/
public class ModeSelectionGUI implements ActionListener
{
private JFrame mainFrame; /*this is the base frame that is popuped with the relevant buttons on it*/
private JButton MyPuzzle; /*here is the button for the puzzel play mode*/
private JButton MyFreePlay; /*here is the button for the freeplay mode*/
/*The mode varibule, feel free to change type to an enum or something*/
private String modeType;

	/*Constructor; takes no arguments and configures all the window componets*/
	public ModeSelectionGUI()
	{
	JPanel base;
	ImageIcon puzzle;
	ImageIcon free;
	BorderLayout basepane;
	
	
		/*change the 'filename' to load a different image for the button text*/
		puzzle = new ImageIcon("./resources/icons/puzzelmodebutton.png");
		/*Change the 'filename' to change the button image for free play*/
		free = new ImageIcon("./resources/icons/freeplay.png");
		
		basepane = new BorderLayout();
		base = new JPanel();
		mainFrame = new JFrame("Please Choose Your Mode");
		mainFrame.setSize(800,350);
		//base.setLayout(basepane);
		mainFrame.add(base);
		mainFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE) ;
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
	}
	/*Popup GUI; takes no arguments; Pop's up the window, it will be hidden when you click a button*/
	public void popupGUI()
	{
		mainFrame.setVisible(true);
	}
	public void actionPerformed(ActionEvent buttonpressed)
	{
		/*THis will setup the mode based on how you clicked the buttons*/
		if(buttonpressed.getActionCommand().equals("Free Mode"))
		{
			System.out.println(buttonpressed.getActionCommand());
			GameWindow window = GameWindow.getInstance();
			window.setMode("free");
			mainFrame.setVisible(false);
			modeType = "free";
		} 
		if(buttonpressed.getActionCommand().equals("Puzzle Mode"))
		{
			System.out.println(buttonpressed.getActionCommand());
			GameWindow window = GameWindow.getInstance();
			window.setMode("puzzle");
			mainFrame.setVisible(false);
			modeType = "puzzle";
		} 

		//mainFrame.close();
	}
	
}

