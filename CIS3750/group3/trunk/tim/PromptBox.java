/*
 *  PromptBox.java
 * 
 *  Created by Filip Anguelov on 03/10/08.
 *  ID: 0223807
 *  E-mail: fanguelo@uoguelph.ca
 *  
 *  This is a simple dialog that we can reuse to show messages to the user instead
 *  of printing to the console
 *  
 *  I tried to keep the layout and naming conventions the same as the HelpGUI
 *  
 */
package tim;

import javax.swing.*;

import java.awt.*;
import java.awt.event.*;


/**
 * @author Filip Anguelov
 */
@SuppressWarnings("serial")
public class PromptBox extends JDialog implements ActionListener
{
	
	private boolean prompt = false;

	private boolean outcome = false;
	
	/**
	 * Empty constructor
	 */
	public PromptBox( )
	{
		super();
		super.setSize( 350, 150 );
	}

	/**
	 * Call this method to display the dialog to the user. You must pass in an ImageIcon object
	 * see IconMaker. 
	 * 
	 * @param icon The ImageIcon object, it must not be null.
	 * @param title The Title that will appear on the dialog.
	 * @param errMessage The message in the message box.
	 * @param width The width of the box.
	 * @param height The height of the box.
	 */
	public void show( ImageIcon icon, String title, String errMessage, int width, int height )
	{
		super.setSize( width, height );
		this.setUp (  icon,  title,  errMessage );
	}
	
	/**
	 * Call this method to display the dialog to the user without an icon
	 * 
	 * @param title The Title that will appear on the dialog.
	 * @param errMessage The message in the message box.
	 * @param width The width of the box.
	 * @param height The height of the box.
	 */
	public void show( String title, String errMessage, int width, int height )
	{
		super.setSize( width, height );
		this.setUp(  null,  title,  errMessage );
	}
	
	/**
	 * Show prompt asking the user for input.
	 * 
	 * @param icon the icon
	 * @param title the title
	 * @param errMessage the err message
	 * @param width the width
	 * @param height the height
	 * 
	 * @return true, if successful
	 */
	public boolean showPrompt( ImageIcon icon, String title, String errMessage, int width, int height )
	{
		this.prompt = true;
		
		this.setUp (  icon,  title,  errMessage );
		
		return this.outcome;
	}


	/**
	 * This sets up the dialog according to what's passed in.
	 * 
	 * @param icon The ImageIcon object
	 * @param title The title of the window.
	 * @param errMessage The message that the user sees.
	 */
	private void setUp( ImageIcon icon, String title, String errMessage )
	{
		
		this.setTitle( title );
		this.setModal( true );
		this.setResizable( false );

		this.setDefaultCloseOperation( JFrame.DISPOSE_ON_CLOSE );

		
		/* create content panel */
		JPanel contentPanel = new JPanel ();
		contentPanel.setLayout(new BorderLayout());
		
		/* create back button in new flowLayout, adjust size and add action listener */
		JPanel backPanel = new JPanel ();
		backPanel.setLayout(new FlowLayout(1));
		
		if( this.prompt == false )
		{
			JButton back = new JButton ("OK");
			back.addActionListener( this );
			backPanel.add(back);
			
		} else {
			
			JButton cont = new JButton ("OK");
			cont.addActionListener( this );
			backPanel.add(cont);
			
			JButton ret = new JButton ("Cancel");
			ret.addActionListener( this );
			backPanel.add(ret);
			
		}
		
		/* Create the text area for the message */
		JTextArea msgTxtBox = new JTextArea ( );

		/* set the line style and properties */
		msgTxtBox.setEditable( false );
		msgTxtBox.setLineWrap( true ); 
		msgTxtBox.setWrapStyleWord( true );
		
		msgTxtBox.setColumns(90);
		msgTxtBox.setTabSize(4);

		/* add the message so we can display it */
		msgTxtBox.setText(errMessage);
		msgTxtBox.setCaretPosition(0);
		
		msgTxtBox.setBackground( this.getBackground() );
		msgTxtBox.setBorder( BorderFactory.createEmptyBorder(5, 5, 5, 5) );
		
		
		/* add a scroll pane in case there is too much stuff */
		JScrollPane scrollPane = new JScrollPane( msgTxtBox , JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED , JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED );
		
		scrollPane.setBorder( BorderFactory.createEmptyBorder() );
		scrollPane.setBackground( this.getBackground() );
		
		/* make the icon */
		JLabel anIcon = new JLabel(icon);
		
		/* add a border to the icon */
		anIcon.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));
		
		/* add components to content panel */
		contentPanel.add(anIcon, BorderLayout.WEST);
		contentPanel.add(scrollPane, BorderLayout.CENTER);
		contentPanel.add(backPanel, BorderLayout.SOUTH);
		
		contentPanel.setBorder(BorderFactory.createEmptyBorder(5, 5, 5, 5));
		this.add(contentPanel);
		

		backPanel.setBackground( this.getBackground() );
		contentPanel.setBackground( this.getBackground() );
		
		/* The following method centers the frame on the screen
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

		this.setVisible( true );
	}

	/**
	 * This is the action listener for the button.
	 */
	public void actionPerformed(ActionEvent e) 
	{
		String event = e.getActionCommand();

		if(event.equalsIgnoreCase("OK"))
		{
			this.outcome = true;
			this.dispose();
		}
		else if(event.equalsIgnoreCase("Cancel"))
		{
			this.outcome = false;
			this.dispose();
		}

	}
}

