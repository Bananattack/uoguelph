import java.util.*;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

public class SlotMachineGUI extends JFrame
{
	public static void main(String[] args)
	{
		new SlotMachineGUI();
	}

	private SlotMachine slotMachine;
	private JPanel panel;
	private JButton pullSlotButton;
	private ArrayList<JLabel> slotLabels;
	private ImageIcon bellIcon, cherryIcon, grapeIcon;
	public SlotMachineGUI()
	{
		super("Slot Machine!");

		bellIcon = new ImageIcon("bell.gif", "A bell!");
		cherryIcon = new ImageIcon("cherry.gif", "A cherry!");
		grapeIcon = new ImageIcon("grape.gif", "A grape!");
		slotMachine = new SlotMachine();

		setLayout(new BorderLayout());
		panel = new JPanel();
		panel.setBackground(Color.white);
		panel.setLayout(new BorderLayout());
		add(panel);

		panel.add(new JLabel("Slot Machine Game"), BorderLayout.NORTH);
		slotLabels = new ArrayList<JLabel>();
		for(int i = 0; i < 3; i++)
		{
			slotLabels.add(new JLabel(""));
		}
		panel.add(slotLabels.get(0), BorderLayout.WEST);
		panel.add(slotLabels.get(1), BorderLayout.CENTER);
		slotLabels.get(1).setHorizontalAlignment(JLabel.CENTER);
		panel.add(slotLabels.get(2), BorderLayout.EAST);

		pullSlotButton = new JButton("Pull Slot");
		pullSlotButton.addActionListener(new ActionListener()
							{
								public void actionPerformed(ActionEvent e)
								{
									randomize();
								}
							});
		panel.add(pullSlotButton, BorderLayout.SOUTH);

		randomize();
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		setSize(640, 480);
		setVisible(true); 
	}

	public void randomize()
	{
		slotMachine.randomizeSlots();
		for(int i = 0; i < 3; i++)
		{
			JLabel label = slotLabels.get(i);
			SlotMachine.SlotPiece slot = slotMachine.getSlot(i);
			if(slot == SlotMachine.SlotPiece.BELL)
			{
				label.setIcon(bellIcon);				
			}
			else if(slot == SlotMachine.SlotPiece.CHERRY)
			{
				label.setIcon(cherryIcon);
			}
			else if(slot == SlotMachine.SlotPiece.GRAPE)
			{
				label.setIcon(grapeIcon);
			}
			label.setText(slot.name());
		}
	}
}
