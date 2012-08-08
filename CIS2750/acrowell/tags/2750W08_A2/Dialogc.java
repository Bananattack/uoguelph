// CIS*2750
// Andrew G. Crowell
// 0545826
// acrowell@uoguelph.ca

import java.awt.*;
import java.awt.event.*;

import java.util.*;

import java.io.*;

import javax.swing.*;
import javax.swing.text.*;

// Dialogc is your big friendly father program that lets you make fun things with configuration files!
public class Dialogc extends JFrame implements KeyListener
{
	public enum UnsavedDialogChoice { SAVE, IGNORE, CANCEL };

	public static final String TITLE = "Dialogc";
	public static final String ERROR_MESSAGE_LOAD_FAILED = "Couldn't open the document. Make sure that you have permissions to access this file, and make sure the filename was entered properly.";
	public static final String ERROR_MESSAGE_SAVE_FAILED = "Couldn't save the document. Make sure that you have write permissions for the location you're trying to save to.";


	private DialogcEditorPane editorConfig;
	private JFileChooser fileChooser;
	private DialogcDocument documentConfig, documentSetup;
	private ActionListener fileNewAction, fileOpenAction, fileSaveAction, fileSaveAsAction, compileAction, compileAndRunAction;
	private File compileCommand = new File("javac");
	private String compilerArguments = "";
	private File runtimeCommand = new File("java");
	private String runtimeArguments = "";
	private File workingDirectory = new File(".");
	private String filename, filenameWithoutExtension;
	private javax.swing.filechooser.FileFilter configFilter;
	private boolean untitled = false;
	private boolean modified = false;
	private int fileIndex;


