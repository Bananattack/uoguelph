// CIS*3750
// PlayfieldPanel.java

package tim;

import Widgets.*;

import java.awt.*;
import java.awt.event.*;
import java.util.ArrayList;

import javax.swing.*;
import javax.swing.border.*;

import net.phys2d.math.Vector2f;

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

	public static final int PLAYFIELD_WIDTH = 500;
	public static final int PLAYFIELD_HEIGHT = 500;
	
	private WidgetBuilder builder = null;
	

	public PlayfieldPanel()
	{
		super();
		
		
		
		this.setBuilder( new WidgetBuilder() );
		this.setSize(PLAYFIELD_WIDTH, PLAYFIELD_HEIGHT);
		this.setMinimumSize(new Dimension(PLAYFIELD_WIDTH, PLAYFIELD_HEIGHT));
		this.setPreferredSize(new Dimension(PLAYFIELD_WIDTH, PLAYFIELD_HEIGHT));
		this.setMaximumSize(new Dimension(PLAYFIELD_WIDTH, PLAYFIELD_HEIGHT));
		
		/* Trying to make it look better */
		this.setBorder(new EtchedBorder());
		this.setBackground( Color.WHITE );
		
		
		PlayfieldMouse mouseListener = new PlayfieldMouse(this, builder);
		
		this.addMouseListener(mouseListener);
		this.addMouseMotionListener(mouseListener);

	}

	public void paintComponent(Graphics g)
	{
		super.paintComponent(g);
		GameWindow.getInstance().render(g);
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

	/**
	 * @param builder the builder to set
	 */
	public void setBuilder( WidgetBuilder builder )
	{
		this.builder = builder;
	}

	/**
	 * @return the builder
	 */
	public WidgetBuilder getBuilder( )
	{
		return builder;
	}
	



	


	


}
