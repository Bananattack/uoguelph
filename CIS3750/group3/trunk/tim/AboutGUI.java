/*Name; AboutGUI
  Author: Jamie McDade
  Course: CIS*3750
  Modified: 10/23/08
*/

package tim;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

import java.io.*;

/*This class helps to display the 'about' screen when accessed.  Originally was going to hardcode the data in, 
  but decided to do the external file method, akin to Trevor's HelpGUI.*/
@SuppressWarnings("serial")
public class AboutGUI extends JFrame
{
	public static String aboutFileName = "resources/about.txt"; //about file to read
	private static String iconPath = "resources/icons/"; //icon folder to use for error icon (if needed)
	private FileIO fileReader;
	private IconMaker iconMaker;
	public AboutGUI()
	{
		super("About");
		this.setSize(300,300);
		this.setResizable(false);
		iconMaker =  new IconMaker();
	
		try 
		{
			
			File current = new File(new File(".").getCanonicalPath());
			fileReader = new FileIO(current); //handle the about file opening and such
			
		} catch (IOException e1) {}
		
		/* Phil (2008-10-23): Great job Jamie, I like it! :D, love the comments too
		 * Got rid of the okPanel as it was making the other panel smaller
		 * made contentPanel use boxLayout as it's way easier to add stuff to it, esp for dialogs
		 */
		
		JPanel contentPanel = new JPanel (); //set up the display
		contentPanel.setLayout(new BoxLayout(contentPanel, BoxLayout.Y_AXIS));
		
		JButton ok = new JButton("OK"); //OK button - close the window when clicked
		ok.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e)
			{
				setVisible(false);
				dispose();
			}
		});
	
		contentPanel.add(createAboutText());
		contentPanel.add(ok);
		contentPanel.setBorder(BorderFactory.createEmptyBorder(5, 5, 5, 5)); //I hear Phil loves borders
		/* Phil (2008-10-23): I like peanut butter chocolate chip cookies too; where's ma' cookies? */
		this.add(contentPanel);
		
		
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
		int x = ((int)screen.getWidth() - this.getWidth()) / 2;
		int y = ((int)screen.getHeight() - this.getHeight()) / 2; 

		this.setLocation(x, y);
	}

	private JScrollPane createAboutText() //make a scrollPane and upload the text from the 'about' file
	{
		JEditorPane textPane = null;
		JScrollPane scroll1 = null;
		PromptBox errorBox = new PromptBox();
		/*HTML style for Andy*/
		String text = fileReader.openAsHTML(aboutFileName);

		if( text == null )
		{
			errorBox.show(iconMaker.makeIcon(fileReader.getGameDir() + iconPath + "dialog-error.png"), "File Not Found", "Could not find the About file. " + aboutFileName, 350, 150);
			System.exit(0);
		}
		textPane = new JTextPane();
		textPane.setContentType("text/html");
		textPane.setEditable(false);
		textPane.setText(text);
		/*Requesting focus at the top so the scrollbar starts at the top.
		  Credit to: http://bytes.com/forum/thread604847.html "Position of a JScrollPane vertical scrollbar slider?"
		  Posted by: horace1 - February 21st, 2007 10:13 AM
		*/
		textPane.select(0,1);
		textPane.requestFocus();
		scroll1 = new JScrollPane(textPane);
		scroll1.setWheelScrollingEnabled(true);
		return (scroll1);
	}
}

