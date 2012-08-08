/*
 *  GameToolBar.java
 *
 *  Created by Filip Anguelov on 2008-10-20.
 *  E-mail: fanguelo@uoguelph.ca
 *
 *  Creates a toolbar with buttons for the game
 *
 */
package tim;

import java.awt.event.*;
import java.io.File;
import java.io.IOException;

import javax.swing.Icon;
import javax.swing.JButton;
import javax.swing.JToolBar;
import javax.swing.JOptionPane;


/**
 * The Class GameToolBar.
 *
 * @author Filip Anguelov
 */
@SuppressWarnings("serial")
public class GameToolBar extends JToolBar
{

	/* The file reader. */
	private FileIO fileReader = null;

	/* The icon maker. */
	private IconMaker iconMaker = null;

	/* The icon path. */
	private static String iconPath = "resources/icons/toolbar/";

	/**
	 * Instantiates a new game tool bar.
	 */
	public GameToolBar()
	{

		/* making various buttons */
		ToolBarButton run = new ToolBarButton("Run", "run");
		run.addActionListener(new ActionListener() {
				public void actionPerformed(ActionEvent e)
				{
					GameWindow.getInstance().play();
				}
			});
		this.add(run);
		
		ToolBarButton pause = new ToolBarButton("Pause", "pause");
		pause.addActionListener(new ActionListener() {
				public void actionPerformed(ActionEvent e)
				{
					GameWindow.getInstance().pause();
				}
			});
		this.add(pause);

		ToolBarButton stop = new ToolBarButton("Stop", "stop");
		stop.addActionListener(new ActionListener() {
				public void actionPerformed(ActionEvent e)
				{
					GameWindow.getInstance().stop();
				}
			});
		this.add(stop);

		this.addSeparator();

		ToolBarButton ccw = new ToolBarButton("Rotate Counter Clockwise", "countclockwise");
		ccw.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e)
			{
				GameWindow.getInstance().rotateCC();
			}
		});
		
		this.add(ccw);

		ToolBarButton cw = new ToolBarButton("Rotate Clockwise", "clockwise");
		this.add(cw);

		this.addSeparator();

		ToolBarButton rs = new ToolBarButton("Reset Game", "reset");
		rs.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e)
			{
				GameWindow.getInstance().reset();
			}
		});
		this.add(rs);
		
		ToolBarButton cl = new ToolBarButton("Clear", "clear");
		cl.addActionListener(new ActionListener() 
		{
			public void actionPerformed(ActionEvent e)
			{
				
					GameWindow.getInstance().clear();
			}
		});
		
		this.add(cl);

		ToolBarButton del = new ToolBarButton("Delete", "delete");
		/* Jamie (2008-10-28) Added a confirmation box for deletion*/
		del.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e)
			{
				GameWindow.getInstance().deleteWidget( );
			}
		});
		this.add(del);
	}

	/**
	 * The Class ToolBarButton.
	 * ToolBarButton is an easier way to make a toolbarbutton
	 * it creates the button with text and an Icon all in one
	 */
	private class ToolBarButton extends JButton
	{

		/**
		 * Instantiates a new tool bar button.
		 *
		 * @param altText the alt text
		 * @param iconName the icon name
		 */
		public ToolBarButton(String altText, String iconName)
		{

			iconMaker =  new IconMaker();

			try
			{
				File current = new File(new File(".").getCanonicalPath());
				fileReader = new FileIO(current);

			} catch (IOException e1) {}

			/* set the name of the button */
			super.setActionCommand(altText);

			/* set the alt text */
			super.setToolTipText(altText);

			/* remove the border around the button */
			super.setBorderPainted( false );

			/* Make an icon */
			Icon  theIcon = iconMaker.makeIcon(fileReader.getGameDir() + iconPath + iconName +".png");

			/*if the Icon cannot be found put the name of the button instead*/
			if ( theIcon != null )
				super.setIcon( theIcon );
			else
				super.setText( altText );

		}
	}
}