	public Dialogc()
	{
		super();

		// A JFileChooser for fancy popup dialogs
		fileChooser = new JFileChooser();

		// A file filter is important, so we only see .config files when
		// searching a big fat bloated directory that seriously needs to lose its weight in files
		configFilter = new javax.swing.filechooser.FileFilter()
			{
				public boolean accept(File file)
				{
					return file.getName().endsWith(".config");
				}
				public String getDescription()
				{
					return "Dialogc Configuration (*.config)";
				}
			};
		fileChooser.addChoosableFileFilter(configFilter);
		
		setSize(640, 480);

		// Create the editor, make it scrollable!
		editorConfig = new DialogcEditorPane();
		editorConfig.addKeyListener(this);
		JScrollPane scrollPane = new JScrollPane(editorConfig);

		getContentPane().add(BorderLayout.CENTER, scrollPane);

		// Create menu bar (for dropdown lists) and toolbar (for editor buttons)
		JMenuBar menuBar = new JMenuBar();
		JToolBar toolBar = new JToolBar();
		toolBar.setFloatable(false);

		// File menu and its respective items
		JMenu fileMenu = new JMenu("File");
		fileMenu.setMnemonic(KeyEvent.VK_F);
		JMenuItem fileMenuItemNew = new JMenuItem("New");
		JButton fileNewButton = new IconButton("icons/new32.png");
		fileNewAction = new ActionListener()
				{
					public void actionPerformed(ActionEvent event)
					{
						newDocument();
					}
				};
		fileMenuItemNew.setAccelerator(KeyStroke.getKeyStroke("ctrl N"));
		fileMenuItemNew.addActionListener(fileNewAction);
		fileNewButton.addActionListener(fileNewAction);
		toolBar.add(fileNewButton);
		JMenuItem fileMenuItemOpen = new JMenuItem("Open...");
		JButton fileOpenButton = new IconButton("icons/open32.png");
		fileOpenAction = new ActionListener()
				{
					public void actionPerformed(ActionEvent event)
					{						openDocument();
					}
				};
		fileMenuItemOpen.setAccelerator(KeyStroke.getKeyStroke("ctrl O"));
		fileMenuItemOpen.addActionListener(fileOpenAction);
		fileOpenButton.addActionListener(fileOpenAction);
		toolBar.add(fileOpenButton);
		JMenuItem fileMenuItemSave = new JMenuItem("Save");
		JButton fileSaveButton = new IconButton("icons/save32.png");
		fileSaveAction = new ActionListener()
				{
					public void actionPerformed(ActionEvent event)
					{
						try
						{							saveDocument();
						}
						catch(IOException e)
						{
							errorMessage(ERROR_MESSAGE_SAVE_FAILED);
							return;
						}
					}
				};
		fileMenuItemSave.setAccelerator(KeyStroke.getKeyStroke("ctrl S"));
		fileMenuItemSave.addActionListener(fileSaveAction);
		fileSaveButton.addActionListener(fileSaveAction);
		toolBar.add(fileSaveButton);
		JMenuItem fileMenuItemSaveAs = new JMenuItem("Save As...");
		fileSaveAsAction = new ActionListener()
				{
					public void actionPerformed(ActionEvent event)
					{						try
						{							saveAsDocument();
						}
						catch(IOException e)
						{
							errorMessage(ERROR_MESSAGE_SAVE_FAILED);
							return;
						}
					}
				};
		fileMenuItemSaveAs.setAccelerator(KeyStroke.getKeyStroke("ctrl shift S"));
		fileMenuItemSaveAs.addActionListener(fileSaveAsAction);
		JMenuItem fileMenuItemQuit = new JMenuItem("Quit");
		fileMenuItemQuit.addActionListener(new ActionListener()
				{
					public void actionPerformed(ActionEvent event)
					{
						quit();
					}
				}
		);
		fileMenuItemQuit.setAccelerator(KeyStroke.getKeyStroke("ctrl Q"));
		fileMenu.add(fileMenuItemNew);
		fileMenu.add(fileMenuItemOpen);
		fileMenu.add(fileMenuItemSave);
		fileMenu.add(fileMenuItemSaveAs);
		fileMenu.add(fileMenuItemQuit);

		// Compile menu and its respective items
		JMenu compileMenu = new JMenu("Compile");
		compileMenu.setMnemonic(KeyEvent.VK_C);
		JMenuItem compileMenuItemCompile = new JMenuItem("Compile");
		JButton compileCompileButton = new IconButton("icons/Boxes32.png");
		compileAction = new ActionListener()
				{
					public void actionPerformed(ActionEvent event)
					{
			           		compileDocument();
					}
				};
		compileMenuItemCompile.setAccelerator(KeyStroke.getKeyStroke("ctrl E"));
		compileMenuItemCompile.addActionListener(compileAction);
		compileCompileButton.addActionListener(compileAction);
		toolBar.add(compileCompileButton);

		JMenuItem compileMenuItemCompileAndRun = new JMenuItem("Compile and Run");
		JButton compileCompileAndRunButton = new IconButton("icons/play32.png");
		compileAndRunAction = new ActionListener()
				{
					public void actionPerformed(ActionEvent event)
					{
						compileAndRunDocument();
					}
				};
		compileMenuItemCompileAndRun.setAccelerator(KeyStroke.getKeyStroke("ctrl R"));
		compileMenuItemCompileAndRun.addActionListener(compileAndRunAction);
		compileCompileAndRunButton.addActionListener(compileAndRunAction);
		toolBar.add(compileCompileAndRunButton);
		compileMenu.add(compileMenuItemCompile);
		compileMenu.add(compileMenuItemCompileAndRun);

		// Configuration menu and its respective items
		JMenu configMenu = new JMenu("Config");
		configMenu.setMnemonic(KeyEvent.VK_O);
		JMenuItem configMenuItemJavaCompiler = new JMenuItem("Java Compiler...");
		configMenuItemJavaCompiler.addActionListener(new ActionListener()
				{
					public void actionPerformed(ActionEvent event)
					{
						fileChooser.setFileFilter(null);
						FileBrowseDialog dialog = new FileBrowseDialog(Dialogc.this, "Java Compiler", compileCommand, fileChooser);
						if(dialog.getChanged())
						{
							compileCommand = dialog.getSelectedFile();
						}
						fileChooser.addChoosableFileFilter(configFilter);
					}
				}
			);
		configMenuItemJavaCompiler.setAccelerator(KeyStroke.getKeyStroke("F2"));
		JMenuItem configMenuItemCompileOptions = new JMenuItem("Compile Options...");
		configMenuItemCompileOptions.addActionListener(new ActionListener()
				{
					public void actionPerformed(ActionEvent event)
					{
						Object result = JOptionPane.showInputDialog(Dialogc.this, "Compile Options", "Compile Options",
							JOptionPane.PLAIN_MESSAGE, null, null, compilerArguments);
						if(result != null)
						{
							compilerArguments = (String) result;
						}
					}
				}
			);
		configMenuItemCompileOptions.setAccelerator(KeyStroke.getKeyStroke("F3"));
		JMenuItem configMenuItemJavaRuntime = new JMenuItem("Java Runtime...");
		configMenuItemJavaRuntime.addActionListener(new ActionListener()
				{
					public void actionPerformed(ActionEvent event)
					{
						fileChooser.setFileFilter(null);
						FileBrowseDialog dialog = new FileBrowseDialog(Dialogc.this, "Java Runtime", runtimeCommand, fileChooser);
						if(dialog.getChanged())
						{
							runtimeCommand = dialog.getSelectedFile();
						}
						fileChooser.addChoosableFileFilter(configFilter);
					}
				}
			);
		configMenuItemJavaRuntime.setAccelerator(KeyStroke.getKeyStroke("F4"));
		JMenuItem configMenuItemRuntimeOptions = new JMenuItem("Runtime Options...");
		configMenuItemRuntimeOptions.addActionListener(new ActionListener()
				{
					public void actionPerformed(ActionEvent event)
					{
						Object result = JOptionPane.showInputDialog(Dialogc.this, "Runtime Options", "Runtime Options",
							JOptionPane.PLAIN_MESSAGE, null, null, runtimeArguments);
						if(result != null)
						{
							runtimeArguments = (String) result;
						}
					}
				}
			);
		configMenuItemRuntimeOptions.setAccelerator(KeyStroke.getKeyStroke("F5"));
		JMenuItem configMenuItemWorkingDirectory = new JMenuItem("Working Directory...");
		configMenuItemWorkingDirectory.addActionListener(new ActionListener()
				{
					public void actionPerformed(ActionEvent event)
					{
						fileChooser.setFileFilter(null);
						fileChooser.setFileSelectionMode(JFileChooser.DIRECTORIES_ONLY);
						FileBrowseDialog dialog = new FileBrowseDialog(Dialogc.this, "Working Directory", workingDirectory, fileChooser);
						if(dialog.getChanged())
						{
							workingDirectory = dialog.getSelectedFile();
							updateArguments();
						}
						fileChooser.setFileSelectionMode(JFileChooser.FILES_ONLY);
						fileChooser.addChoosableFileFilter(configFilter);
					}
				}
			);
		configMenuItemWorkingDirectory.setAccelerator(KeyStroke.getKeyStroke("F6"));
		configMenu.add(configMenuItemJavaCompiler);
		configMenu.add(configMenuItemCompileOptions);
		configMenu.add(configMenuItemJavaRuntime);
		configMenu.add(configMenuItemRuntimeOptions);
		configMenu.add(configMenuItemWorkingDirectory);

		// Help menu and its respective items
		JMenu helpMenu = new JMenu("Help");
		helpMenu.setMnemonic(KeyEvent.VK_H);
		JMenuItem helpMenuItemHelp = new JMenuItem("Help");
		helpMenuItemHelp.addActionListener(new ActionListener()
				{
					public void actionPerformed(ActionEvent event)
					{
						String message = "";
						Scanner scanner;
						try
						{
							scanner = new Scanner(new BufferedReader(new FileReader("README")));
						}
						catch(Exception e)
						{
							return;
						}
						while(scanner.hasNextLine())
						{
							message += scanner.nextLine() + "\n";
						}
						scanner.close();

						JTextArea area = new JTextArea(message);
						area.setRows(20);
						area.setColumns(50);
						area.setEditable(false);
					        area.setLineWrap(true);
						JOptionPane.showMessageDialog(null, new JScrollPane(area), "Help", JOptionPane.PLAIN_MESSAGE);
					}
				}
		);
		helpMenuItemHelp.setAccelerator(KeyStroke.getKeyStroke("F1"));
		JMenuItem helpMenuItemAbout = new JMenuItem("About");
		helpMenuItemAbout.addActionListener(new ActionListener()
				{
					public void actionPerformed(ActionEvent event)
					{
						popupMessage("Dialogc (CIS*2750 Assignment 2)\n"
								+ "by Andrew Crowell\n"
								+ "0545826\n"
								+ "acrowell@uoguelph.ca\n");
					}
				}
		);
		helpMenuItemAbout.setAccelerator(KeyStroke.getKeyStroke("ctrl alt H"));
		helpMenu.add(helpMenuItemHelp);
		helpMenu.add(helpMenuItemAbout);

		// Add all the menus we created to the menu bar, and add the
		// menu bar and toolbar to this frame
		menuBar.add(fileMenu);
		menuBar.add(compileMenu);
		menuBar.add(configMenu);
		menuBar.add(helpMenu);
		setJMenuBar(menuBar);
		getContentPane().add(toolBar, BorderLayout.NORTH);

		// Appear!
		setVisible(true);

		// Make it so that a quit dialog pops up with menacing questions
		// like "You have unsaved changes ARE YOU SURE YOU WANT CLOSE".
		setDefaultCloseOperation(DO_NOTHING_ON_CLOSE);
		addWindowListener(new WindowAdapter()
				{
					public void windowClosing(WindowEvent windowEvent)
					{
						quit();
					}
				}
		);

		// Create a new document, so that there's something
		// in the editor from the start
		newDocument();
	}

