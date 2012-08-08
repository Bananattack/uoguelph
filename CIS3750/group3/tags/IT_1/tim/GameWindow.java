// CIS*3750
// GameWindow.java
// Andy (2008-10-06):
// 	Renamed from WidgetPanelTest.java to GameWindow.
//	GameWindow is a Singleton, because a lot of outside classes will
//	rely on pieces of global information which is less work and less memory overhead
// 	than holding onto the pointer in every object that relies on it.
//	This will hopefully become the main base for running the program.

// Andy (2008-10-01): Refactored class structure to be in package.
package tim;


import java.awt.*;
import javax.swing.*;
import javax.swing.border.*;

public class GameWindow extends JFrame
{
	private static GameWindow instance = null;
	public static GameWindow getInstance()
	{
		if(instance == null)
		{
			instance = new GameWindow();
		}
		return instance;
	}

	public static void main(String[] args) 
	{
		GameWindow window = GameWindow.getInstance();
	}


	// TODO: Replace with active widget selection when Widget API is integrated.
	private int activePlaceholderIndex = -1;

	private GameWindow()
	{
		super();
		setLayout(new BorderLayout());

		/* Phil (2008-10-03): Testing out the menu class */
		GameMenuBar menuBar = new GameMenuBar();
		menuBar.setAll();

		WidgetPanel widgetPanel = new WidgetPanel();
		for(int i = 0; i < 32; i++)
		{			
			widgetPanel.add(new WidgetPanelButton(i + 1));
		}

		PlayfieldPanel playPanel = new PlayfieldPanel();

		JPanel extraPanel = new JPanel();		
		extraPanel.add(new JButton("For scale"));
		extraPanel.setBorder(new EtchedBorder());

		add(widgetPanel, BorderLayout.EAST);	
		add(playPanel, BorderLayout.CENTER);
		add(extraPanel, BorderLayout.WEST);

		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		setBounds(0, 0, 800, 600);
		setJMenuBar(menuBar);
		setVisible(true);
	}

	public int getActivePlaceholderIndex()
	{
		return activePlaceholderIndex;
	}

	public void setActivePlaceholderIndex(int activePlaceholderIndex)
	{
		this.activePlaceholderIndex = activePlaceholderIndex;
	}
}
