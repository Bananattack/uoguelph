package tim;

import Widgets.*;
import java.io.File;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;
import java.util.ArrayList;

import org.w3c.dom.*;

/**
 * @author Darren O'Shea
 *
 */
public class LevelSaver 
{
	public static boolean save(String path)
	{
		Document dom;

		try
		{
			DocumentBuilderFactory domFactory = DocumentBuilderFactory.newInstance();
			DocumentBuilder domBuilder = domFactory.newDocumentBuilder();
			dom = domBuilder.newDocument();
		}
		catch(Exception e)
		{
			return false;
		}

		Element level = dom.createElement("level");
		dom.appendChild(level);

		Element name = dom.createElement("name");
		name.appendChild(dom.createTextNode(GameWindow.getInstance().getLevelName()));

		Element targettime = dom.createElement("targettime");
		targettime.appendChild(dom.createTextNode(Integer.toString(GameWindow.getInstance().getTargetTime())));

		Element gravity = dom.createElement("gravity");
		gravity.appendChild(dom.createTextNode(Integer.toString(GameWindow.getInstance().getGravity())));

		Element airPressure = dom.createElement("airPressure");
		airPressure.appendChild(dom.createTextNode(Integer.toString(GameWindow.getInstance().getAirPressure())));

		Element hint = dom.createElement("hint");
		hint.appendChild(dom.createTextNode(GameWindow.getInstance().getHint()));

		level.appendChild(name);
		level.appendChild(targettime);
		level.appendChild(gravity);
		level.appendChild(airPressure);
		level.appendChild(hint);

		Element goal = dom.createElement("goal");
		ArrayList<LevelTask> tasks = GameWindow.getInstance().getTasks();
		for(LevelTask t : tasks)
		{
			Element task = dom.createElement("task");
			Element widgetID = dom.createElement("type");
			widgetID.appendChild(dom.createTextNode(t.getWidgetType().toString()));
			Element action = dom.createElement("action");
			action.appendChild(dom.createTextNode("destroy"));
			Element posX = dom.createElement("posX");
			posX.appendChild(dom.createTextNode(Integer.toString(t.getPosX())));
			Element posY = dom.createElement("posY");
			posY.appendChild(dom.createTextNode(Integer.toString(t.getPosY())));

			task.appendChild(widgetID);
			task.appendChild(action);
			task.appendChild(posX);
			task.appendChild(posY);

			goal.appendChild(task);
		}

		level.appendChild(goal);

		Element object = dom.createElement("object");
	
		ArrayList<GamePiece> pieces = GameWindow.getInstance().getGamePieces();
		for(GamePiece gp : pieces)
		{
			Widget w = gp.getWidget();

			if((w instanceof ObjectiveWidget) == false)
			{
				Element widget = dom.createElement("widget");
		
				Element id = dom.createElement("id");
				id.appendChild(dom.createTextNode(Integer.toString(gp.getWidgetID())));

				Element type = dom.createElement("type");
				String className = w.getClass().getName();
				className = className.substring(className.lastIndexOf('.')+1);
				type.appendChild(dom.createTextNode(className));

				Element posX = dom.createElement("posX");
				posX.appendChild(dom.createTextNode(Integer.toString((int)w.getPositionX())));
				Element posY = dom.createElement("posY");
				posY.appendChild(dom.createTextNode(Integer.toString((int)w.getPositionY())));

				Element direction = dom.createElement("direction");
				switch(w.getDirection())
				{
					case EAST:
						direction.appendChild(dom.createTextNode("EAST"));
						break;
					case WEST:
						direction.appendChild(dom.createTextNode("WEST"));
						break;
					case NORTH:
						direction.appendChild(dom.createTextNode("NORTH"));
						break;
					case SOUTH:
						direction.appendChild(dom.createTextNode("SOUTH"));
						break;
				}			
				
				Element isLocked = dom.createElement("isLocked");
				isLocked.appendChild(dom.createTextNode(Boolean.toString(w.isLocked())));

				widget.appendChild(id);
				widget.appendChild(type);
				widget.appendChild(posX);
				widget.appendChild(posY);
				widget.appendChild(direction);
				widget.appendChild(isLocked);

				object.appendChild(widget);
			}
		}

		level.appendChild(object);

		
		StreamResult res = new StreamResult(new File(path));
		
		TransformerFactory autobots = TransformerFactory.newInstance();
		Transformer optimus;
		try
		{
			optimus = autobots.newTransformer();
			optimus.transform(new DOMSource(dom), res);
		}
		catch (Exception e)
		{
			e.printStackTrace();
		}
		
		System.out.println(res);
		
		return true;
	}
}
