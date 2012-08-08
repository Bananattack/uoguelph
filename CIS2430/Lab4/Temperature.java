

public class Temperature
{
	private double degrees;
	private char scale;

	public Temperature()
	{
		this('C', 0.0);
	}

	public Temperature(char scale)
	{
		this(scale, 0.0);
	}

	public Temperature(char scale, double degrees)
	{
		this.scale = scale;
		this.degrees = degrees;
	}
	
	public char getScale()
	{
		return scale;
	}

	public double getDegreeC()
	{
		if (scale == 'F')
		{
			return (degrees - 32) / 1.8;
		}
		else if (scale == 'C')
		{
			return degrees;
		}
		return 0.0;
	}

	public void setDegreeC(double degrees)
	{
		this.degrees = degrees;
		scale = 'C';
	}

	public double getDegreeF()
	{
		if (scale == 'F')
		{
			return degrees;
		}
		else if (scale == 'C')
		{
			return 1.8 * degrees + 32;
		}
		return 0.0;
	}

	public void setDegreeF(double degrees)
	{
		this.degrees = degrees;
		scale = 'F';
	}

	public boolean isAboveFreezing()
	{
		return getDegreeC() > 0;
	}

	public String toString()
	{
		return "Temperature (" + degrees + " degrees " + scale + ")";
	}

	public boolean equals(Object rs)
	{
		return rs instanceof Temperature
			&& getDegreeC() == ((Temperature) rs).getDegreeC();
	}

}
