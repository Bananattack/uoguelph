package tim;

/*	CIS*3750
	UserScore.java
	by Trevor Clark
	
	This class kepts track of the users score for the tim game. It
	decrements the score by 1 by default every second. The decrement
	can be modified by calling the correct constructor. The largest
	value of decrement it will accept is 50. the smallest it will 
	accept is 1. If an invalid decrement value is entered it will be set
	to the default value (1).
*/

import javax.swing.Timer;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

public class UserScore
{
	/* CONSTANTS */
	public static final int DEFAULT_DECREMENT = 1; /* default value for decrement */
	public static final int DELAY = 1000;		   /* 1000 milliseconds = 1 sec */

	private int initialScore;
	private int score;
	private int decrement;
	private boolean isDecrementing;
	private Timer timer;
	
	/* CONSTRUCTORS */
	
	/* constructor with just 1 parameter (score) */
	public UserScore(int score)
	{
		setInitialScore(score);
		setScore(score);
		setDecrement(DEFAULT_DECREMENT);
		setIsDecrementing(false);
		initializeTimer();
	}
	
	/* constructor with 2 parameters (score and decrement) */
	public UserScore(int score, int decrement)
	{
		setInitialScore(score);
		setScore(score);
		setDecrement(decrement);
		setIsDecrementing(false);
		initializeTimer();
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
	
	/* setter for the initial score int */
	public void setInitialScore(int initialScore)
	{
		this.initialScore = initialScore;
	}
	
	/* setter for the score int */
	public void setScore(int score)
	{
		this.score = score;
	}
	
	/* setter for the decrement int.
	   If decrement is < 1 or > 50 the decrement
	   is set to 1.
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
	
	/* setter for the is decrementing boolean */
	public void setIsDecrementing(boolean isDecrementing)
	{
		this.isDecrementing = isDecrementing;
	}
	
	/* initialize the timer */
	/* "How to Use Swing Timers"
	 *
	 * The following method initializes the timer
	 *
	 * Reference
	 * ---------------------------
	 * Author: Sun Microsystems, Inc.
	 * URL: http://java.sun.com/docs/books/tutorial/uiswing/misc/timer.html
	 *
	*/
	public void initializeTimer()
	{
		this.timer = new Timer(DELAY, new timerListener());
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
	
	/* if isDecrementing is false this method will start 
	   the timer and set isDecrementing to true
	*/
	public void startDecrementing()
	{
		if (getIsDecrementing() == false)
		{
			setIsDecrementing(true);
			this.timer.start();
		}
	}
	
	/* if isDecrementing is true this method will stop
	   the timer and set isDecrementing to false
	*/
	public void stopDecrementing()
	{
		if (getIsDecrementing() == true)
		{
			setIsDecrementing(false);
			this.timer.stop();
		}
	}
	
	/* action listener for the timer */
	private class timerListener implements ActionListener
    {
		public void actionPerformed(ActionEvent e) 
		{
			setScore(getScore() - getDecrement());
			System.out.println(getScore());
		}
            
	}
}