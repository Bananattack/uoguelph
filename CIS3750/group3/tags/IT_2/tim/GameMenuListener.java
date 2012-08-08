/*
 * GameMenuListener.java
 * 
 * Created by Filip Anguelov on 2008-10-19.
 * E-mail: fanguelo@uoguelph.ca
 * 
 * Class that listens to menu clicks and does stuff
 * 
 */
package tim;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ItemEvent;
import java.awt.event.ItemListener;

// TODO: Auto-generated Javadoc
/**
 * The listener interface for receiving gameBar events.
 * The class that is interested in processing a gameBar
 * event implements this interface, and the object created
 * with that class is registered with a component using the
 * component's <code>addGameBarListener<code> method. When
 * the gameBar event occurs, that object's appropriate
 * method is invoked.
 * 
 * @see GameBarEvent
 */
public class GameMenuListener implements ActionListener, ItemListener
{
	
	/**
	 * Instantiates a new game menu listener.
	 */
	public GameMenuListener()
	{
		
	}

	/* (non-Javadoc)
	 * @see java.awt.event.ItemListener#itemStateChanged(java.awt.event.ItemEvent)
	 */
	public void itemStateChanged( ItemEvent e )
	{
		// TODO Auto-generated method stub
		
	}

	/* (non-Javadoc)
	 * @see java.awt.event.ActionListener#actionPerformed(java.awt.event.ActionEvent)
	 */
	public void actionPerformed( ActionEvent e )
	{
		String selectedMenu = e.getActionCommand();

		/* when the user invokes quit*/
		if ( selectedMenu.equalsIgnoreCase( "Quit" ) ) 
		{
		
			/* call a function to see if there needs to be any saving then quit etc */
		
		} 
		
		else if ( selectedMenu.equalsIgnoreCase( "High Scores" ) ) 
		{
			
			/* call a to pop up the High Scores */
			
		}
		
		else if ( selectedMenu.equalsIgnoreCase( "Save As" ) ) 
		{
			
			/* call a to Save As */
			
		}
		
		else if ( selectedMenu.equalsIgnoreCase( "Save" ) ) 
		{
			
			/* call a to Save */
			
		}
		
		else if ( selectedMenu.equalsIgnoreCase( "Open" ) ) 
		{
			
			/* call a to Open saved game */
			
		}
		
		else if ( selectedMenu.equalsIgnoreCase( "New" ) ) 
		{
			
			/* call a to pop up the New game window */
			
		}
		
		else if ( selectedMenu.equalsIgnoreCase( "Prefferences" ) ) 
		{
			GameSettingsChanger setGame = new GameSettingsChanger();
			
			/* in the array air pressure is at position 0 and gravity at 1 */
			int[] temp = setGame.show( GameWindow.getInstance().getAirPressure(), GameWindow.getInstance().getGravity()  );
			
			System.out.println("The Air Pressure is now: " + temp[0] + "\nThe Gravity is now: " + temp[1]);
			GameWindow.getInstance().setAirPressure( temp[0] );
			GameWindow.getInstance().setGravity( temp[1] );
			/* call a to pop up the Gravity */
			
		}
		
		else if ( selectedMenu.equalsIgnoreCase( "About" ) ) 
		{
			
			/* call a to pop up the About */
			
		}
		
		else if ( selectedMenu.equalsIgnoreCase( "Game Help" ) ) 
		{
			
			HelpGUI test = new HelpGUI();
			test.setVisible(true);
			
		}

		
	}

}
