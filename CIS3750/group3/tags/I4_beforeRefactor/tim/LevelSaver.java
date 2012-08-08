package tim;

import java.io.File;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;

import org.w3c.dom.Document;

/**
 * @author Darren O'Shea
 *
 */
public class LevelSaver 
{
	public static boolean save(String path)
	{
		Document dom;

		try
		{
			DocumentBuilderFactory domFactory = DocumentBuilderFactory.newInstance();
			DocumentBuilder domBuilder = domFactory.newDocumentBuilder();
			dom = domBuilder.newDocument();
		}
		catch(Exception e)
		{
			return false;
		}
		
		StreamResult res = new StreamResult(new File(path));
		
		TransformerFactory autobots = TransformerFactory.newInstance();
		Transformer optimus;
		try
		{
			optimus = autobots.newTransformer();
			optimus.transform(new DOMSource(dom), res);
		}
		catch (Exception e)
		{
			e.printStackTrace();
		}
		
		System.out.println(res);
		
		return true;
	}
}
