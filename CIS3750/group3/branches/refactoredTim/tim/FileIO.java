/*
 *  FileIO.java
 *  
 *  Created by Filip Anguelov on 29/01/08.
 *  ID: 0223807
 *  E-mail: fanguelo@uoguelph.ca
 *  
 *  Simple class to handle file input/output
 *  This is a work in progress.
 *  
 */
package tim;

import java.io.*;
import java.util.Scanner;

import javax.swing.JFileChooser;
import javax.swing.JOptionPane;

public class FileIO
{
	private File currentSaveFile;
	private File currentGameDir;
	private final JFileChooser myFileChooser = new JFileChooser();

	public FileIO(File current)
	{
		this.currentGameDir = current;
		this.myFileChooser.setCurrentDirectory(current);
		this.myFileChooser.addChoosableFileFilter(new configFilter());
	}

	/**
	 *
	 * Get rid of the extension of a filename, in some case
	 * where it is more or less unnecessary.
	 *
	 * @param filename - The string to read.
	 * @return filename, sans extension.
	 */
	public static String stripExtension(String filename)
	{
		String strippedName = "";

		int pos = filename.lastIndexOf (".");

		if (pos >= 0)
		{
			strippedName = filename.substring(0, pos);
		}
		return strippedName;
	}
	
	/* save the current working directory */
	public void setcurrentGameDir(File current)
	{
		this.currentGameDir = current;
	}

	public File getcurrentGameDir ()
	{
		return this.currentGameDir;
	}

	
	public String getGameDir()
	{
		return new String( currentGameDir.toString() + "/" );
	}
	
	/* return the name of the file which would be our save file*/
	public String getConfigFileName()
	{
		if ( this.currentSaveFile != null)
			return (new String( this.currentSaveFile.getName()));
		else
			return null;
	}

	/* from the current project path get the directory only without the name and extension */
	public String getSaveFileDir()
	{

		try 
		{
			return this.currentSaveFile.getCanonicalPath().toString().substring(0, this.currentSaveFile.toString().indexOf(this.getSaveFileNameOnly()));

		} catch (IOException e) {
			e.printStackTrace();
			return "not found";
		}

	}

	/*
	 *  this function returns the name of the text file thats opened without the .tim extension 
	 * 
	 * */
	public String getSaveFileNameOnly()
	{
		if( this.currentSaveFile != null )
		{
			if (this.currentSaveFile.getName().contains("."))
			{
				return ( new String ( this.currentSaveFile.getName().substring(0, this.currentSaveFile.getName().lastIndexOf('.'))));

			}
			else if ((this.currentSaveFile!=null))
			{
				return (new String (this.currentSaveFile.getName()));
			}
			else
				return "NULL";
		}
		else
			return "untitled";
	}

	public String getSaveFileName()
	{
		return this.getSaveFileNameOnly();
	}

	/* this fucntion is invoked to save as, by popping up a save dialog first
	 * the function returns true if the text in the text box was succesfully saved to a file
	 */
	public boolean saveAs(String theText)
	{

		int retval = this.myFileChooser.showSaveDialog(null);

		/* open up a file chooser */
		if (retval == JFileChooser.APPROVE_OPTION) {

			File file = this.myFileChooser.getSelectedFile();

			if( new File ( file.toString().concat( ".config" ) ).exists() )
			{
				/* if a file exists, ask the user if he wants to overwrite it */
				int overwrite = JOptionPane.showConfirmDialog(null, "Overwrite?", "Overwrite?", JOptionPane.YES_NO_OPTION);

				if(overwrite == JOptionPane.NO_OPTION)
					return false;
			}	

			/* concat the .config extension to every file */
			if( file.toString().indexOf(".tim") == -1 )
			{
				file = new File ( file.toString().concat( ".tim" ) );
			}

			saveFile( file , theText);

			this.currentSaveFile = file;

			return (true);

		} 
		else 
		{
			return (false);

		}


	}

	/* 
	 * 
	 * the save function is quite simple, it just saves and returns true on success 
	 * 
	 * */
	public boolean save(String txt)
	{

		return (saveFile(this.currentSaveFile, txt));

	}

	/* 
	 * 
	 * the actual saveFile function that invokes a buffered writer
	 * returns true on success and takes in a String to write
	 *
	 */
	public boolean saveFile(File file, String txt) 
	{
		try 
		{

			BufferedWriter writer = new BufferedWriter(new FileWriter(file));
			writer.write(txt);
			writer.close();  

		} 
		catch (IOException e) 
		{
			return (false);
		}

		return (true);
	}

