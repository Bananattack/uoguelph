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
	private ArrayList<Component> widgets=new ArrayList();

	/**
	 * Creates a ready-to-use WidgetPanel. Just use add() to place widgets in the box.
	 *
	 * @param  comp the widget to be added to the box of widgets
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

		//Each widget will be stretched equally to fill the vertical space
		gbcConstraints.fill=GridBagConstraints.VERTICAL;
		gbcConstraints.weighty=1.0;

		//Next widget will be placed at position (0,0) on the grid
		gbcConstraints.gridx=0;
		gbcConstraints.gridy=0;
		
		//finally, add the layout object to the panel
		myPanel.setLayout(gblLayout);

		//We dont want a vertical scrollbar on the scrollpane, widgets are layed out in columns of 5
		getHorizontalScrollBar().setUnitIncrement(10);
		horizontalScrollBarPolicy = JScrollPane.HORIZONTAL_SCROLLBAR_ALWAYS;
		verticalScrollBarPolicy = JScrollPane.VERTICAL_SCROLLBAR_NEVER;

		//Current arbitrary border style is beveled 
		setBorder(new BevelBorder(BevelBorder.LOWERED));

		//TODO: figure out what dimensions should be for scrollpane
		setPreferredSize(new Dimension(100, 110));
	
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
		if(widgets.size() % 5 == 0)
		{
			gbcConstraints.gridx++;
			gbcConstraints.gridy=0;
		}
		gbcConstraints.gridy++;

		//We want new constraints now, so update the layout
		gblLayout.setConstraints(comp, gbcConstraints);		

		//Allow us to keep track of the widget by adding it
		//to an ArrayList
		widgets.add(comp);

		return myPanel.add(comp);
	}
}
