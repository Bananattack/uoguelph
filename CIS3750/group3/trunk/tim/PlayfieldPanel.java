// CIS*3750
// PlayfieldPanel.java

package tim;

import Widgets.*;

import java.awt.*;
import java.awt.event.*;
import java.awt.geom.Rectangle2D;
import java.util.ArrayList;

import javax.swing.*;
import javax.swing.border.*;

import net.phys2d.math.Vector2f;
import net.phys2d.raw.Body;
import net.phys2d.raw.Joint;

/**
 * PlayfieldPanel is where all the action, puzzle editing
 * and so forth occurs.
 *
 * @author Andrew Crowell
 */
public class PlayfieldPanel extends JPanel
{
	/**
	 *
	 */
	private static final long serialVersionUID = 1L;
	private GameController controller = null;
	public static final int PLAYFIELD_WIDTH = 500;
	public static final int PLAYFIELD_HEIGHT = 500;



	/**
	 * Instantiates a new playfield panel.
	 */
	public PlayfieldPanel()
	{
		super();
	}

	/**
	 * Instantiates a new playfield panel.
	 *
	 * @param controller the controller
	 */
	public PlayfieldPanel(GameController controller)
	{
		super();

		this.setBackground( Color.WHITE );
		this.controller = controller;
		this.show();
	}

	/**
	 * Sets the controller.
	 *
	 * @param c the new controller
	 */
	public void setController(GameController c)
	{
		this.controller = c;
	}

	/**
	 * Gets the controller.
	 *
	 * @return the controller
	 */
	public GameController getController()
	{
		return controller;
	}


	/* (non-Javadoc)
	 * @see java.awt.Component#show()
	 */
	public void show()
	{
		this.setBackground( Color.WHITE );
		this.setSize(PLAYFIELD_WIDTH, PLAYFIELD_HEIGHT);
		this.setMinimumSize(new Dimension(PLAYFIELD_WIDTH, PLAYFIELD_HEIGHT));
		this.setPreferredSize(new Dimension(PLAYFIELD_WIDTH, PLAYFIELD_HEIGHT));
		this.setMaximumSize(new Dimension(PLAYFIELD_WIDTH, PLAYFIELD_HEIGHT));

		/* Trying to make it look better */
		this.setBorder(new EtchedBorder());

		PlayfieldMouse mouseListener = new PlayfieldMouse( controller, controller.getBuilder() );

		this.addMouseListener(mouseListener);
		this.addMouseMotionListener(mouseListener);
	}

	/* (non-Javadoc)
	 * @see javax.swing.JComponent#paintComponent(java.awt.Graphics)
	 */
	public void paintComponent( Graphics g )
	{
		super.paintComponent(g);
		controller.render((Graphics2D) g);
	}

	/* (non-Javadoc)
	 * @see javax.swing.JComponent#getWidth()
	 */
	public int getWidth()
	{
		int temp = PLAYFIELD_WIDTH;
		return temp;
	}

	/* (non-Javadoc)
	 * @see javax.swing.JComponent#getHeight()
	 */
	public int getHeight()
	{
		int temp = PLAYFIELD_HEIGHT;
		return temp;
	}


}