	// The quit operation, basically a System.exit() call, but
	// it lets you save first.
	public void quit()
	{
		if(modified)
		{
			switch(unsavedChangesDialog())
			{
				case SAVE:
					try
					{
						if(!saveDocument())
						{
							return;
						}
					}
					catch(IOException e)
					{
						errorMessage(ERROR_MESSAGE_SAVE_FAILED);
						return;
					}
					break;
				case IGNORE:
					break;
				case CANCEL:
					return;
			}
		}
		System.exit(0);
	}

	// Create a new document.
	public void newDocument()
	{
		if(modified)
		{
			switch(unsavedChangesDialog())
			{
				case SAVE:
					try
					{
						if(!saveDocument())
						{
							return;
						}
					}
					catch(IOException e)
					{
						errorMessage(ERROR_MESSAGE_SAVE_FAILED);
						return;
					}
					break;
				case IGNORE:
					break;
				case CANCEL:
					return;
			}
		}

		untitled = true;
		setModified(false);
		fileIndex++;
		setFilename("Document " + fileIndex);
		documentConfig = new DialogcDocument(DialogcDocument.DocumentType.Config);
		editorConfig.setDocument(documentConfig);
		editorConfig.requestFocus();
	}

	// Open an existing document.
	public void openDocument()
	{
		if(modified)
		{
			switch(unsavedChangesDialog())
			{
				case SAVE:
					try
					{
						if(!saveDocument())
						{
							return;
						}
					}
					catch(IOException e)
					{
						errorMessage(ERROR_MESSAGE_SAVE_FAILED);
						return;
					}
					break;
				case IGNORE:
					break;
				case CANCEL:
					return;
			}
		}

		try
		{
			File f = new File(workingDirectory.getCanonicalPath());
			fileChooser.setCurrentDirectory(f);
		}
		catch (IOException e)
		{
		}
		int result = fileChooser.showOpenDialog(this);
		String contents;
		if (result != JFileChooser.APPROVE_OPTION)
		{
			return;
		}
		try
		{
			contents = loadFile(fileChooser.getSelectedFile().getPath());
		}
		catch(IOException e)
		{
			errorMessage(ERROR_MESSAGE_LOAD_FAILED);
			return;
		}

		setFilename(fileChooser.getSelectedFile().getPath());
		documentConfig = new DialogcDocument(DialogcDocument.DocumentType.Config);

		try
		{
			documentConfig.insertString(0, contents, null);
		}
		catch(BadLocationException e)
		{
		}
		editorConfig.setDocument(documentConfig);
	}

