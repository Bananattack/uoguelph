/*
 * ActiveButton.java
 *
 * Written by Filip Anguelov
 * ID: 0223807
 * E-mail: fanguelo@uoguelph.ca
 *
 * A class that defines a custom button containing a widget, its image
 * and an encapsulating panel. This button is added to the widget panel
 *
 *
 */
package tim;



import javax.swing.JLabel;
import javax.swing.JPanel;


// TODO: Auto-generated Javadoc
/**
 * The Class ActiveButton.
 *
 * @author Filip Anguelov & Kevin Strype
 */
public class ActiveButton
{

	/* The button panel. */
	/** The button panel. */
	private JPanel buttonPanel = null;

	/* The widget button. */
	/** The widget button. */
	private WidgetPanelButton widgetButton = null;

	/* The panel label. */
	/** The panel label. */
	private JLabel panelLabel = null;

	/*this is the number opf widgets left for you to place in puzzle mode*/
	/** The amount remains. */
	private int amountRemains;


	/*this is teh amount of total widgets avaiable to place, defualts to infinate*/
	/** The total. */
	private int total;



	/**
	 * Instantiates a new active button.
	 *
	 * @param panel the panel
	 * @param button the button
	 * @param label the label
	 */
	protected ActiveButton(JPanel panel, WidgetPanelButton button, JLabel label)
	{
		amountRemains = 10;
		total = 10;
		this.buttonPanel = panel;
		this.widgetButton = button;
		this.panelLabel = label;

	}

	/**
	 * Gets the button.
	 *
	 * @return Returns a JButton to the caller
	 */
	protected WidgetPanelButton getButton()
	{
		return this.widgetButton;
	}

	/**
	 * Gets the panel.
	 *
	 * @return Returns the panel that encapsulates the JButton
	 */
	protected JPanel getPanel()
	{
		return this.buttonPanel;
	}

	/**
	 * Gets the label.
	 *
	 * @return the label
	 *
	 * Returns the JLabel assosicated with the button
	 */
	protected JLabel getLabel()
	{
		return this.panelLabel;

	}

	/**
	 * Sets the label.
	 *
	 * @param label Sets the label that will appear below the button
	 */
	protected void setLabel( JLabel label )
	{
		this.panelLabel = label;
	}

	/**
	 * Sets the button.
	 *
	 * @param button The new button that will be part of the ActiveButton
	 */
	protected void setButton( WidgetPanelButton button )
	{
		this.widgetButton = button;

	}

	/**
	 * Sets the panel.
	 *
	 * @param panel The new panel encapsulates the button and label
	 */
	protected void setPanel( JPanel panel )
	{
		this.buttonPanel = panel;
	}


	/**
	 * Gets the remaining widgets.
	 *
	 * @return The remaining widgets that can be placed.
	 */
	protected int getRemainingAmount()
	{
		return amountRemains;
	}

	/**
	 * Setup the total amount of peaceable widgets in puzzle mode.
	 *
	 * @param amount The total amount of widgets to allowed place on the play area. (-1 means infinite)
	 */
	protected void setTotalAmount(int amount)
	{
		total = amount;

		this.updateLabel();
	}

	/**
	 * Getter for the total amount of remaining widgets to be place on the level
	 *
	 * @return Returns the total amount of available widgets.
	 */
	protected int getTotalAmount()
	{
		return total;
	}


	/**
	 * Decrement amount.
	 *
	 * @return true, if successful
	 */
	protected boolean decrementAmount()
	{
		if(amountRemains > 0)
		{
			amountRemains = amountRemains -1;
		}
		else
		{
			return false;
		}

		this.updateLabel();

		return true;
	}


	/**
	 * Increment amount.
	 *
	 * @return true, if successful
	 */
	protected boolean incrementAmount()
	{
		if(amountRemains < total)
		{
			amountRemains = amountRemains +1;
		}
		else
		{
			return false;
		}

		this.updateLabel();
		return true;
	}

	/**
	 * Reset amount of available widgets
	 */
	protected void resetAmount()
	{
		this.amountRemains = this.total;
		this.updateLabel();
	}

	/**
	 * Update label.
	 */
	public void updateLabel()
	{
		String s = widgetButton.getWidget().getName();
		if(getTotalAmount() >= 0)
		{
			s = getRemainingAmount() + " x " + s;
		}
		panelLabel.setText(s);
	}

}
