/*
 * 
 */
package tim;

import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JPanel;

// TODO: Auto-generated Javadoc
/**
 * The Class ActiveButton.
 * 
 * @author Filip Anguelov
 *
 */
public class ActiveButton
{
	
	/** The button panel. */
	private JPanel buttonPanel = null;
	
	/** The widget button. */
	private JButton widgetButton = null;
	
	/** The panel label. */
	private JLabel panelLabel = null;
	
	/**
	 * Instantiates a new active button.
	 * 
	 * @param panel the panel
	 * @param button the button
	 * @param label the label
	 */
	protected ActiveButton(JPanel panel, JButton button, JLabel label)
	{
		this.buttonPanel = panel;
		this.widgetButton = button;
		this.panelLabel = label;
		
	}
	
	/**
	 * Gets the button.
	 * 
	 * @return the button
	 */
	protected JButton getButton()
	{
		return this.widgetButton;
	}
	
	/**
	 * Gets the panel.
	 * 
	 * @return the panel
	 */
	protected JPanel getPanel()
	{
		return this.buttonPanel;
	}
	
	/**
	 * Gets the label.
	 * 
	 * @return the label
	 */
	protected JLabel getLabel()
	{
		return this.panelLabel;		
	}
	
	/**
	 * Sets the label.
	 * 
	 * @param label the new label
	 */
	protected void setLabel( JLabel label )
	{
		this.panelLabel = label;
	}
	
	/**
	 * Sets the button.
	 * 
	 * @param button the new button
	 */
	protected void setButton( JButton button )
	{
		this.widgetButton = button;
	}
	
	/**
	 * Sets the panel.
	 * 
	 * @param panel the new panel
	 */
	protected void setPanel( JPanel panel )
	{
		this.buttonPanel = panel;
	}

}