	// Refresh the title, so that modifications and filename changes can be visible to the user.
	public void refreshTitle()
	{
		setTitle((modified ? "*" : "") + TITLE + " - " + filename);
	}

	// Set the filename, and do all sorts of cool updating associated with it.
	public void setFilename(String filename)
	{
		int dotPosition;

		this.filename = filename;

		dotPosition = filename.lastIndexOf(".");
		if(dotPosition == -1)
		{
			filenameWithoutExtension = filename;
		}
		else
		{
			filenameWithoutExtension = filename.substring(0, dotPosition);
		}
		refreshTitle();
		updateArguments();
	}

	// Set the modification flag on a document so that unsaved changes can be detected.
	public void setModified(boolean modified)
	{
		this.modified = modified;
		refreshTitle();
	}

	// Update the commandline arguments to a default which reflects the current 
	// working directory and filename.
	public void updateArguments()
	{
		String projectName = new File(filenameWithoutExtension).getName();
		compilerArguments = "-classpath " + workingDirectory + ":" + workingDirectory + "/" + projectName + " -d " + workingDirectory + "/" + projectName
					+ " " + workingDirectory + "/" + projectName + "/" + projectName + ".java";
		runtimeArguments = "-classpath " + workingDirectory + ":"  + workingDirectory + "/" + projectName
					+ " " + projectName;
	}

