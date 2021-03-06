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



	public PlayfieldPanel()
	{
		super();
	}

	public PlayfieldPanel(GameController controller)
	{
		super();

		this.setBackground( Color.white );
		this.controller = controller;
		this.show();
	}

	public void setController(GameController c)
	{
		this.controller = c;
	}


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

	public void paintComponent(Graphics g)
	{
		super.paintComponent(g);
		
		Graphics2D graph = (Graphics2D) g;

		controller.render(graph);
		
	//	GameWindow.getInstance().render(g);
	}

	public int getWidth()
	{
		int temp = PLAYFIELD_WIDTH;
		return temp;
	}

	public int getHeight()
	{
		int temp = PLAYFIELD_HEIGHT;
		return temp;
	}


}
