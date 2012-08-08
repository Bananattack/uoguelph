// CIS*3750
// ScorePanel.java

package tim;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

/*
	ScorePanel is the panel where the user score will
	appear during game.
	@author Trevor Clark
 */

public class ScorePanel
{
	/* CONSTANTS */
	public static final int DELAY = 1000;		   /* 1000 milliseconds = 1 sec */
	
	private UserScore score;
	private Timer timer;
	
	private JPanel myPanel;
	private JLabel scoreLabel;
	private JTextArea scoreText;
	
	/* CONSTRUCTOR */
	
	/* 
		constructor with just 1 parameter
		@param score the users initial score
	 */
	public ScorePanel(int score)
	{
		this.score = new UserScore(score);
		
		this.myPanel = createPanel();
		this.scoreLabel = createLabel();
		this.scoreText = createTextArea();
		this.scoreText.setEditable(false);
		this.myPanel.add(scoreLabel, BorderLayout.CENTER);
		this.myPanel.add(scoreText, BorderLayout.EAST);
		
		initializeTimer();
	}
	
	/* 
		constructor with 2 parameters 
		@param score the users initial score
		@param decrement the inital decrement value
	*/
	public ScorePanel(int score, int decrement)
	{
		this.score = new UserScore(score, decrement);
		
		this.myPanel = createPanel();
		this.scoreLabel = createLabel();
		this.scoreText = createTextArea();
		this.scoreText.setEditable(false);
		this.myPanel.add(scoreLabel, BorderLayout.CENTER);
		this.myPanel.add(scoreText, BorderLayout.EAST);
		
		initializeTimer();
	}
	
	/* GETTERS */
	
	/* wrapper getter method for UserScore.getInitialScore() */
	public int getInitialScore()
	{
		return(this.score.getInitialScore());
	}
	
	/* wrapper getter method for UserScore.getScore() */
	public int getScore()
	{
		return(this.score.getScore());
	}
	
	/* wrapper getter method for UserScore.getDecrement() */
	public int getDecrement()
	{
		return(this.score.getDecrement());
	}
	
	/* wrapper getter method for UserScore.getIsDecrementing() */
	public boolean getIsDecrementing()
	{
		return(this.score.getIsDecrementing());
	}
	
	/*SETTERS*/
	
	public void setInitialScore(int score)
	{
		this.score.setInitialScore(score);
	}
	
	public void setDecrement(int decrement)
	{
		this.score.setDecrement(decrement);
	}
	
	public JPanel getPanel()
	{
		return(this.myPanel);
	}
	
	
	/* wrapper method for UserScore.resetScore() */
	public void resetScore()
	{
		this.score.resetScore();
	}
	
	/* creates the panel for the ScorePanel */
	public JPanel createPanel()
	{
		return (new JPanel(new BorderLayout()));
	}
	
	/* creates the label that says SCORE" */
	public JLabel createLabel()
	{
		return (new JLabel("SCORE:", SwingConstants.RIGHT));
	}
	
	/* creates the text are that displays the score */
	public JTextArea createTextArea()
	{
		return (new JTextArea("" + getScore()));
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
	
	/* if isDecrementing is false this method will start 
	   the timer and set isDecrementing to true
	*/
	public void startDecrementing()
	{
		if (getIsDecrementing() == false)
		{
			score.setIsDecrementing(true);
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
			score.setIsDecrementing(false);
			this.timer.stop();
		}
	}
	
	/* action listener for the timer */
	private class timerListener implements ActionListener
    {
		public void actionPerformed(ActionEvent e) 
		{
			score.setScore(getScore() - getDecrement());
			scoreText.setText("" + getScore());
		}
	}

}
