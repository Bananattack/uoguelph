// CIS*3750
// HelpGUI.java
// by Trevor Clark

// Andy (2008-10-01): Refactored class structure to be in package.
package tim;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

import java.io.*;

/*	
	HelpGUI is the help page in TIM. It reads a file
	and displays it in a text area.
*/

@SuppressWarnings("serial")
public class HelpGUI extends JFrame
{

	// Andy (2008-10-01): May need to modify this path later, so I made it a constant here.
	public static String helpFilename = "resources/help.txt";
	
	// Phil (2008-10-3)
	private static String iconPath = "resources/icons/";
	private FileIO fileReader;
	
	private IconMaker iconMaker;
	
	/**
	 * Instantiates a new help gui.
	 */
	public HelpGUI ()
	{
		super ("Help");
		this.setSize (300, 500);
		this.setResizable(false);
		
		/* initialize the icon maker*/
		iconMaker =  new IconMaker();
		
		/* Creates a new file handler class to open and save files for the GUI*/
		try 
		{
			
			File current = new File(new File(".").getCanonicalPath());
			fileReader = new FileIO(current);
			
		} catch (IOException e1) {}
		
		/* create content panel */
		JPanel contentPanel = new JPanel ();
		contentPanel.setLayout(new BorderLayout());
		
		/* create back button in new flowLayout, adjust size and add action listener */
		JPanel backPanel = new JPanel ();
		backPanel.setLayout(new FlowLayout(1));
		JButton back = new JButton ("Back");
		back.addActionListener(new backListener());
		backPanel.add(back);
		
		/* add components to content panel */
		contentPanel.add(createText(), BorderLayout.CENTER);
		contentPanel.add(backPanel, BorderLayout.SOUTH);
	
		/* Phil (2008-10-03): add a border so it looks nice (er) */
		contentPanel.setBorder(BorderFactory.createEmptyBorder(5, 5, 5, 5));
		
		this.add(contentPanel);
		
		/* Phil (2008-10-03): Added a way to center the frame on the screen
		 * 
		 * "Centering windows on the screen; It's a good thing." 
		 *  - Martha Stewart(tm)
		 * 
		 * The following method centers the frame on the screen
		 * 
		 * Reference
		 * ---------------------------
		 * Author: Code Beach ( Name of actual person could not be located ) 
		 * URL: http://blog.codebeach.com/2008/02/center-dialog-box-frame-or-window-in.html
		 * 
		 */
		
		/* get the screen resolutions */
		Dimension screen = Toolkit.getDefaultToolkit().getScreenSize();


		/* convert the size to integers 
		 * subtract from the window size and div by 2
		 * to get the center position of the screen
		 */
		int x = ((int)screen.getWidth() - this.getWidth()) / 2;
		int y = ((int)screen.getHeight() - this.getHeight()) / 2; 

		this.setLocation(x, y);

	}
	
	/* back button action listener */
	/**
	 * The listener interface for receiving back events.
	 * The class that is interested in processing a back
	 * event implements this interface, and the object created
	 * with that class is registered with a component using the
	 * component's <code>addbackListener<code> method. When
	 * the back event occurs, that object's appropriate
	 * method is invoked.
	 * 
	 * @see backEvent
	 */
	private class backListener implements ActionListener
    {

        public void actionPerformed(ActionEvent arg0) 
		{
			setVisible(false);
			dispose();
        }
    }

	/* create text area to view readme and insert readme file */
	private JScrollPane createText()
	{
		JEditorPane textpane = null;
		JScrollPane scrollingTemp = null;
		
		/* create a prompt box to output errors */
		PromptBox errBox = new PromptBox();
		
		/* use the file reader to read in the Help file as HTML per Andy's way */
		String text = fileReader.openAsHTML(helpFilename);

		if( text == null )
		{
			/* Phil (2008-10-03): We need to keep the errors GUI'd */
			errBox.show(iconMaker.makeIcon(fileReader.getGameDir() + iconPath + "dialog-error.png"), "File Not Found", "Could not find the Help file. " + helpFilename, 350, 150);
			System.exit(0);
		}
		
		textpane = new JTextPane();
		textpane.setContentType("text/html");
		textpane.setEditable(false);
		textpane.setText(text);

		/* create scroll bars for textarea */
		scrollingTemp = new JScrollPane(textpane);
		scrollingTemp.setWheelScrollingEnabled(true);
		
		
		return (scrollingTemp);
	}
}
