package tim;


public interface modes
{
	//this method should be called to setup all instance vars and prepair the mode to be played
	//it should return either 0 or # to indacate pass or fail, and depending on that number, the reason for that fail
	public int initMode();

}
