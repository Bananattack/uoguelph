// CIS*3750
// WidgetPanel.java
// by Darren O'Shea

// Andy (2008-10-01): Refactored class structure to be in package.
package tim;

import java.awt.event.*;
import javax.swing.*;
import javax.swing.border.*;
import java.awt.*;
import java.util.*;

/**
 * WidgetPanel is the widget box used in TIM. It displays widgets
 * in columns, 5 widgets tall, with a horizontal scroll bar for
 * browsing available widgets.
 *
 * @author      Darren O'Shea
 * @version     %I%, %G%
 * @since       1.6
 */
public class WidgetPanel extends JScrollPane
{
	private JPanel myPanel;
	private GridBagConstraints gbcConstraints;
	private GridBagLayout gblLayout;
	private ArrayList<Component> widgets = new ArrayList();

	private PlayfieldPanel panel = null;

	/**
	 * Creates a ready-to-use WidgetPanel. Just use add() to place widgets in the box.
	 *
	 * @return      the same component that was added
	 */
	public WidgetPanel()
	{


		//Let JScrollPane do any initializing it needs to
		super();

		//add the JPanel that will serve as our viewport
		myPanel = new JPanel();

		//create the layout using GridBagLayout style
		gblLayout = new GridBagLayout();


		//GridBagLayout needs constraints to place the widgets properly
		gbcConstraints = new GridBagConstraints();

		//Next widget will occupy 1 grid cell wide and tall
		gbcConstraints.gridheight=1;
		gbcConstraints.gridwidth=1;

		/* Phil (2008-10-20): This creates space around buttons */
		gbcConstraints.insets = new Insets(2, 2, 2, 2);

		/* Phil (2008-10-20): instead of stretching why not align vertically somehow? */
		//Each widget will be stretched equally to fill the vertical space
		//gbcConstraints.fill=GridBagConstraints.VERTICAL;
		//gbcConstraints.weighty=1.0;

		//Next widget will be placed at position (0,0) on the grid
		gbcConstraints.gridx=0;
		gbcConstraints.gridy=0;


		//finally, add the layout object to the panel
		myPanel.setLayout(gblLayout);

		//We dont want a vertical scrollbar on the scrollpane, widgets are layed out in columns of 5
		getHorizontalScrollBar().setUnitIncrement( 100 );
		getVerticalScrollBar().setUnitIncrement( 140 );
		horizontalScrollBarPolicy = JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED;
		verticalScrollBarPolicy = JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED;

		//Current arbitrary border style is etched
		setBorder(new EtchedBorder());

		//TODO: figure out what dimensions should be for scrollpane
		/* Phil (2008-10-20): Size of two widgets plus overhead (setting panel to 287 and game window to 800 gives
		 * us a game panel of 500) */
		setPreferredSize(new Dimension(287, 110));

		//Now that our JPanel is set up, give it to the scroll pane
		setViewportView(myPanel);

	}

	/**
	 * Adds a widget to the box of widgets. Overrides the usual add() method.
	 *
	 * @param  comp the widget to be added to the box of widgets
	 * @return      the same component that was added
	 */
	public Component add(Component comp)
	{

		//If we now have 5 widgets in a column, make a new column
		/* Phil (2008-10-23): changed to two colums max */
		if(widgets.size() % 2 == 0)
		{
			gbcConstraints.gridy++;
			gbcConstraints.gridx=0;
		}
		gbcConstraints.gridx++;

		//We want new constraints now, so update the layout
		gblLayout.setConstraints(comp, gbcConstraints);

		//Allow us to keep track of the widget by adding it
		//to an ArrayList
		widgets.add(comp);

		Component c = myPanel.add(comp);
		validate();
		return c;
	}

	/**
	 * Clears widgets from the panel
	 *
	 */
	public void clear()
	{
		gbcConstraints.gridy = 0;
		gbcConstraints.gridx = 0;

		//Allow us to keep track of the widget by adding it
		//to an ArrayList
		widgets.clear();

		myPanel.removeAll();
		validate();
	}

	public void setEnabled(boolean enabled)
	{
		setVisible(enabled);
	}
}
