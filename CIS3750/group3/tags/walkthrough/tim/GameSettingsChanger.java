/*
 *  GameSettingsChanger.java
 *
 *  Created by Filip Anguelov on 2008-10-19.
 *  E-mail: fanguelo@uoguelph.ca
 *
 * Creates a dialog to change the games parameters
 *
 */
package tim;

import java.awt.BorderLayout;

import java.awt.Dimension;
import java.awt.Font;
import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.BorderFactory;
import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JFrame;

import javax.swing.JPanel;
import javax.swing.JSeparator;
import javax.swing.JSlider;
import javax.swing.JTextField;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;


/**
 * @author Filip Anguelov
 */
@SuppressWarnings("serial")
public class GameSettingsChanger extends JDialog implements ActionListener
{

	/* The air value. */
	private int airValue = 0;
	
	/* The gravity value. */
	private int gravValue = 0;
	
	/* The current grav label. */
	private JTextField currentGravLabel = null;
	
	/* The current air label. */
	private JTextField currentAirLabel = null;
	
	private JSlider airSlider = null;
	private JSlider gravSlider = null;
	
	/* The array of values to be returned */
	private int[] values = { 0,0 };
	
	
	/* The Constant SET_MIN/MAX is sued for the sliders min max range */
	private static final int SET_MIN = -50;
	private static final int SET_MAX = 50;
	
	
	/**
	 * Instantiates a new game settings changer.
	 */
	public GameSettingsChanger()
	{
		super();
		super.setSize( 350, 250 );
		super.setTitle( "Set game parameters" );
		super.setModal( true );
		super.setResizable( false );
		super.setDefaultCloseOperation( JFrame.DISPOSE_ON_CLOSE );
	}
	

	
	/**
	 * Show the dialog to the screen
	 * 
	 * @param currentAir the current air pressure
	 * @param currentGrav the current world gravity
	 */
	public int[] show(int currentAir, int currentGrav)
	{
		this.airValue = currentAir;
		this.gravValue = currentGrav;
		
		this.values[0] = this.airValue;
		this.values[1] = this.gravValue;
		
		Font font = new Font("Sanserif", Font.BOLD, 30);

		/* set the label for the gravity */
		currentGravLabel = new JTextField( Integer.toString( this.gravValue ), JTextField.CENTER );
		currentGravLabel.setFont( font );
		currentGravLabel.setEditable( false );
		currentGravLabel.setHorizontalAlignment(JTextField.CENTER);

		/* set the label for the air pressure */
		currentAirLabel = new JTextField( Integer.toString( this.airValue ), JTextField.CENTER );
		currentAirLabel.setEditable( false );
		currentAirLabel.setFont( font );
		currentAirLabel.setHorizontalAlignment(JTextField.CENTER);

		/* create a panel that would hold the sliders panels */
		JPanel settingsPanel = new JPanel();
		
		/* give it a box layout in aligning components vertically */
		settingsPanel.setLayout(new BoxLayout(settingsPanel, BoxLayout.Y_AXIS));
		
		/* make a panel for the air pressure, again aligning horizontally */
		JPanel airPanel = new JPanel();
		airPanel.setLayout(new BoxLayout(airPanel, BoxLayout.X_AXIS));
		airPanel.setAlignmentX( CENTER_ALIGNMENT );
		
		/* Create the air pressure slider setting initial values */
		this.airSlider = new JSlider(JSlider.HORIZONTAL, SET_MIN, SET_MAX, this.airValue);
		
		/* add the listener method
		 * Inspired by:
		 * Author: Patrick Chan, The Java Developers Almanac 1.4
		 * URL: http://www.exampledepot.com/egs/javax.swing/slider_SliderEvt.html?l=rel
		 */
		airSlider.addChangeListener(new ChangeListener() 
		{
	        /* when the user moves the slide we want to update the label and get values*/
	        public void stateChanged( ChangeEvent evt ) 
	        {
	            JSlider slider = (JSlider)evt.getSource();
	    
	            /* if the slider is moving */
	            if (slider.getValueIsAdjusting()) 
	            {
	            	/* update values and display position integer */
	            	airValue = slider.getValue();
	            	currentAirLabel.setText( Integer.toString( airValue ) );
	            }
	        }
	    });

		/* make a titled boder around the panel */
		airPanel.setBorder(BorderFactory.createTitledBorder("Air Pressure"));

		/* set the tick mark settings */
		airSlider.setMajorTickSpacing(25);
		airSlider.setMinorTickSpacing(5);
		airSlider.setPaintTicks(true);
		airSlider.setPaintLabels(true);
		
		/* add the slider to the panel */
		airPanel.add( airSlider );
		
		/* add a separator between panel and label */
		airPanel.add(new JSeparator(JSeparator.VERTICAL));
		
		/* add the label */
		airPanel.add( currentAirLabel );

		/* add the panel to the major panel */
		settingsPanel.add( airPanel );
		
		
		/* make a panel for the grav slider and the grav label */
		JPanel gravPanel = new JPanel( new BorderLayout() );
		
		/* add things horizontally */
		gravPanel.setLayout(new BoxLayout(gravPanel, BoxLayout.X_AXIS));
		gravPanel.setAlignmentX( CENTER_ALIGNMENT );
		
		/* create a titled border for the panel */
		gravPanel.setBorder(BorderFactory.createTitledBorder("Gravity"));
		
		/* make a slider for the gravity */
		this.gravSlider = new JSlider(JSlider.HORIZONTAL, SET_MIN, SET_MAX, this.gravValue);
		
		gravSlider.addChangeListener( new ChangeListener() 
		{
	        public void stateChanged( ChangeEvent evt) 
	        {
	            JSlider slider = (JSlider)evt.getSource();
	    
	            if (slider.getValueIsAdjusting()) 
	            {
	            	gravValue = slider.getValue();
	            	currentGravLabel.setText( Integer.toString( gravValue ) );
	            }
	        }
	    });

		
		
		gravSlider.setMajorTickSpacing(25);
		gravSlider.setMinorTickSpacing(5);
		gravSlider.setPaintTicks(true);
		gravSlider.setPaintLabels(true);
		
		gravPanel.add( gravSlider, BorderLayout.CENTER );
		gravPanel.add(new JSeparator(JSeparator.VERTICAL));
		gravPanel.add( currentGravLabel );
		settingsPanel.add( gravPanel );
		
		
		
		JPanel buttonPanel = new JPanel();
		buttonPanel.setLayout( new BoxLayout( buttonPanel, BoxLayout.X_AXIS));
		
		JButton setButton = new JButton ("Set");
		setButton.addActionListener( this );
		
		JButton canButton = new JButton ("Cancel");
		canButton.addActionListener( this );
		
		JButton resButton = new JButton ("Default");
		resButton.addActionListener( this );
		
		buttonPanel.add(setButton);
		buttonPanel.add(canButton);
		buttonPanel.add(resButton);
		
		settingsPanel.add( buttonPanel );
		
		
		this.add( settingsPanel );
		
		/* The following method centers the frame on the screen
		 * 
		 * Reference
		 * ---------------------------
		 * Author: Code Beach ( Name of actual person could not be located ) 
		 * URL: http://blog.codebeach.com/2008/02/center-dialog-box-frame-or-window-in.html
		 * 
		 */
		
		/* get the screen resolutions */
		Dimension screen = Toolkit.getDefaultToolkit().getScreenSize();


		/* convert the size to integers 
		 * subtract from the window size and div by 2
		 * to get the center position of the screen
		 */
		int x = ((int)screen.getWidth() - this.getWidth()) / 2;
		int y = ((int)screen.getHeight() - this.getHeight()) / 2; 

		this.setLocation(x, y);
		
		this.setVisible( true );
		
	
		return this.values;
	}
	

	
	public void actionPerformed( ActionEvent e )
	{
			String event = e.getActionCommand();

			if(event.equalsIgnoreCase("set"))
			{
				this.values[0] = this.airValue;
				this.values[1] = this.gravValue;
				this.dispose();
			}
			
			else if( event.equalsIgnoreCase( "cancel" ) )
			{
				this.dispose();
			}
			
			//else if( event.equalsIgnoreCase( "default" ) )
			//{
			//	//GameWindow.getInstance().getDefaultSettings()[0];
			//}
			
			else
			{
				/* need to store default values somewhere */
				
				int defaults[] = GameWindow.getInstance().getDefaultSettings();
				System.out.println("Air pressure is:" + defaults[0] + " Grav: " + defaults[1] );
				this.airValue = defaults[0];
				this.gravValue = defaults[1];
				this.airSlider.setValue( this.airValue );
				this.gravSlider.setValue( this.gravValue );
				currentGravLabel.setText( Integer.toString( this.gravValue ) );
				currentAirLabel.setText( Integer.toString( this.airValue ) );
				
			}
		}
	}
