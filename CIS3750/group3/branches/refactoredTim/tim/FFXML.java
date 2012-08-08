package tim;

import java.io.*;

public class FFXML implements FilenameFilter
{
	public boolean accept(File dir, String name)
	{
		return name.toLowerCase().endsWith(".xml"); 
	}
}