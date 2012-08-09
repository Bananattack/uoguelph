// CIS*2750
// Andrew G. Crowell
// 0545826
// acrowell@uoguelph.ca
import java.awt.*;
import javax.swing.*;

// Button with an icon, enough said.
public class IconButton extends JButton
{
	public IconButton(String filename)
	{
		super(new ImageIcon(filename));
		setHorizontalTextPosition(CENTER);
		setVerticalTextPosition(BOTTOM);
		setMargin(new Insets(0, 0, 0, 0));
	}

}
