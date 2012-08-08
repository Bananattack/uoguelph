/*Mode changer class mk 1, for use in hazardous menu enviroments. Will allow the menu to when itsw elements "Puzzle" or "Free play" are triggered to setup and switch too puzzel or free play modes respectivly*/
package tim;

import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;

/*this action listener should be added to the menu button, which must be named Puzzle and Free Play*/
/*I made it an abstract class so it could easily be implemented to create greator functionality for an action listener*/
/*10/07 kevin: decided it is better not as an abstract class but instead as a stub*/
public class modeChangerClass implements ActionListener
{
	public void actionPerformed(ActionEvent e) 
	{
		/*here I am enforcing the API that the modes are rffered to as Puzzle and Free Play*/
		if(e.toString().equals("Puzzle"))
		{
			/*this is where the mode setup should be called*/
			 
		}
		if(e.toString().equals("Free Play"))
		{
			/*this is where the mode setup should be called*/ 
		}
	}
}/*end of file*/
