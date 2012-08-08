package tim;

import org.w3c.dom.*;
import Widgets.*;

public class LevelTask
{
	private String action;
	private Class widgetType;
	private int posX, posY;
	private boolean bComplete=false;
	private GamePiece piece;
	
	public LevelTask(Node child)
	{
		try
		{
			widgetType = Class.forName("Widgets." + LevelLoader.getChildByTagName("type", child).getFirstChild().getNodeValue());
		}
		catch (Exception e)
		{
			System.out.println(e);
		}
		
		action = LevelLoader.getChildByTagName("action", child).getFirstChild().getNodeValue();
		posX = (int)Float.parseFloat(LevelLoader.getChildByTagName("posX", child).getFirstChild().getNodeValue());
		posY = (int)Float.parseFloat(LevelLoader.getChildByTagName("posY", child).getFirstChild().getNodeValue());
	}

	public Class getWidgetType()
	{
		return widgetType;
	}

	public int getPosX()
	{
		return posX;
	}

	public int getPosY()
	{
		return posY;
	}

	public Widget taskToWidget()
	{
		Widget w = new ObjectiveWidget();
		w.setPositionX(posX);
		w.setPositionY(posY);
		w.setLock(true);
		return w;
	}

	public void setPiece(GamePiece inPiece)
	{
		piece = inPiece;
	}
	
	public void hit(GamePiece objective, GamePiece instigator)
	{
		if((objective == piece) && (instigator.getWidget().getClass() == widgetType))
		{
			bComplete=true;
		}
	}
	
	public boolean isComplete()
	{
		return bComplete;
	}
}