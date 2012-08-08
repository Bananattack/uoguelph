import org.junit.Test;
import org.junit.Before;
import org.junit.After;
import static org.junit.Assert.assertEquals;

public class TemperatureTest 
{
	private Temperature degree;
	private char C = 'C';
	private char F = 'F';
	
	/**
	 * Creates an instance of the class Temperature.
	 * This instance will be used to perform the tests.
	 */
	@Before public void runBeforeEachTest() {
		degree = new Temperature();
	}
	
	/**
	 * Destroys the test object by "losing" the 
	 * reference of the test object. The garbage collector
	 * will take care of freeing the unreferenced objects.  
	 */
	@After public void runAfterEachTest()  {
		degree = null;
	}
	/**
	 * verify that parameterized constructors are working 
	 */
	@Test public void testConstructors()
	{
		
		char result;
		double storedTemp;
		Temperature oneParam = new Temperature(C);
		result = oneParam.getScale();
		assertEquals("Checking single parameter constructor with C",C, result);

		oneParam = new Temperature(F);
		result = oneParam.getScale();
		assertEquals("Checking single parameter constructor with F",F, result);
	
		double temp = -23;
		Temperature twoParam = new Temperature(C, temp);
		storedTemp = twoParam.getDegreeC();
		assertEquals("Checking two parameter constructor",
				temp, storedTemp, 0.0001);
	}

	/**
	 * Verify that a temperature set in celcius can be
	 * retrieved properly in celcius.
	 */
        @Test public void testDegreeC() {
		double expected = 20;
		degree.setDegreeC(expected);
		double result = degree.getDegreeC();
		assertEquals("Setting the temperature to 20 degree Celcius",
				expected, result, 0.0001);
	}

	/**
	 * Verify that a temperature set in Fahrenheit can be
	 * retrieved properly in Fahrenheit.
	 */
	@Test public void testDegreeF() {
		double expected = 70;
		degree.setDegreeF(expected);
		double result = degree.getDegreeF();
		assertEquals("Setting the temperature to 70 degree Fahrenheit",
				expected, result, 0.0);
	}
	
	/**
	 * Verify that a temperature set in Fahrenheit can be
	 * translated and retrieved in Celcius.
	 */
	@Test public void testDegreeFtoC() {
		double expected = 20;
		degree.setDegreeF(68);
		double result = degree.getDegreeC();
		assertEquals("Reading degree Fahrenheit in Celcius",
				expected, result, 0.0001);
	}
	
	/**
	 * Verify that a temperature set in Celcius can be
	 * translated and retrieved in Fahrenheit.
	 */
	@Test public void testDegreeCtoF() {
		double expected = 77;
		degree.setDegreeC(25);
		double result = degree.getDegreeF();
		assertEquals("Reading degree Celcius in Fahrenheit",
				expected, result, 0.0);
	}
	/**
	 * verify that a temperature is accurately identified as being above or below freezing
	 */
	
	@Test public void testIsAboveFreezing()
	{
		boolean expected = true;
		boolean result;
		double warmTemp = 23;
		double coldTemp = 15;
		degree.setDegreeC(warmTemp);
		result = degree.isAboveFreezing();
		assertEquals("Checking to see if Freezing", expected, result);
		expected = false;
		degree.setDegreeF(coldTemp);
		result = degree.isAboveFreezing();
		assertEquals("Checking to see if Freezing second time", expected, result);
		
	}
	
	@Test public void testEquals()
	{
		Temperature temperatureOne = new Temperature(C);
		Temperature temperatureTwo = new Temperature(F);
		temperatureOne.setDegreeC(25);
		temperatureTwo.setDegreeF(77);
		Boolean expected = true;
		Boolean result;
		result = temperatureOne.equals(temperatureTwo);
		assertEquals("Checking equality one=two", expected, result);
		result = temperatureTwo.equals(temperatureOne);
		assertEquals("Checking equality two=one", expected, result);
	}
}
	
	
	
	


