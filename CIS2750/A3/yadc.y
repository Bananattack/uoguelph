%{
	/* Configuration parser */
	#include <stdlib.h>
	#include <stdio.h>
	#include "adt/List.h"
	#include "adt/String.h"
	#include "adt/Table.h"

	unsigned int line_count = 1;
	char* filename = "unknown file";
	char* title = NULL;
	List* buffer = NULL;
	List* fields = NULL;
	List* buttons = NULL;
	Table* values = NULL;
%}

%union
{
	char *sval;
}

%token EQUALS
%token SEMICOLON
%token LIST_OPEN
%token LIST_CLOSE
%token COMMA
%token <sval> TITLE
%token <sval> FIELDS
%token <sval> BUTTONS
%token <sval> WORD

%type <sval> expr
%type <sval> string

%%

statement	: statement assignment
		| assignment
		;
string		: TITLE
		| FIELDS
		| BUTTONS
		| WORD
		;
assignment	: TITLE EQUALS expr		{
							if(title == NULL)
							{
								title = $3;
							}
							else
							{
								return yyerror("title already assigned.");
							}
						}
		| FIELDS EQUALS listexpr	{
							if(fields == NULL)
							{
								fields = buffer;
								buffer = NULL;
							}
							else
							{
								return yyerror("fields already assigned.");
							}
						}
		| BUTTONS EQUALS listexpr	{
							if(buttons == NULL)
							{
								buttons = buffer;
								buffer = NULL;
							}
							else
							{
								return yyerror("buttons already assigned.");
							}
						}
		| WORD EQUALS expr		{
							tableAddEntry(values, $1, $3);
						}
		;
expr		: string SEMICOLON		{ $$ = $1; }
		| SEMICOLON			{ $$ = "semicolon"; }
		| LIST_OPEN			{ return yyerror("expected string but got list"); }
		;
listexpr	: LIST_OPEN listitem SEMICOLON
		| string			{ return yyerror("expected list but got string"); }
		| SEMICOLON			{ return yyerror("expected list but got semicolon"); }
		;
listitem	: string COMMA listitem		{
							if(buffer == NULL)
							{
								buffer = listCreate();
							}
							listHead(buffer);
							listAddBefore(buffer, $1);
						}
		| string LIST_CLOSE		{
							if(buffer == NULL)
							{
								buffer = listCreate();
							}
							listHead(buffer);
							listAddBefore(buffer, $1);
						}
		| LIST_CLOSE			{
							if(buffer == NULL)
							{
								buffer = listCreate();
							}
						}
		;

%%

extern FILE *yyin;

int yyerror(char *s)
{
	fprintf(stderr, "%s in %s on line %d\n", s, filename, line_count);
	exit(-1);
	return -1;
}

