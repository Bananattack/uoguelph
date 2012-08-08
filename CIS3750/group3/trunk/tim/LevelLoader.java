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

	public static boolean load(GameController controller, String path)
	{
		Document dom;
		ArrayList<GamePiece> widgets = new ArrayList<GamePiece>();
		ArrayList<LevelTask> tasks = new ArrayList<LevelTask>();

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
		GameWindow.getInstance().setLevelName(levelName.getFirstChild().getNodeValue());
		Node targetTime = getChildByTagName("targettime", root);
		Node gravity = getChildByTagName("gravity", root);
		Node airPressure = getChildByTagName("airPressure", root);
		Node hint = getChildByTagName("hint", root);

		Node goal = getChildByTagName("goal", root);


		ArrayList<Node> nodeList = getChildrenByTagName("task", goal);
		for(Node task : nodeList)
		{
			LevelTask t = new LevelTask(task);
			tasks.add(t);
		}

		Node object = getChildByTagName("object", root);
		nodeList = getChildrenByTagName("widget", object);

		for(Node widget : nodeList)
		{
			String widgetName = getChildByTagName("type", widget).getFirstChild().getNodeValue();
			int id = Integer.parseInt(getChildByTagName("id", widget).getFirstChild().getNodeValue());
			int posX = (int)Float.parseFloat(getChildByTagName("posX", widget).getFirstChild().getNodeValue());
			int posY = (int)Float.parseFloat(getChildByTagName("posY", widget).getFirstChild().getNodeValue());
			String direction = getChildByTagName("direction", widget).getFirstChild().getNodeValue();
			String isLocked = getChildByTagName("isLocked", widget).getFirstChild().getNodeValue();


			System.out.println("Widget: "+widgetName+", "+posX+", "+posY+", "+direction+", "+isLocked);

			Object o;
			try
			{
				Class c = WidgetLoader.loadWidget("Widgets." + widgetName);
				o = c.getConstructor(new Class[] {}).newInstance(new Object[] {});

				Widget w = (Widget)o;
				w.setPositionX(posX);
				w.setPositionY(posY);
				w.setLock(Boolean.valueOf(isLocked));
				GamePiece gp = new GamePiece(w);
				gp.setWidgetID(id);
				widgets.add(gp);
			}
			catch (Exception e)
			{
				System.out.println("Can't find widget " + widgetName);
			}
		}

		//If you've made it this far without errors, you can update the world
		controller.stopGame();
		controller.clearGame(true);
		controller.setNewPiece(false);
		controller.setMovingOldPiece(false);
		controller.setSelectedPiece(null);
		// In puzzle mode, clear the widget panel entirely
		if(!GameWindow.getInstance().getGameType())
		{
			GameWindow.getInstance().clearWidgetPanel();
		}

		for(LevelTask t : tasks)
		{
			GamePiece gp = new GamePiece(t.taskToWidget());
			t.setPiece(gp);
			controller.addWidget(gp);
			controller.addTask(t);
		}

		// Andy(2008-11-20):
		// Below is the rather ugly code for counting all widget types, and only having locked ones on-map
		HashMap<Class, Integer> widgetCount = new HashMap();
		HashMap<Class, ActiveButton> buttonMap = new HashMap();

		// Iterate all widgets in the level...
		for(GamePiece gp : widgets)
		{
			// For locked ones, (or if free play) place on map.
			if(gp.getWidget().isLocked() || GameWindow.getInstance().getGameType())
			{
				controller.addWidget(gp);
			}
			// For unlocked ones (puzzle mode), we make the user place them
			// (they're what makes puzzle mode a PUZZLE)
			else
			{
				Class c = gp.getWidget().getClass();
				// Keep count of unique widget types
				if(widgetCount.containsKey(c))
				{
					widgetCount.put(c, widgetCount.get(c) + 1);
				}
				else
				{
					buttonMap.put(c, GameWindow.getInstance().createActiveButton(gp));
					widgetCount.put(c, 1);
				}
			}
		}

		// Iterate over all unique widget types and update their associated count in the widget panel
		for(Map.Entry<Class, Integer> pair : widgetCount.entrySet())
		{
			buttonMap.get(pair.getKey()).setTotalAmount(pair.getValue());
			buttonMap.get(pair.getKey()).resetAmount();
		}

		return true;
	}
}
