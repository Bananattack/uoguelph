package tim;

/* Title: ScoreIO.java
   Author: Jamie McDade
   ID:     0272527
   Course: CIS3750
   E-Mail: jmcdade@uoguelph.ca
   Date Modified: 11-21-08
   Description: This class handles the score information for each level in puzzle mode.  Based on the name of
                the level file, it will strip the .xml extension and append a .score extension.  These files
				contain the initial score, and decrement value for each level (which can be changed in the
				"Set Score" menu in free-play mode).
*/
import java.io.*;
import java.util.*;

public class ScoreIO 
{
	private String fileName; /*the level's file name, which will be used for naming the .score file*/
	private File scoreFile; /*the score file*/
	private int levelScore; /*the level's initial score and decrement*/
	private int levelDecrement;
	
	public ScoreIO(int levelScore1, int levelDecrement1)
	{
		levelScore = levelScore1;
		levelDecrement = levelDecrement1;
	}
	
	/**
	 * Create the score file
	 * @param file The level file needed to make the score file
	 *
	 */
	public void createScoreFile(File file)
	{
		this.fileName = file.getName();
		this.fileName = fileName.substring(0, fileName.lastIndexOf(".")); /*Get the level name and strip the .xml*/
		scoreFile = new File(file.getParent()+"/"+fileName+".score"); /*Attempt to make the .score file*/
		try
		{
			if(scoreFile.createNewFile() == false) /*File already exists*/
			{
				if(scoreFile.delete() == false) /*Attempt to delete the file*/
				{
					System.out.println("Unable to delete the previous score file.");
				}
				if(scoreFile.createNewFile() == false)
				{
					System.out.println("Unable to create a score file for this level!");
					return;
				}
			}
		}
		catch(IOException e1) /*Unable to access the file for some reason*/
		{
			System.out.println("Unable to access the score file.");
			System.out.println(e1.getMessage());
		}
		PrintWriter output = null; /*PrintWriter for the ability to println to the file*/
		try
		{
			output = new PrintWriter(new BufferedWriter(new FileWriter(scoreFile)));
		}
		catch(IOException e2)
		{
			System.out.println("Unable to open score file for writing!");
		}
		output.println(levelScore); /*input the level score and decrement*/
		output.println(levelDecrement);
		output.close();
	}
	
	/**
	 * Read from a .score file
	 *
	 * @param file The associated level file used to find the proper score file.
	 */
	public void readScoreFile(File file)
	{
		String text;
		int tempScore = levelScore; /*temporary score and decrement in case of invalid .score file format*/
		int tempDecrement = levelDecrement;
		this.fileName = file.getName();
		this.fileName = fileName.substring(0, fileName.lastIndexOf(".")); /*strip .xml extension*/
		scoreFile = new File(file.getParent()+"/"+fileName+".score");
		Scanner input = null; /*Scanner used to do nextInt() method to capture the score and decrement values*/
		if(scoreFile.exists()) 
		{
			try
			{
				input = new Scanner(new BufferedReader(new FileReader(scoreFile)));
			}
			catch(IOException e) /*Catch some sort of IO error with the score file*/
			{
				System.out.println("Unable to open score file for reading!");
			}
			try
			{
				levelScore = input.nextInt(); /*proper format should have the initial score on the first line, 
				                               followed by the decrement on the second line*/
				levelDecrement = input.nextInt();
			}
			catch(Exception e) /*Improper .score file format*/
			{
				System.out.println("Invalid score file!");
				levelScore = tempScore; /*restore the previous score and decrement values*/
				levelDecrement = tempDecrement;
			}
		}
	}
	
	/**
	 * Get the initial score for the level
	 * @return levelScore The initial score for the level
	 */
	public int getLevelScore()
	{
		return levelScore;
	}
	
	/**
	 * Set the initial score for the level
	 * @param levelScore1 The initial score to set for the level
	 */
	public void setLevelScore(int levelScore1)
	{
		levelScore = levelScore1;
	}
	
	/**
	 * Get the score decrement for the level
	 * @return levelDecrement The score decrement for the level
	 */
	public int getLevelDecrement()
	{
		return levelDecrement;
	}
	
	/**
	 * Set the score decrement for the level
	 * @param levelDecrement1 The score decrement to set for the level
	 */
	public void setLevelDecrement(int levelDecrement1)
	{
		levelDecrement = levelDecrement1;
	}
}

