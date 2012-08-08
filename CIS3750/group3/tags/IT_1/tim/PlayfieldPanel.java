// CIS*3750
// PlayfieldPanel.java

package tim;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.border.*;

/**
 * PlayfieldPanel is where all the action, puzzle editing
 * and so forth occurs.
 *
 * @author Andrew Crowell
 */
public class PlayfieldPanel extends JPanel implements MouseListener
{
	public static final int PLAYFIELD_WIDTH = 500;
	public static final int PLAYFIELD_HEIGHT = 500;

	public PlayfieldPanel()
	{
		super();
		setSize(PLAYFIELD_WIDTH, PLAYFIELD_HEIGHT);
		setMinimumSize(new Dimension(PLAYFIELD_WIDTH, PLAYFIELD_HEIGHT));
		setPreferredSize(new Dimension(PLAYFIELD_WIDTH, PLAYFIELD_HEIGHT));
		setMaximumSize(new Dimension(PLAYFIELD_WIDTH, PLAYFIELD_HEIGHT));
		addMouseListener(this);
		//TODO: Override paint method later instead. For now this lets me place things though.
		setLayout(null);
	}

	// [!!!] IMPORTANT! Since there is currently no backend for widget placement,
	// [!!!] removal should be deferred until a backend storage of widgets is made.

	public void mousePressed(MouseEvent e)
	{
		//TODO: Replace with actual widget placement logic.
		//TODO: Forbid overlap of widgets.
		int x = e.getX();
		int y = e.getY();
		int placeholderIndex = GameWindow.getInstance().getActivePlaceholderIndex();

		// If we left clicked with an item selected and the cursor was on-screen,
		// try to place an item.
		if (e.getButton() == MouseEvent.BUTTON1
			&& placeholderIndex != -1
			&& x >= 0 && x < PLAYFIELD_WIDTH
			&& y >= 0 && y < PLAYFIELD_HEIGHT)
		{

			JLabel label = new JLabel("Widget " + placeholderIndex);
			label.setOpaque(true);
			label.setLocation(x, y);
			label.setSize(100, 100);

			int r = (int) (Math.random() * 6);
			switch(r)
			{
				case 0: label.setBackground(new Color(255, 0, 0, 63)); break;
				case 1: label.setBackground(new Color(0, 255, 0, 63)); break;
				case 2:	label.setBackground(new Color(0, 0, 255, 63)); break;
				case 3: label.setBackground(new Color(255, 255, 0, 63)); break;
				case 4: label.setBackground(new Color(255, 0, 255, 63)); break;
				case 5: label.setBackground(new Color(0, 255, 255, 63)); break;

			}
			add(label);
			
			// Placement complete!
			System.out.println("Placed item " + placeholderIndex + " at (" + x + ", " + y + ")!");

			// If you hold down shift, the active item is remembered for next click.
			// To do this, we use a bitwise AND to check if the shift modifier is on.
			if((e.getModifiersEx() & MouseEvent.SHIFT_DOWN_MASK) == MouseEvent.SHIFT_DOWN_MASK)
			{
				System.out.println("Shift!");
			}
			// Otherwise, the active item is wiped.	
			else
			{
				GameWindow.getInstance().setActivePlaceholderIndex(-1);
			}

 			repaint();
		}
	}

	public void mouseReleased(MouseEvent e)
	{
	}

	public void mouseEntered(MouseEvent e)
	{
	}

	public void mouseExited(MouseEvent e)
	{
	}

	public void mouseClicked(MouseEvent e)
	{
	}
}
