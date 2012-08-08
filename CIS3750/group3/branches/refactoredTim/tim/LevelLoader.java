package tim;

import Widgets.*;
import java.util.*;
import org.w3c.dom.*;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;


public class LevelLoader
{
	public static Node getChildByTagName(String tagName, Node root)
	{
		Node child = root.getFirstChild();
		while(child != null)
		{
			if(child.getNodeName() == tagName)
			{
				return child;
			}
			else
			{
				child = child.getNextSibling();
			}
		}

		return child;
	}

	public static ArrayList<Node> getChildrenByTagName(String tagName, Node root)
	{
		ArrayList<Node> children = new ArrayList<Node>();
		Node child = root.getFirstChild();
		while(child != null)
		{
			if(child.getNodeName() == tagName)
			{
				children.add(child);
				child = child.getNextSibling();
			}
			else
			{
				child = child.getNextSibling();
			}
		}

		return children;
	}

	public static boolean load(String path)
	{
		Document dom;

		try
		{
			DocumentBuilderFactory domFactory = DocumentBuilderFactory.newInstance();
			DocumentBuilder domBuilder = domFactory.newDocumentBuilder();
			dom = domBuilder.parse(path);
		}
		catch(Exception e)
		{
			return false;
		}

		NodeList nl = dom.getElementsByTagName("level");
		Node root = nl.item(0);
		Node levelName = getChildByTagName("name", root);
		Node targetTime = getChildByTagName("targettime", root);
		Node gravity = getChildByTagName("gravity", root);
		Node airPressure = getChildByTagName("airPressure", root);
		Node hint = getChildByTagName("hint", root);

		Node goal = getChildByTagName("goal", root);
		ArrayList<LevelTask> tasks = new ArrayList<LevelTask>();

		ArrayList<Node> nodeList = getChildrenByTagName("task", goal);
		for(Node task : nodeList)
		{
			tasks.add(new LevelTask(task));			
		}

		Node object = getChildByTagName("object", root);
		nodeList = getChildrenByTagName("widget", object);

		ArrayList<Widget> widgets = new ArrayList<Widget>();
		for(Node widget : nodeList)
		{
			String widgetName = getChildByTagName("type", widget).getFirstChild().getNodeValue();
			int posX = Integer.parseInt(getChildByTagName("posX", widget).getFirstChild().getNodeValue());
			int posY = Integer.parseInt(getChildByTagName("posY", widget).getFirstChild().getNodeValue());
			String direction = getChildByTagName("direction", widget).getFirstChild().getNodeValue();
			String isLocked = getChildByTagName("isLocked", widget).getFirstChild().getNodeValue();
			

			System.out.println("Widget: "+widgetName+", "+posX+", "+posY+", "+direction+", "+isLocked);
			
			Object o;
			try
			{
				Class c = Class.forName("Widgets." + widgetName);
				o = c.getConstructor(new Class[] {}).newInstance(new Object[] {});
			}
			catch(Exception e)
			{
				return false;
			}
			
			Widget w = (Widget)o;
			w.setPositionX(posX);
			w.setPositionY(posY);
			widgets.add(w);
		}

		//If you've made it this far without errors, you can update the world
		//GameWindow.getInstance().clear(true);

		for(Widget w : widgets)
		{
		//	GameWindow.getInstance().addWidget(new GamePiece(w));			
		}

		return true;
	}
}