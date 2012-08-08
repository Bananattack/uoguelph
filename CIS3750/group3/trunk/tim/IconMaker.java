/*
 *  IconMaker.java
 * 
 *  Created by Filip Anguelov on 03/10/08.
 *  ID: 0223807
 *  E-mail: fanguelo@uoguelph.ca
 *  
 *  Simple class to make an Icon for various things
 *  
 *  Some parts are taken from respective tutorials online. See below.
 *  
 */
package tim;

import java.io.File;
import javax.swing.ImageIcon;

/**
 * 
 * @author Filip Anguelov
 *
 */
public class IconMaker 
{

	public IconMaker()
	{
		
	}
	
	/**
	 * This function takes in the path of an image (example: "c:\blah\hot.png") 
	 * and makes an icon that can be placed into menu items or anywhere on screen
	 * 
	 * Reference
	 * --------------------------
	 * Author: Sun Microsystems, Inc 
	 * URL: http://java.sun.com/docs/books/tutorial/uiswing/components/icon.html
	 * 
	 * Additionally
	 * URL: http://java.sun.com/j2se/1.5.0/docs/api/javax/swing/ImageIcon.html
	 * URL: http://java.sun.com/j2se/1.4.2/docs/api/java/net/URL.html
	 * 
	 * @param filePath The file path of the image to be made into an icon
	 * @return Returns a new ImageIcon
	 */
	public ImageIcon makeIcon( String filePath )
	{
		/* make a file object to check for a valid path */
		File image =  new File( filePath );
		
		/* if the filename is valid make it */
		if ( image.exists() )
		{
			return new ImageIcon( filePath );
		}
		else
		{
			return null;
		}
	}

	
}
