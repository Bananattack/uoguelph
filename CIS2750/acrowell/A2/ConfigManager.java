// CIS*2750
// Andrew G. Crowell
// 0545826
// acrowell@uoguelph.ca

// Config manager, does native wrapping around the ParameterManager made in Assignment 1
public class ConfigManager
{
	static { System.loadLibrary("ConfigManager"); }

	public enum ParameterType { STRING_TYPE, INT_TYPE, LIST_TYPE, BOOLEAN_TYPE, REAL_TYPE };

	public ConfigManager()
	{
		create();
	}
	
	public void finalize()
	{
		destroy();
	}

	private int internalPointer;
	private native void create();
	private native void destroy();

	public native void manage(String parameterName, byte parameterType, boolean required);
	public native boolean parseFrom(String filename) throws ConfigParseException;
	public native boolean hasValue(String parameterName);
	public native String getStringValue(String parameterName);
	public native int getIntValue(String parameterName);
	public native float getFloatValue(String parameterName);
	public native boolean getBooleanValue(String parameterName);
	public native String[] getListValue(String parameterName);

	public void manage(String parameterName, ParameterType parameterType, boolean required)
	{
		byte type = '\0';
		switch(parameterType)
		{
			case STRING_TYPE:
				type = 's';
				break;
			case INT_TYPE:
				type = 'i';
				break;
			case LIST_TYPE:
				type = 'l';
				break;
			case BOOLEAN_TYPE:
				type = 'b';
				break;
			case REAL_TYPE:
				type = 'r';
				break;
		}
		manage(parameterName, type, required);
	}

	public static void main(String[] args)
	{
		ConfigManager cm = new ConfigManager();
		cm.manage("name", ParameterType.STRING_TYPE, true);
		cm.manage("information", ParameterType.STRING_TYPE, false);
		cm.manage("hp", ParameterType.INT_TYPE, true);
		cm.manage("mp", ParameterType.INT_TYPE, false);
		cm.manage("skills", ParameterType.LIST_TYPE, true);
		cm.manage("is_boss", ParameterType.BOOLEAN_TYPE, false);
		cm.manage("kill_ratio", ParameterType.REAL_TYPE, true);
		try
		{
			cm.parseFrom("monster.txt");
		}
		catch(ConfigParseException e)
		{
			System.out.println(e);
		}
		System.out.println("Name: " + cm.getStringValue("name"));
		System.out.println("HP: " + cm.getIntValue("hp"));

		for (String s: cm.getListValue("skills"))
		{
			System.out.println("Skill " + s);
		}
	}

}

