package tim;

import org.w3c.dom.*;

public class LevelTask
{
	public String action;
	public int widgetID;

	public LevelTask(Node child)
	{
		widgetID = Integer.parseInt(LevelLoader.getChildByTagName("widgetID", child).getFirstChild().getNodeValue());
		action = LevelLoader.getChildByTagName("action", child).getFirstChild().getNodeValue();
	}
}