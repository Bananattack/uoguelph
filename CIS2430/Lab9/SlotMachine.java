import java.util.*;

public class SlotMachine 
{
	private static final int defaultSlotCount = 3;
	public enum SlotPiece { BELL, CHERRY, GRAPE };

	private ArrayList<SlotPiece> slots;
	public SlotMachine()
	{
		slots = new ArrayList<SlotPiece>();
		for(int i = 0; i < defaultSlotCount; i++)
		{
			slots.add(null);
		}
		randomizeSlots();
	}

	public SlotPiece getSlot(int i)
	{
		return slots.get(i);
	}
	
	public void randomizeSlots()
	{
		for(int i = 0; i < 3; i++)
		{
			int r = (int) (Math.random() * 3);
			SlotPiece piece = null;
			switch(r)
			{
				case 0: piece = SlotPiece.BELL; break;
				case 1: piece = SlotPiece.CHERRY; break;
				case 2: piece = SlotPiece.GRAPE; break;
			}
			slots.set(i, piece);
		}
	}
}
