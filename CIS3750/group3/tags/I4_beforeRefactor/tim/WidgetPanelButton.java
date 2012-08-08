// CIS*3750
// WidgetPanelButton.java
// by Andrew Crowell
//// Currently this only represents an ad-hoc front-end representation.
// Requires refactoring later when more back-end stuff is made.

package tim;

import Widgets.Widget;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.border.EtchedBorder;


/**
 * WidgetPanelButton is an item in the widget panel, which is
 * used to select an active play piece.
 *
 * @author Andrew Crowell
 * @TODO keep track of how many widgets are left
 */
public class WidgetPanelButton extends JButton implements ActionListener
{
	private int numStack;
	private Widget w;

	/* Darren O'Shea (2008-10-19)
	 *
	 * Added Widget support to the widget button
	 *
	 */
	public WidgetPanelButton(Widget inW, int inNumStack)
	{
		super(inW.getIcon());

		w = inW;
		numStack = inNumStack;
		//setBorder( BorderFactory.createLineBorder(Color.GRAY) );
		/* Phil (2008-10-20) Made buttons have constant dimensions */
		/* Andy (2008-10-20) Adjusted the dimensions to be 100x100,
		 * which is the max imageicon size the Widget API claims to allow.
		 */
		
		/* Phil (2008-10-20) : fixed buttons size to 100x100 got rid of
		 * settings that had no effect
		 */
		setPreferredSize( new Dimension ( 100, 100) );
		setMinimumSize( new Dimension (100, 100) );
		
		setBorder( BorderFactory.createEmptyBorder( 0, 0, 0, 0 ) );
		addActionListener(this);
	}

	public WidgetPanelButton(Widget inW)
	{
		this(inW, 1);
	}


	public void actionPerformed(ActionEvent e)
	{
		System.out.println("Selected item " + w);
		GameWindow.getInstance().setGamePiece(w);
		GameWindow.getInstance().setNew( true );
	}
}
