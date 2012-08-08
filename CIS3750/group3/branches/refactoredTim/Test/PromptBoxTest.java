/**
 * 
 */
package Test;

import javax.swing.ImageIcon;

import tim.*;
import Widgets.*;
import junit.framework.TestCase;

/**
 * @author doshea
 *
 */
public class PromptBoxTest extends TestCase {

	/**
	 * @param name
	 */
	public PromptBoxTest(String name) {
		super(name);
	}

	/* (non-Javadoc)
	 * @see junit.framework.TestCase#setUp()
	 */
	protected void setUp() throws Exception {
		super.setUp();
	}

	/* (non-Javadoc)
	 * @see junit.framework.TestCase#tearDown()
	 */
	protected void tearDown() throws Exception {
		super.tearDown();
	}

	/**
	 * Test method for {@link tim.PromptBox#show(javax.swing.ImageIcon, java.lang.String, java.lang.String, int, int)}.
	 */
	public final void testShowImageIconStringStringIntInt() {
		PromptBox pbTest = new PromptBox();
		pbTest.show(new ImageIcon("resources/icons/dialog-error.png"), "Achtung!", "Your U-boat is sinking.", 500, 150);
		assertTrue(pbTest != null);
	}

}
