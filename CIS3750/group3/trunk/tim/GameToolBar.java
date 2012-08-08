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
public class GameToolBar extends JToolBar
{
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
			setActionCommand(altText);
			/* set the alt text */
			setToolTipText(altText);
			/* Some people hate borders */
			setBorderPainted(false);
			setContentAreaFilled(false);

			/* Make an icon */
			Icon icon = iconMaker.makeIcon(fileReader.getGameDir() + iconPath + iconName +".png");

			/*if the Icon cannot be found put the name of the button instead*/
			if (icon != null)
			{
				setIcon(icon);
			}
			else
			{
				setText(altText);
			}

			setHorizontalTextPosition(CENTER);
			setVerticalTextPosition(BOTTOM);
			setMargin(new java.awt.Insets(0, 0, 0, 0));

		}
	}


	/* The file reader. */
	private FileIO fileReader = null;

	/* The icon maker. */
	private IconMaker iconMaker = null;

	/* The icon path. */
	private static String iconPath = "resources/icons/toolbar/";
	
	/* The lock widget button */
	ToolBarButton lock = null;

	/**
	 * Instantiates a new game tool bar.
	 */
	public GameToolBar( final GameController c )
	{
		setFloatable(false);

		/* making various buttons */
		ToolBarButton run = new ToolBarButton("Run", "run");
		run.addActionListener(new ActionListener() {
				public void actionPerformed(ActionEvent e)
				{
				 c.startGame();
				}
			});
		this.add(run);
		
		ToolBarButton pause = new ToolBarButton("Pause", "pause");
		pause.addActionListener(new ActionListener() {
				public void actionPerformed(ActionEvent e)
				{
					c.pauseGame();
				}
			});
		this.add(pause);

		ToolBarButton stop = new ToolBarButton("Stop", "stop");
		
		stop.addActionListener(new ActionListener() {
				public void actionPerformed(ActionEvent e)
				{
					c.stopGame();
				}
			});
		this.add(stop);

		this.addSeparator();

		ToolBarButton ccw = new ToolBarButton("Rotate Counter Clockwise", "countclockwise");
		ccw.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e)
			{
				c.rotateLeft();
			}
		});
		
		this.add(ccw);

		ToolBarButton cw = new ToolBarButton("Rotate Clockwise", "clockwise");
		
		cw.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e)
			{
				c.rotateRight();
			}
		});
		
		this.add(cw);
				
		this.lock = new ToolBarButton("Lock", "lock");
		this.lock.addActionListener(new ActionListener() 
		{
			public void actionPerformed(ActionEvent e)
			{

				c.lockSelected();
			}
		});
		this.add(this.lock);

		this.addSeparator();
		ToolBarButton del = new ToolBarButton("Delete", "delete");
		/* Jamie (2008-10-28) Added a confirmation box for deletion*/
		del.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e)
			{
				c.deleteWidget();
			}
		});
		this.add(del);

		this.addSeparator();

		ToolBarButton cl = new ToolBarButton("Clear", "clear");
		cl.addActionListener(new ActionListener() 
		{
			public void actionPerformed(ActionEvent e)
			{

				c.clearGame();
			}
		});
		
		this.add(cl);
	}
	
	/**
	 * Disable lock.
	 */
	public void disableLock()
	{
		this.lock.setEnabled( false );
	}
	
	/**
	 * Enable lock.
	 */
	public void enableLock()
	{
		this.lock.setEnabled( true );
	}
}
