/*
 *  GameMenuBar.java
 *
 *  Created by Filip Anguelov on 03/10/08.
 *  E-mail: fanguelo@uoguelph.ca
 *
 *  Simple class to simplify the creation of menus for a given window
 *
 *  Some parts are taken from respective tutorials online. See below.
 *
 */

package tim;

import java.awt.Event;
import java.awt.Toolkit;
import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;
import java.awt.event.KeyEvent;

import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.KeyStroke;

/**
 *
 * @author Filip Anguelov
 *
 */
@SuppressWarnings("serial")
public class GameMenuBar extends JMenuBar
{

	/* global for the listener we use later */
	private ActionListener theListener = null;
	
	/**
	 * Empty constructor
	 */
	public GameMenuBar()
	{
		super();
	}


	/**
	 * This sets the action listener for the items
	 * @param action Non null ActionListener object
	 */
	public void setActionListener( ActionListener action )
	{
		this.theListener = action;
	}

	/**
	 * Returns the action listener in use. Might as well have it
	 * if there is set.
	 * @return Returns an ActionListener object.
	 */
	public ActionListener getActionListener()
	{
		return this.theListener;
	}


	/**
	 * This function sets all the menu items in order.
	 */
	public void setAll()
	{
		this.setFileMenus();
		this.setConfigMenus();
		this.setHelp();
	}

	/**
	 * This function just sets the File menu
	 *
	 * Needed help with getting the Apple accelerators
	 *
	 * Resources
	 * ---------------------
	 * Author: Apple Inc.
	 * URL: http://developer.apple.com/documentation/Java/Conceptual/Java14Development/07-NativePlatformIntegration/NativePlatformIntegration.html
	 */
	public void setFileMenus()
	{
		/* create a JMenu item, the root of the menu */
		JMenu fileMenu = new JMenu("File");

		/* this sets the key that can be pressed in order to raise the menu after hitting the ALT key */
		fileMenu.setMnemonic(KeyEvent.VK_F);

		/* a tool tip for help */
		fileMenu.getAccessibleContext().setAccessibleDescription("The only menu in this program that has menu items");


		/* set the item under the menu */
		JMenuItem newFile = new JMenuItem("New",KeyEvent.VK_N);

		/* this sets the accelerator the shortcut for the item, it is similar to CTRL+N, but on mac it' the Apple key */
		newFile.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_N, Event.META_MASK));
		newFile.getAccessibleContext().setAccessibleDescription("This creates a new game");

		/* add the listener for this item */
		newFile.addActionListener(this.theListener);


		JMenuItem open = new JMenuItem("Open", KeyEvent.VK_O);
		open.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_O, Event.META_MASK));
		open.getAccessibleContext().setAccessibleDescription("Opens an existing game");
		open.addActionListener(this.theListener);


		JMenuItem save = new JMenuItem("Save", KeyEvent.VK_S);
		save.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_S, Event.META_MASK));
		save.getAccessibleContext().setAccessibleDescription("Save current game");
		save.addActionListener(this.theListener);


		JMenuItem saveAs = new JMenuItem("Save As", KeyEvent.VK_V);
		saveAs.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_S, (java.awt.event.InputEvent.SHIFT_MASK | (Toolkit.getDefaultToolkit().getMenuShortcutKeyMask()))));
		saveAs.getAccessibleContext().setAccessibleDescription("Save current game as");
		saveAs.addActionListener(this.theListener);

		JMenuItem scoreList = new JMenuItem("High Scores", KeyEvent.VK_I);
		scoreList.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_I, Event.META_MASK));
		scoreList.getAccessibleContext().setAccessibleDescription("Get a list of top players");
		scoreList.addActionListener(this.theListener);


		JMenuItem quit = new JMenuItem("Quit", KeyEvent.VK_Q);
		quit.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_Q, Event.META_MASK));
		quit.getAccessibleContext().setAccessibleDescription("Quit the game");
		quit.addActionListener(this.theListener);
		

		/* after creating the items we add them to the menu */
		fileMenu.add(newFile);
		fileMenu.add(open);
		fileMenu.add(save);
		fileMenu.add(saveAs);
		fileMenu.addSeparator();
		fileMenu.add(scoreList);
		fileMenu.addSeparator();
		fileMenu.add(quit);

		/* after adding the items the menu is then added to the MenuBar */
		this.add(fileMenu);
	}

	/**
	 * Sets the Configure menus
	 */
	public void setConfigMenus()
	{
		/* create a JMenu item, the root of the menu */
		JMenu cfgMenu = new JMenu("Configure");

		/* this sets the key that can be pressed in order to raise the menu after hitting the ALT key */
		// Andy (2008-10-07): Changed Mnemonic to be different from file menu.
		cfgMenu.setMnemonic(KeyEvent.VK_O);

		/* a tool tip for help */
		cfgMenu.getAccessibleContext().setAccessibleDescription("Set the game variables");

		JMenuItem cfgPref = new JMenuItem("Preferences",KeyEvent.VK_P);
		cfgPref.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_P, Event.META_MASK));
		cfgPref.getAccessibleContext().setAccessibleDescription("This adjusts the gravity");
		cfgPref.addActionListener(this.theListener);


		/* after creating the items we add them to the menu */
		cfgMenu.add(cfgPref);


		this.add(cfgMenu);
	}


	public void setHelp()
	{

		/* create a JMenu item, the root of the menu */
		JMenu hlpMenu = new JMenu("Help");

		/* this sets the key that can be pressed in order to raise the menu after hitting the ALT key */
		hlpMenu.setMnemonic(KeyEvent.VK_H);

		/* a tool tip for help */
		hlpMenu.getAccessibleContext().setAccessibleDescription("Set the game variables");

		JMenuItem hlpHelp = new JMenuItem("Game Help",KeyEvent.VK_H);
		hlpHelp.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_H, Event.META_MASK));
		hlpHelp.getAccessibleContext().setAccessibleDescription("Learn how to play and use the program");
		hlpHelp.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e)
			{
				HelpGUI help = new HelpGUI();
				help.setVisible(true);
			}
		});

		JMenuItem hlpAbout = new JMenuItem("About", KeyEvent.VK_B);
		hlpAbout.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_B, Event.META_MASK));
		hlpAbout.getAccessibleContext().setAccessibleDescription("Who made this mess");
		
		//Jamie (2008-10-23): Added my About class!  Yeehaa.
		hlpAbout.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e)
			{
				AboutGUI about = new AboutGUI();
				about.setVisible(true);
			}
		});

		/* after creating the items we add them to the menu */
		hlpMenu.add(hlpHelp);
		hlpMenu.addSeparator();
		hlpMenu.add(hlpAbout);

		/* after adding the items the menu is then added to the MenuBar */
		this.add(hlpMenu);

	}


}
