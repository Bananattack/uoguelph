// CIS*2750
// Andrew G. Crowell
// 0545826
// acrowell@uoguelph.ca
import java.io.*;
import java.util.*;

// A compiler which gnaws through the rough and crunchy surface of the .config file
// attempts to hiccup out a compiled Java program, which hopefully works.
public class ConfigCompiler
{
	// Parses the .config, converts to .java and then compiles into a .class.
	// Fairly large, recommending myself to refactor this later!
	public static void compile(File file, File workingDirectory, String compileCommand, String compileArguments) throws IOException, ConfigParseException
	{
		// Create the config manager
		ConfigManager cm = new ConfigManager();

		// Manage the expected parameters on the first pass
		cm.manage("title", ConfigManager.ParameterType.STRING_TYPE, true);
		cm.manage("fields", ConfigManager.ParameterType.LIST_TYPE, true);
		cm.manage("buttons", ConfigManager.ParameterType.LIST_TYPE, true);

		// Parse
		cm.parseFrom(file.getPath() + ".config");

		// Get the values out
		String title = cm.getStringValue("title");
		String[] fields = cm.getListValue("fields");
		String[] buttons = cm.getListValue("buttons");

		// Create a new clean config manager, for a second pass
		cm = new ConfigManager();

		// The second pass will manage all the fields and buttons
		// named in the fields[] and buttons[] lists
		for(String field: fields)
		{
			cm.manage(field, ConfigManager.ParameterType.STRING_TYPE, true);
		}
		for(String button: buttons)
		{
			cm.manage(button, ConfigManager.ParameterType.STRING_TYPE, true);
		}

		// Parse
		cm.parseFrom(file.getPath() + ".config");
	
		// Ensure that fields are of the values "integer", "string", or "float"
		// -- probably used for MySQL database field types in Assignment 4.
		for(String field : fields)
		{
			String value = cm.getStringValue(field);
			if(!value.equals("integer") && !value.equals("string") && !value.equals("float"))
			{
				throw new ConfigParseException("Field '" + field + "' assigned invalid type '" + value + "'");
			}
		}
		
		// Okay, we've got enough info to start coughing out .java code.
		PrintWriter writer = new PrintWriter(new BufferedWriter(new FileWriter(workingDirectory.getPath()+ "/" + file.getName() + "/" + file.getName() + ".java")));

		// Import statements
		writer.println("import java.awt.*;");
		writer.println("import javax.swing.*;");
		writer.println();
		// Class definition
		writer.println("public class " + file.getName() + " extends JFrame");
		writer.println("{");
		writer.println("\t");
		// Instance variables
		writer.println("\t// The status area");
		writer.println("\tJTextArea statusArea;");
		writer.println("\t");
		writer.println("\t// Fields");
		for(String field : fields)
		{
			writer.println("\tprivate JLabel " + field + "_label;");
			writer.println("\tprivate JTextField " + field + "_field;");
		}
		writer.println("\t");
		writer.println("\t// Buttons");
		for(String button : buttons)
		{
			writer.println("\tprivate JButton " + button + "_button;");
		}
		writer.println("\t");
		// The constructor
		writer.println("\t// Constructor");
		writer.println("\tpublic " + file.getName() + "()");
		writer.println("\t{");
		writer.println("\t\tsuper(\"" + title +"\");");
		writer.println("\t\tJPanel fieldsPanel = new JPanel(new BorderLayout());");
		writer.println("\t\tJPanel buttonsPanel = new JPanel();");
		writer.println("\t\tJPanel upperPanel = new JPanel(new BorderLayout());");
		writer.println("\t\tJPanel statusPanel = new JPanel(new BorderLayout());");
		writer.println("\t\tupperPanel.add(fieldsPanel, BorderLayout.NORTH);");
		writer.println("\t\tupperPanel.add(buttonsPanel, BorderLayout.CENTER);");
		writer.println("\t\tgetContentPane().add(upperPanel, BorderLayout.NORTH);");
		writer.println("\t\tgetContentPane().add(statusPanel, BorderLayout.CENTER);");
		writer.println("\t\tJPanel labelPanel = new JPanel(new GridLayout(" + fields.length + ", 1));");
		writer.println("\t\tJPanel textFieldPanel = new JPanel(new GridLayout(" + fields.length + ", 1));");
		writer.println("\t\tfieldsPanel.add(labelPanel, BorderLayout.WEST);");
		writer.println("\t\tfieldsPanel.add(textFieldPanel, BorderLayout.CENTER);");
		for(String field : fields)
		{
			writer.println("\t\t" + field + "_label = new JLabel(\"" + field + "\", JLabel.RIGHT);");
			writer.println("\t\t" + field + "_field = new JTextField(20);");
			writer.println("\t\t" + field + "_label.setLabelFor(" + field + "_field);");
			writer.println("\t\tlabelPanel.add(" + field + "_label);");
			writer.println("\t\ttextFieldPanel.add(" + field + "_field);");
			writer.println("\t\t");
		}

		for(String button : buttons)
		{
			writer.println("\t\t" + button + "_button = new JButton(\"" + button + "\");");
			writer.println("\t\t" + button + "_button.addActionListener(new " + cm.getStringValue(button) + "());");
			writer.println("\t\tbuttonsPanel.add(" + button + "_button);");
			writer.println("\t\t");
		}
		writer.println("\t\tstatusPanel.add(new JLabel(\"Status\", JLabel.CENTER), BorderLayout.NORTH);");
		writer.println("\t\tstatusArea = new JTextArea();");
		writer.println("\t\tstatusArea.setLineWrap(true);");
		writer.println("\t\tstatusArea.setEditable(false);");
		writer.println("\t\tJScrollPane statusScroller = new JScrollPane(statusArea);");
		writer.println("\t\tstatusPanel.add(statusScroller, BorderLayout.CENTER);");
		writer.println("\t\tsetDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);");
		writer.println("\t\tsetSize(600, 400);");
		writer.println("\t\tsetVisible(true);");
		writer.println("\t}");
		writer.println("\t");
		// Accessor/mutator methods, with the current caveat that field names cannot
		// collide with getXXXX/setXXXX defined in JFrame or its ancestors or it
		// will cause wonky behaviour on the frame.
		for(String field : fields)
		{
			writer.println("\tpublic String get" + field + "()");
			writer.println("\t{");
			writer.println("\t\treturn " + field + "_field.getText();");
			writer.println("\t}");
			writer.println("\t");
			writer.println("\tpublic void set" + field + "(String " + field + ")");
			writer.println("\t{");
			writer.println("\t\t" + field + "_field.setText(" + field + ");");
			writer.println("\t}");
			writer.println("\t");
		}
		// Append to status area
		writer.println("\tpublic void appendToStatusArea(String message)");
		writer.println("\t{");
		writer.println("\t\tstatusArea.append(message + \"\\n\");");
		writer.println("\t}");
		writer.println("\t");
		// The main()
		writer.println("\t// Main method.");
		writer.println("\tpublic static void main(String[] args)");
		writer.println("\t{");
		writer.println("\t\tnew " + file.getName() + "();");
		writer.println("\t}");
		writer.println("\t");
		writer.println("}");
		writer.close();

		// Now for the fun part -- compile the code using the command they give you.
		int exitCode = 0;
		String errorMessage = "";
		String cmd = "";
		try
		{
			cmd = compileCommand + " " + compileArguments;
			Process command = Runtime.getRuntime().exec(cmd);
			BufferedReader errorStream = new BufferedReader(new InputStreamReader(command.getErrorStream()));
			String line = "";
			while ((line = errorStream.readLine()) != null)
			{
				errorMessage += line + "\n";
			}
			exitCode = command.waitFor();
		}
		catch (Exception e)
		{
			throw new ConfigParseException("Failed to run command " + e);
		}
		if (exitCode != 0)
		{
			throw new ConfigParseException("Error during compilation:\n" + errorMessage + " " + cmd);
		}
	}

	// Run the code using the command they give you.
	public static void run(File file, String runtimeCommand, String runtimeArguments) throws IOException, ConfigParseException
	{
		int exitCode = 0;
		String errorMessage = "";
		String cmd = "";
		try
		{
			cmd = runtimeCommand + " " + runtimeArguments;
			Process command = Runtime.getRuntime().exec(cmd);
			BufferedReader errorStream = new BufferedReader(new InputStreamReader(command.getErrorStream()));
			BufferedReader printStream = new BufferedReader(new InputStreamReader(command.getInputStream()));
			String line = "";
			while ((line = errorStream.readLine()) != null)
			{
				errorMessage += line + "\n";
			}
			while ((line = printStream.readLine()) != null)
			{
				System.out.println(line);
			}
			exitCode = command.waitFor();
		}
		catch (Exception e)
		{
			throw new ConfigParseException("Failed to run command " + e);
		}
		if (exitCode != 0)
		{
			throw new ConfigParseException("Error during execution:\n" + errorMessage + " " + cmd);
		}
	} 


}
