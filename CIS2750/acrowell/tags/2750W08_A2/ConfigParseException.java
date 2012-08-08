// CIS*2750
// Andrew G. Crowell
// 0545826
// acrowell@uoguelph.ca

// Exception thrown when parsing/compilation/execution of configuration code fails.
public class ConfigParseException extends Exception
{
	public ConfigParseException()
	{
		super();
	}

	public ConfigParseException(String s)
	{
		super(s);
	}
}
