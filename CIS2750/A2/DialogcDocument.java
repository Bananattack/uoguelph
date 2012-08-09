// CIS*2750
// Andrew G. Crowell
// 0545826
// acrowell@uoguelph.ca

import java.awt.*;
import javax.swing.*;
import javax.swing.text.*;
import javax.swing.undo.*;

// Stylized document for Dialogc while includes syntax highlighting
public class DialogcDocument extends DefaultStyledDocument
{
	public enum DocumentType { Config, Setup };
	private SimpleAttributeSet normalText;
	private	SimpleAttributeSet commentText;
	private SimpleAttributeSet delimiterText;

	public DialogcDocument(DocumentType documentType)
	{
		normalText = new SimpleAttributeSet();
		StyleConstants.setForeground(normalText, Color.black);

		commentText = new SimpleAttributeSet();
		StyleConstants.setForeground(commentText, new Color(0x00, 0xc0, 0x80));
		StyleConstants.setBackground(commentText, new Color(0xe8, 0xff, 0xf3));

		delimiterText = new SimpleAttributeSet();
		StyleConstants.setForeground(delimiterText, new Color(0xff, 0x00, 0x80));
	}

	public void insertString(int offset, String str, AttributeSet a) throws BadLocationException
	{
		super.insertString(offset, str, a);
		highlightChanges();
	}

	public void remove(int offset, int length) throws BadLocationException
	{
		super.remove(offset, length);
	}

	// Scans code and syntax highlights
	public void highlightChanges() throws BadLocationException
	{		
		String s = getText(0, getLength());
		boolean comment = false;

		// Iterate through the characters in the text and color it
		for(int j = 0; j < s.length(); j++)
		{
			if(!comment)
			{
				// Special delimiters are highlighted with the "delimiterText" style defined above
				if(s.charAt(j) == ';' || s.charAt(j) == '=' || s.charAt(j) == '{' || s.charAt(j) == '}' || s.charAt(j) == ',')
				{
					setCharacterAttributes(j, 1, delimiterText, true);
				}
				// Found a comment character
				else if(s.charAt(j) == '#')
				{
					setCharacterAttributes(j, 1, commentText, true);
					comment = true;
				}
				// "normalText" is applied a simple style attribute
				else
				{
					setCharacterAttributes(j, 1, normalText, true);
				}
			}
			// Comments are highlighted until end-of-line
			else
			{
				setCharacterAttributes(j, 1, commentText, true);
				if(s.charAt(j) == '\n')
				{
					comment = false;
				}
			}
		}
	}
}
