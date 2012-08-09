import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

public class TestListener implements ActionListener
{
	exampleFieldEdit ex;
	public TestListener(exampleFieldEdit ex)
	{
		this.ex = ex;
	}

	public void actionPerformed(ActionEvent e)
	{
		try
		{
			ex.appendToStatusArea("A1 = " + ex.getDCA1() + "!");
		}
		catch(IllegalFieldValue err)
		{
			ex.appendToStatusArea("Illegal field value");
		}
	}
}
