// CIS*3750
// WidgetPanelButton.java
// by Andrew Crowell
//// Currently this only represents an ad-hoc front-end representation.
// Requires refactoring later when more back-end stuff is made.
// TODO: Relate to actual widgets and get rid of "placeholderIndex".
// TODO: Generate icons/thumbnails from actual Widgets.

package tim;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;


/**
 * WidgetPanelButton is an item in the widget panel, which is
 * used to select an active play piece.
 *
 * @author Andrew Crowell
 */
public class WidgetPanelButton extends JButton implements ActionListener
{
	private int placeholderIndex;
	public WidgetPanelButton(int placeholderIndex)
	{
		super("Widget" + placeholderIndex);
		this.placeholderIndex = placeholderIndex;
		addActionListener(this);
	}

	public void actionPerformed(ActionEvent e)
	{
		System.out.println("Selected item " + placeholderIndex);
		GameWindow.getInstance().setActivePlaceholderIndex(placeholderIndex);
	}
}
