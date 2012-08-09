import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

public class TestListener implements ActionListener
{
	public void actionPerformed(ActionEvent e)
	{
		JComponent c = (JComponent) e.getSource();
		example ex = (example) c.getTopLevelAncestor();
		ex.appendToStatusArea("Tacos '" + ex.getname() + "'");
	}
}