int main(int argc, char** argv)
{
	int i;
	FILE* input;
	FILE* output;
	char* project_name;
	char* subdirectory_name;
	char* val;
	char fn[4096];

	values = tableCreate(16, StringHash, StringEquals);

	if(argc != 4)
	{
		fprintf(stderr, "\n");
		fprintf(stderr, "Invalid execution '");
		for (i = 0; i < argc; i++)
		{
			fprintf(stderr, "%s ", argv[i]);
		}
		fprintf(stderr, "'\n");
		fprintf(stderr, "Usage: %s [path-to-config] [project-name] [path-to-project-subdirectory]\n", argv[0]);
		fprintf(stderr, "\n");		
		return -1;
	}
	
	filename = argv[1];
	project_name = argv[2];
	subdirectory_name = argv[3];

	input = fopen(filename, "r");
	if(input == NULL)
	{
		fprintf(stderr, "The file '%s' could not be opened for reading %s", filename);
		return -1;
	}
	yyin = input;

	while(!feof(yyin))
	{
		yyparse();
	}

	if(title == NULL)
	{
		fprintf(stderr, "title is not initialized in %s\n", filename);
		return -1;
	}
	if(fields == NULL)
	{
		fprintf(stderr, "fields is not initialized in %s\n", filename);
		return -1;
	}
	if(buttons == NULL)
	{
		fprintf(stderr, "buttons is not initialized in %s\n", filename);
		return -1;
	}

	listHead(fields);
	for (i = 0; i < listLength(fields); i++)
	{
		val = tableGetEntry(values, listGetCurrent(fields)); 
		if(val == NULL)
		{
			fprintf(stderr, "%s is not initialized in %s\n", listGetCurrent(fields), filename);
			return -1;
		}
		else if(!StringEquals(val, "string") && !StringEquals(val, "integer") && !StringEquals(val, "float"))
		{
			fprintf(stderr, "%s is not initialized is to invalid type %s in %s\n", listGetCurrent(fields), val, filename);
			return -1;
		}
		listNext(fields);
	}

	listHead(buttons);
	for (i = 0; i < listLength(buttons); i++)
	{
		if(tableGetEntry(values, listGetCurrent(buttons)) == NULL)
		{
			fprintf(stderr, "%s is not initialized in %s\n", listGetCurrent(buttons), filename);
			return -1;
		}
		listNext(buttons);
	}

	sprintf(fn, "%s/IllegalFieldValue.java", subdirectory_name);
	output = fopen(fn, "w");

	fprintf(output, "public class IllegalFieldValue extends Exception\n");
	fprintf(output, "{\n");
	fprintf(output, "\tpublic IllegalFieldValue() { super(); }\n");
	fprintf(output, "\tpublic IllegalFieldValue(String message) { super(message); }\n");
	fprintf(output, "}\n");
	fclose(output);

	sprintf(fn, "%s/%sFieldEdit.java", subdirectory_name, project_name);
	output = fopen(fn, "w");

	fprintf(output, "public interface %sFieldEdit\n", project_name);
	fprintf(output, "{\n");
	// Accessor/mutator methods defined
	listHead(fields);
	for (i = 0; i < listLength(fields); i++)
	{
		val = listGetCurrent(fields);
		fprintf(output, "\tpublic String getDC%s() throws IllegalFieldValue;\n", val);
		fprintf(output, "\tpublic void setDC%s(String %s);\n", val, val);
		fprintf(output, "\t\n");
		listNext(fields);
	}
	fprintf(output, "\tpublic void appendToStatusArea(String message);\n");
	fprintf(output, "}\n");
	fclose(output);
	
	// Write the Java code
	sprintf(fn, "%s/%s.java", subdirectory_name, project_name);
	output = fopen(fn, "w");

	// Import statements
	fprintf(output, "import java.awt.*;\n");
	fprintf(output, "import javax.swing.*;\n");
	fprintf(output, "\n");
	// Class definition
	fprintf(output, "public class %s extends JFrame implements %sFieldEdit\n", project_name, project_name);
	fprintf(output, "{\n");
	fprintf(output, "\t\n");
	// Instance variables
	fprintf(output, "\t// The status area\n");
	fprintf(output, "\tJTextArea statusArea;\n");
	fprintf(output, "\t\n");
	fprintf(output, "\t// Fields\n");
	listHead(fields);
	for (i = 0; i < listLength(fields); i++)
	{
		val = listGetCurrent(fields);
		fprintf(output, "\tprivate JLabel %s_label;\n", val);
		fprintf(output, "\tprivate JTextField %s_field;\n", val);
		listNext(fields);
	}
	fprintf(output, "\t\n");
	fprintf(output, "\t// Buttons\n");
	listHead(buttons);
	for (i = 0; i < listLength(buttons); i++)
	{
		val = listGetCurrent(buttons);
		fprintf(output, "\tprivate JButton %s_button;\n", val);
		listNext(buttons);
	}
	fprintf(output, "\t\n");
	/* The constructor */
	fprintf(output, "\t// Constructor\n");
	fprintf(output, "\tpublic %s()\n", project_name);
	fprintf(output, "\t{\n");
	fprintf(output, "\t\tsuper(\"%s\");\n", title);
	fprintf(output, "\t\tJPanel fieldsPanel = new JPanel(new BorderLayout());\n");
	fprintf(output, "\t\tJPanel buttonsPanel = new JPanel();\n");
	fprintf(output, "\t\tJPanel upperPanel = new JPanel(new BorderLayout());\n");
	fprintf(output, "\t\tJPanel statusPanel = new JPanel(new BorderLayout());\n");
	fprintf(output, "\t\tupperPanel.add(fieldsPanel, BorderLayout.NORTH);\n");
	fprintf(output, "\t\tupperPanel.add(buttonsPanel, BorderLayout.CENTER);\n");
	fprintf(output, "\t\tgetContentPane().add(upperPanel, BorderLayout.NORTH);\n");
	fprintf(output, "\t\tgetContentPane().add(statusPanel, BorderLayout.CENTER);\n");
	fprintf(output, "\t\tJPanel labelPanel = new JPanel(new GridLayout(%d, 1));\n", listLength(fields));
	fprintf(output, "\t\tJPanel textFieldPanel = new JPanel(new GridLayout(%d, 1));\n", listLength(fields));
	fprintf(output, "\t\tfieldsPanel.add(labelPanel, BorderLayout.WEST);\n");
	fprintf(output, "\t\tfieldsPanel.add(textFieldPanel, BorderLayout.CENTER);\n");
	listHead(fields);
	for (i = 0; i < listLength(fields); i++)
	{
		val = listGetCurrent(fields);
		fprintf(output, "\t\t%s_label = new JLabel(\"%s\", JLabel.RIGHT);\n", val, val);
		fprintf(output, "\t\t%s_field = new JTextField(20);\n", val);
		fprintf(output, "\t\t%s_label.setLabelFor(%s_field);\n", val, val);
		fprintf(output, "\t\tlabelPanel.add(%s_label);\n", val);
		fprintf(output, "\t\ttextFieldPanel.add(%s_field);\n", val);
		fprintf(output, "\t\t\n");
		listNext(fields);
	}
	listHead(buttons);
	for (i = 0; i < listLength(buttons); i++)
	{
		val = listGetCurrent(buttons);
		fprintf(output, "\t\t%s_button = new JButton(\"%s\");\n", val, val);
		fprintf(output, "\t\t%s_button.addActionListener(new %s(this));\n", val, tableGetEntry(values, val));
		fprintf(output, "\t\tbuttonsPanel.add(%s_button);\n", val);
		fprintf(output, "\t\t\n");
		listNext(buttons);
	}
	fprintf(output, "\t\tstatusPanel.add(new JLabel(\"Status\", JLabel.CENTER), BorderLayout.NORTH);\n");
	fprintf(output, "\t\tstatusArea = new JTextArea();\n");
	fprintf(output, "\t\tstatusArea.setLineWrap(true);\n");
	fprintf(output, "\t\tstatusArea.setEditable(false);\n");
	fprintf(output, "\t\tJScrollPane statusScroller = new JScrollPane(statusArea);\n");
	fprintf(output, "\t\tstatusPanel.add(statusScroller, BorderLayout.CENTER);\n");
	fprintf(output, "\t\tsetDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);\n");
	fprintf(output, "\t\tsetSize(600, 400);\n");
	fprintf(output, "\t\tsetVisible(true);\n");
	fprintf(output, "\t}\n");
	fprintf(output, "\t\n");
	/* Accessor/mutator methods implemented */
	listHead(fields);
	for (i = 0; i < listLength(fields); i++)
	{
		val = listGetCurrent(fields);
		fprintf(output, "\tpublic String getDC%s() throws IllegalFieldValue\n", val);
		fprintf(output, "\t{\n");
		if(StringEquals(tableGetEntry(values, val), "integer"))
		{
			fprintf(output, "\ttry\n");
			fprintf(output, "\t{\n");
			fprintf(output, "\t\tInteger.parseInt(%s_field.getText());\n", val);
			fprintf(output, "\t}\n");
			fprintf(output, "\tcatch(NumberFormatException e)\n");
			fprintf(output, "\t{\n");
			fprintf(output, "\t\tthrow new IllegalFieldValue(%s_field.getText());\n", val);
			fprintf(output, "\t}\n");
		}
		else if(StringEquals(tableGetEntry(values, val), "float"))
		{
			fprintf(output, "\ttry\n");
			fprintf(output, "\t{\n");
			fprintf(output, "\t\tFloat.parseFloat(%s_field.getText());\n", val);
			fprintf(output, "\t}\n");
			fprintf(output, "\tcatch(NumberFormatException e)\n");
			fprintf(output, "\t{\n");
			fprintf(output, "\t\tthrow new IllegalFieldValue(%s_field.getText());\n", val);
			fprintf(output, "\t}\n");
		}
		fprintf(output, "\t\treturn %s_field.getText();\n", val);
		fprintf(output, "\t}\n");
		fprintf(output, "\t\n");
		fprintf(output, "\tpublic void setDC%s(String %s)\n", val, val);
		fprintf(output, "\t{\n");
		fprintf(output, "\t\t%s_field.setText(%s);\n", val, val);
		fprintf(output, "\t}\n");
		fprintf(output, "\t\n");
		listNext(fields);
	}
	/* Append to status area */
	fprintf(output, "\tpublic void appendToStatusArea(String message)\n");
	fprintf(output, "\t{\n");
	fprintf(output, "\t\tstatusArea.append(message + \"\\n\");\n");
	fprintf(output, "\t}\n");
	fprintf(output, "\t\n");
	/* The main() */
	fprintf(output, "\t// Main method.\n");
	fprintf(output, "\tpublic static void main(String[] args)\n");
	fprintf(output, "\t{\n");
	fprintf(output, "\t\tnew %s();\n", project_name);
	fprintf(output, "\t}\n");
	fprintf(output, "\t\n");
	fprintf(output, "}\n");
	fclose(output);
}