	// The prompt to save the document, but ask for the name and location first.
	public boolean saveAsDocument() throws IOException
	{
		try
		{
			File f = new File(workingDirectory.getCanonicalPath());

			fileChooser.setCurrentDirectory(f);
		}
		catch (IOException e)
		{
		}
		int result = fileChooser.showSaveDialog(this);
		if (result == JFileChooser.APPROVE_OPTION)
		{
			setFilename(fileChooser.getSelectedFile().getPath());
			saveFile(filename);
			return true;
		}
		return false;
	}

	// The prompt to save the document, remembering the last location saved,
	// or prompting the user in the instance the file isn't yet named.
	public boolean saveDocument() throws IOException
	{

		if(untitled)
		{
			return saveAsDocument();
		}
		else
		{
			saveFile(filename);
			return true;
		}
	}

	// The prompt to compile the document into a .java and then into a .class
	// in a subfolder of the working directory
	public boolean compileDocument()
	{	
		if(modified || untitled)
		{
			switch(unsavedChangesDialog("Compilation requires that files be saved before they can be parsed.\n", "\nNot saving will cause the compilation to either fail or use an outdated copy of the file.\n"))
			{
				case SAVE:
					try
					{
						if(!saveDocument())
						{
							return false;
						}
					}
					catch(IOException e)
					{
						errorMessage(ERROR_MESSAGE_SAVE_FAILED);
						return false;
					}
					break;
				case IGNORE:
					break;
				case CANCEL:
					return false;
			}
		}

		try
		{
			compileFile(filename);
		}
		catch(IOException e)
		{
			errorMessage(e.getMessage());
			return false;
		}
		catch(ConfigParseException e)
		{
			errorMessage(e.getMessage());
			return false;
		}
		return true;
	}

	// The prompt to compile the document into a .java and then into a .class
	// in a subfolder of the working directory, and then ADDITIONALLY, attempt
	// to run it if the compilation succeeded.
	public void compileAndRunDocument()
	{
		if(compileDocument())
		{
			try
			{
				runFile(filename);
			}
			catch(IOException e)
			{
				errorMessage(e.getMessage());	
			}
			catch(ConfigParseException e)
			{
				errorMessage(e.getMessage());
			}
		}
	}

	// Do the actual saving of the file, now knowing the details like the path to the file
	public void saveFile(String filename) throws IOException
	{
		try
	    	{
			PrintWriter writer = new PrintWriter(new BufferedWriter(new FileWriter(filename)));
			writer.print(documentConfig.getText(0, documentConfig.getLength()));
			writer.close();
		}
		catch(BadLocationException e)
		{
		}

		untitled = false;
		setModified(false);
	}

