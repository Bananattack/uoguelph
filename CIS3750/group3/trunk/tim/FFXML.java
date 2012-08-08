package tim;
import java.io.File;
import javax.swing.filechooser.FileFilter;

public class FFXML extends FileFilter
{
	public boolean accept(File f) 
	{
		return f.isDirectory() || f.toString().toLowerCase().endsWith(".xml"); 
	}

	public String getDescription()
	{
		return "Level Format";
	}
}
