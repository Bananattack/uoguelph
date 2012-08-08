package tim;

//	CIS*3750
//	UserScore.java

/*	
	This class kepts track of the users score for the tim game. It
	decrements the score by 1 by default every second. The decrement
	can be modified by calling the correct constructor. The largest
	value of decrement it will accept is 50. the smallest it will 
	accept is 1. If an invalid decrement value is entered it will be set
	to the default value (1).
	@author Trevor Clark
*/

public class UserScore
{
	/* CONSTANTS */
	public static final int DEFAULT_DECREMENT = 1; /* default value for decrement */

	private int initialScore;
	private int score;
	private int decrement;
	private boolean isDecrementing;
	
	/* CONSTRUCTORS */
	
	/* 
		constructor with just 1 parameter
		@param score the users initial score
	 */
	public UserScore(int score)
	{
		setInitialScore(score);
		setScore(score);
		setDecrement(DEFAULT_DECREMENT);
		setIsDecrementing(false);
	}
	
	/* 
		constructor with 2 parameters 
		@param score the users initial score
		@param decrement the inital decrement value
	*/
	public UserScore(int score, int decrement)
	{
		setInitialScore(score);
		setScore(score);
		setDecrement(decrement);
		setIsDecrementing(false);
	}
	
	/* GETTERS */
	
	/* getter for the initial score int */
	public int getInitialScore()
	{
		return(this.initialScore);
	}
	
	/* getter for the score int */
	public int getScore()
	{
		return(this.score);
	}
	
	/* getter for the decrement int */
	public int getDecrement()
	{
		return(this.decrement);
	}
	
	/* getter for the is decrementing boolean */
	public boolean getIsDecrementing()
	{
		return(this.isDecrementing);
	}
	
	/* SETTERS */
	
	/* 
		setter for the initial score int
		@param score the score
	 */
	public void setInitialScore(int initialScore)
	{
		this.initialScore = initialScore;
	}
	
	/*
		setter for the score int
		@param score the score
	 */
	public void setScore(int score)
	{
		this.score = score;
	}
	
	/* 
		setter for the decrement int.
		If decrement is < 1 or > 50 the decrement
		is set to 1.the decrement value
		@param decrement 
	*/
	public void setDecrement(int decrement)
	{
		if (decrement > 0 && decrement <= 50)
		{
			this.decrement = decrement;
		}
		else
		{
			this.decrement = DEFAULT_DECREMENT;
		}
	}
	
	/* 
		setter for the is decrementing boolean
		@param isDecrementing the isDecrementing (true or false)
	 */
	public void setIsDecrementing(boolean isDecrementing)
	{
		this.isDecrementing = isDecrementing;
	}
	
	/* Resets score to inital value.
	   Does nothing if isDecrementing is true	
	   Does not start the timer.
	*/
	public void resetScore()
	{
		if (getIsDecrementing() == false)
		{
			setScore(getInitialScore());
		}
	}
}