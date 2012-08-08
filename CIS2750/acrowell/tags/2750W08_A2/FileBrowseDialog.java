// CIS*2750
// Andrew G. Crowell
// 0545826
// acrowell@uoguelph.ca
import java.awt.*;
import java.awt.event.*;
import java.io.*;
import javax.swing.*;

// A modal dialog which enables the user to select files, and returns its path to the user.
public class FileBrowseDialog extends JDialog
{
	private JTextField fileField;
	private JFileChooser fileChooser;
	private boolean changed;
	public FileBrowseDialog(Frame owner, String title, File initialValue, JFileChooser fileChooser)
	{
		super(owner, title, true);
		this.fileChooser = fileChooser;

		JPanel panel = new JPanel(new BorderLayout());
		fileField = new JTextField(initialValue.getPath());
		panel.add(new JLabel(title, JLabel.CENTER), BorderLayout.NORTH);
		panel.add(fileField, BorderLayout.CENTER);

		// Browsing opens up the file chooser the user specifies.
		JButton browseButton = new JButton("Browse...");
		browseButton.addActionListener(new ActionListener()
				{
					public void actionPerformed(ActionEvent e)
					{
						int result = getFileChooser().showOpenDialog(FileBrowseDialog.this);
						if (result == JFileChooser.APPROVE_OPTION)
						{
							setSelectedFile(getFileChooser().getSelectedFile());
						}
					}
				}
			);
		panel.add(browseButton, BorderLayout.EAST);

		JPanel bottomButtons = new JPanel();
		JButton okButton = new JButton("OK");
		okButton.addActionListener(new ActionListener()
				{
					public void actionPerformed(ActionEvent e)
					{
						setChanged(true);
						dispose();
					}
				}
			);
		bottomButtons.add(okButton);
		JButton cancelButton = new JButton("Cancel");
		cancelButton.addActionListener(new ActionListener()
				{
					public void actionPerformed(ActionEvent e)
					{
						setChanged(false);
						dispose();
					}
				}
			);
		bottomButtons.add(cancelButton);
		panel.add(bottomButtons, BorderLayout.SOUTH);

		getContentPane().add(panel, BorderLayout.NORTH);

		setSize(400, 100);

		setDefaultCloseOperation(DO_NOTHING_ON_CLOSE);
		addWindowListener(new WindowAdapter()
			{
				public void windowClosing(WindowEvent windowEvent)
				{
					setChanged(false);
					dispose();
				}
			}
		);
		setVisible(true);
	}

	// Gets whether or not the values of the FileBrowseDialog have legitimately changed
	// It's only okay to get the selected file, if this is set to true (ie. the user
	// clicks the OK button) but this class won't prevent you from doing otherwise.
	public boolean getChanged()
	{
		return changed;
	}

	// Sets the change flag described above, and assumes this will
	// only be used internally by FileBrowseDialog
	public void setChanged(boolean changed)
	{
		this.changed = changed;
	}

	// Gets the file chooser associated with this FileBrowseDialog
	public JFileChooser getFileChooser()
	{
		return fileChooser;
	}

	// Sets the currently selected file, and assumes this will
	// only be used internally by FileBrowseDialog
	public void setSelectedFile(File f)
	{
		fileField.setText(f.getPath());
	}

	// Retreives the currently selected file, which is only valid when
	// getChanged() returns true, but this class won't prevent you from checking
	// anyway.
	public File getSelectedFile()
	{
		return new File(fileField.getText());
	}
}
