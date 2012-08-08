package rogueModel;


/**
 * A container that holds certain information in regards to key events.
 * Used to track which buttons have been pressed lately to prevent crazy key-repeat lag.
 */
public class KeypressInfo
{
	private int scancode;
	private int iterationsHeld;
	
	/**
	 * Creates a key with a specified scancode.
	 */
	public KeypressInfo(int scancode)
	{
		this.scancode = scancode;
		this.iterationsHeld = 0;
	}
	
	/**
	 * Gets the scancode.
	 * @return scancode
	 */
	public int getScancode()
	{
		return scancode;
	}
	
	/**
	 * Gets the iterations. That is, the number of "ticks" that this button has been inspected.
	 * @return iterations
	 */
	public int getIterationsHeld()
	{
		return iterationsHeld;
	}
	
	/**
	 * Increases the button's iterations held by one.
	 */
	public void increaseIterationsHeld()
	{
		iterationsHeld++;
	}
	
	/**
	 * A useful string representation
	 * @return a formatted string of this object.
	 */
	public String toString()
	{
		return "KeypressInfo (scancode: " + scancode + ", iterations held: " + iterationsHeld + " )";
	}
	
	/**
	 * Two KeypressInfo objects are equal if they refer to the same scancode.
	 * 
	 * @return ki instanceof KeypressInfo
			&& this.scancode == ((KeypressInfo) ki).scancode
	 */
	public boolean equals(Object ki)
	{
		return ki instanceof KeypressInfo
			&& this.scancode == ((KeypressInfo) ki).scancode;
	}
}