	// Do the actual loaing of the file, now knowing the details like the path to the file
	public String loadFile(String filename) throws IOException
	{
		String loadResult = "";
		Scanner scanner = new Scanner(new BufferedReader(new FileReader(filename)));
		while(scanner.hasNextLine())
		{
			loadResult += scanner.nextLine() + "\n";
		}
		scanner.close();

		untitled = false;
		setModified(false);
		return loadResult;
	}

	// Do the actual compilation of the file
	public void compileFile(String filename) throws IOException, ConfigParseException
	{
		boolean success = false;
		File f = null;

		f = new File(workingDirectory + "/" + new File(filenameWithoutExtension).getName());
		if(f.isDirectory())
		{
			String[] choices = { "Compile", "Don't compile" };
			int result = JOptionPane.showOptionDialog(this, "A directory named '" + f.getPath() + "' already exists? Compile and overwrite?", "Warning", JOptionPane.YES_NO_OPTION, JOptionPane.WARNING_MESSAGE, null, choices, choices[0]);
			if(result != 0)
			{
				return;
			}
		}

		try
		{
			success = f.isDirectory() || f.mkdir();
		}
		catch(SecurityException e)
		{
		}
		if(!success)
		{
			errorMessage("The project subdirectory '" + filename + "' could not be created. Cancelling compilation.");
			return;
		}
		
		ConfigCompiler.compile(new File(filenameWithoutExtension), workingDirectory, compileCommand.getPath(), compilerArguments);
	}

	// Do the actual running of the file, which assumes that a compilation has already been performed.
	public void runFile(String filename) throws IOException, ConfigParseException
	{
		ConfigCompiler.run(new File(filenameWithoutExtension), runtimeCommand.getPath(), runtimeArguments);
	}

	// Creates a popup with a specified message	
	public void popupMessage(String message)
	{
		JOptionPane.showMessageDialog(this, message, "Message", JOptionPane.INFORMATION_MESSAGE);
	}

	// Creates a popup with a specified error message
	public void errorMessage(String message)
	{
		JOptionPane.showMessageDialog(this, message, "Error", JOptionPane.ERROR_MESSAGE);
	}

	// Prompts the user to save changes, continue without saving at all,
	// or cancel any action that will wipe the unsaved changes from the editor.
	public UnsavedDialogChoice unsavedChangesDialog()
	{
		return unsavedChangesDialog("", "");
	}

	// Prompts the user to save changes, continue without saving at all,
	// or cancel any action that will wipe the unsaved changes from the editor.
	// Additionally provides text that goes before and after the general unsaved changes message.
	public UnsavedDialogChoice unsavedChangesDialog(String preceding, String succeeding)
	{
		String[] choices = { "Save", "Continue without saving", "Cancel" };
		int result = JOptionPane.showOptionDialog(this, preceding + "You have unsaved changes. Save first?" + succeeding, "Warning", JOptionPane.YES_NO_CANCEL_OPTION, JOptionPane.WARNING_MESSAGE, null, choices, choices[0]);
		switch(result)
		{
			case 0:
				return UnsavedDialogChoice.SAVE;
			case 1:
				return UnsavedDialogChoice.IGNORE;
			case 2:
			default:
				return UnsavedDialogChoice.CANCEL;
		}
	}

	// Flags as modified if a printable character is typed into the document
	public void keyTyped(KeyEvent e)
	{
		if(!e.isAltDown() && !e.isControlDown())
		{
			setModified(true);
		}
	}

	public void keyPressed(KeyEvent e)
	{
	}

	// If any special key combinations can modify the text document,
	// for instance, the cut command (but not copy), and paste,
	// put them here, so the modifications are recorded.
	public void keyReleased(KeyEvent e)
	{
		if(e.isControlDown()
			&& (e.getKeyCode() == KeyEvent.VK_X
				|| e.getKeyCode() == KeyEvent.VK_V)
			)
		{
			setModified(true);
		}
	}

	// Create the main window in its full sexy codified glory.
	public static void main(String[] args)
	{
		new Dialogc();
	}
}
