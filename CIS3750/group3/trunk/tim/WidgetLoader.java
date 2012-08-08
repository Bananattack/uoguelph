package tim;

import Widgets.*;
import java.io.*;
import java.net.*;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;
import java.util.ArrayList;
import java.util.Enumeration;
import java.util.jar.JarEntry;
import java.util.jar.JarFile;

import org.w3c.dom.*;

/**
 * @author Darren O'Shea
 *
 */
public class WidgetLoader
{
	public static Class loadWidget(String fullWidgetName)
	{
		try
		{
			Class c = Class.forName(fullWidgetName);

			return c;
		}
		catch(Exception e)
		{
			System.out.println(e);
		}
		
		System.out.println("returning null");
		return null;
	}

	private static boolean widgetInList(ArrayList<Widget> widgetList, Widget w)
	{
		for(Widget myW : widgetList)
		{
			if(myW.getClass() == w.getClass())
			{
				return true;
			}
		}

		return false;
	}

	public static ArrayList<Widget> getAllWidgets()
	{
		ArrayList<Widget> widgetList = new ArrayList<Widget>();
		String[] filenames;
		
		filenames = new File("Widgets/").list(new FilenameFilter() {
			public boolean accept(File dir, String name)
			{
				return name.endsWith(".class");
			}
		});
		
		for(String filename : filenames)
		{
			try
			{
				Class c = Class.forName("Widgets." + FileIO.stripExtension(filename));
				Object o = c.getConstructor(new Class[] {}).newInstance(new Object[] {});
				if(o instanceof Widget && o.getClass() != Widget.class)
				{
					widgetList.add((Widget)o);
				}
			}
			catch(Exception e)
			{
				System.out.println(e);
			}
		}

		filenames = new File("./").list(new FilenameFilter() {
		public boolean accept(File dir, String name)
		{
				return name.endsWith(".jar");
		}
		});
		for(String filename : filenames)
		{
				try
				{
						JarFile j = new JarFile(filename);                              
						JarEntry wEntry = (JarEntry) j.getJarEntry("Widgets/");
						if(wEntry != null)
						{
								Enumeration jEnum = j.entries();
								while (jEnum.hasMoreElements())
								{
										JarEntry entry = (JarEntry) jEnum.nextElement();
										if(entry.getName().endsWith(".class"))
										{
												String widgetName = FileIO.stripExtension(entry.getName());
												widgetName = widgetName.replace('/','.');
												Class c = loadWidget(widgetName);
												try
												{
														Object o = c.getConstructor(new Class[] {}).newInstance(new Object[] {});
														if(o instanceof Widget && o.getClass() != Widget.class)
														{
																if( widgetInList(widgetList, (Widget)o) == false)
																{
																		widgetList.add((Widget)o);
																}
														}
												}
												catch (Exception e3)
												{
														System.out.println(e3);
												}                                                       
										}
								}
						}
				}
				catch(Exception e2)
				{
						System.out.println(e2);
				}
		}


		return widgetList;
	}
}