	public boolean mkPrjDir()
	{
		if(new File ( this.getcurrentGameDir() + "/" + this.getSaveFileName() ).exists())
			return true;

		if ( ( ( new File ( this.getcurrentGameDir() + "/" + this.getSaveFileName() ) ).mkdirs() ) != true) 
		{
			return false;
		}

		return true;
	}


	/*
	 * setPWD() is invoked to allow the user to choose a directory for saving the game
	 * returns true if a new path is set.
	 * 
	 */
	public boolean setPWD()
	{
		final JFileChooser fc = new JFileChooser();

		fc.setFileSelectionMode(JFileChooser.DIRECTORIES_ONLY);

		fc.setCurrentDirectory(this.getcurrentGameDir());

		int retval = fc.showOpenDialog(null);

		if (retval == JFileChooser.APPROVE_OPTION) 
		{

			try
			{	
				/*check if the directory the user wants exists */
				if(fc.getSelectedFile().exists() != true)
				{

					/* if not, create it for them */
					if ( ( ( new File ( fc.getSelectedFile ().toString () ) ).mkdirs() ) != true) 
					{
						JOptionPane.showMessageDialog(null,"Could not create directory: " + fc.getSelectedFile () +  " , check permissions?");
						return false;
					}

					this.setcurrentGameDir(fc.getSelectedFile().getCanonicalFile());

				}
				else
				{

					this.setcurrentGameDir(fc.getSelectedFile().getCanonicalFile());

				}


			}catch (IOException e1) {}

		}
		return true;
	}

	/* This function opens a file by invoking the file chooser */
	public String openFile()
	{


		int retval = this.myFileChooser.showOpenDialog(null);

		/* return the directory location */
		if (retval == JFileChooser.APPROVE_OPTION) 
		{
			File file =  this.myFileChooser.getSelectedFile();
			this.currentSaveFile=file;
			return (new String(readFile(file)));
		}
		return (null);
	}

	/* this function is used to browse for an application and as such is not subject 
	 * to the filters of the other file chooser
	 */
	public String openApp()
	{

		JFileChooser appChooser = new JFileChooser();

		int retval = appChooser.showOpenDialog(null);

		if (retval == JFileChooser.APPROVE_OPTION) 
		{
			File file =  appChooser.getSelectedFile();

			return (new String(file.getName()));
		}else
			return (null);
	}

	/* this function opens a given file directly given
	 * passed in directory
	 */
	public String openOther(String dir)
	{

		File file;
		
		try 
		{
			file = new File(new File(".").getCanonicalPath() + "/" +dir);
			return (new String(readFile(file)));
		} 
		catch (IOException e) 
		{}

		return null;

	}
	
	public String openAsHTML( String dir )
	{
		File file;
		
		try {
			
			file = new File(new File(".").getCanonicalPath() + "/" + dir);
			return( new String( readAsHTML( file ) ) );
			
		} catch (IOException e) {
			
			return null;
			
		} catch (NullPointerException n) {
			
			return null;

		}
		
	}
	
	private String readAsHTML( File file )
	{
		
		Scanner inputStream = null;
		try 
		{
			inputStream = new Scanner(new FileReader( file ));
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			return null;
		}
		
		// Andy (2008-01-01): Changed textarea to editor pane (so it can use HTML in the help text and it wraps)
		// But this means it needs a string too.
		String text = "<html><body>";
		
		/* read file in and save to textarea */
		boolean firstLine = true;
		
		while( inputStream.hasNext() )
		{
			text += (firstLine? "" : "<br>") + inputStream.nextLine();
			firstLine = false;
		}
		text += "</body></html>";
		
		return text;
	}

	/* the readFile() fucntion is used to read in a given file using the
	 * BufferedReader. it stores the contents of the file into a string that gets 
	 * passed back to the caller
	 */
	private String readFile(File file)
	{
		String str;
		String out = "";

		try 
		{
			
			BufferedReader in = new BufferedReader(new FileReader(file));


			while ((str = in.readLine()) != null) 
			{
				out += str + "\n";
			}

			in.close();

		} 
		catch (IOException e) 
		{
			
			return null;
		
		}

		return new String(out);
	}

	/* Set the filter fo the file chooser to look for .tim files */
	class configFilter extends javax.swing.filechooser.FileFilter 
	{

		public boolean accept(File file) 
		{
			String filename = file.getName();
			return filename.endsWith(".tim");
		}

		public String getDescription() 
		{
			return "*.tim";
		}
	}
}