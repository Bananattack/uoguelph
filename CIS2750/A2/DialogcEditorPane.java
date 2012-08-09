// CIS*2750
// Andrew G. Crowell
// 0545826
// acrowell@uoguelph.ca
import javax.swing.*;
import javax.swing.text.*;

// An editor used by Dialogc that supports stylized documents
// with syntax highlighting and maybe later, undo and redo operations
public class DialogcEditorPane extends JEditorPane
{
	public DialogcEditorPane()
	{
		super();
		setEditable(true);
		setEditorKit(new StyledEditorKit());
	}
}
