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
 */
public class WidgetPanelButton extends JButton implements ActionListener
{
	private Widget widget;
	private GameController controller = null;

	public WidgetPanelButton(Widget widget, Icon icon, GameController controller)
	{
		super();

		this.setIcon( icon );
		
		this.controller = controller;
		this.widget = widget;

		// Phil (2008-10-20): Fixed buttons size to 100x100, got rid of settings that had no effect
		setPreferredSize(new Dimension(100, 100));
		setMinimumSize(new Dimension(100, 100));

		setBorder(BorderFactory.createEmptyBorder(0, 0, 0, 0));
		addActionListener(this);
	}

	/**
	 * Returns the widget assoiated with this WidgetPanelButton
	 * @return Widget the widget that belongs to this button
	 */
	public Widget getWidget()
	{
		return widget;
	}

	public void actionPerformed(ActionEvent e)
	{
		controller.setSelectedPiece(widget);
		controller.setNewPiece(true);
	}
}
