/**
 * 
 */
package Test;

import java.awt.event.*;
import java.util.*;
import net.phys2d.math.*;
import tim.*;
import Widgets.*;
import junit.framework.TestCase;

/**
 * @author doshea
 *
 */
public class PlayfieldPanelTest extends TestCase {

	/**
	 * @param name
	 */
	public PlayfieldPanelTest(String name) {
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
	 * Test method for {@link tim.PlayfieldPanel#mouseReleased(MouseEvent)}.
	 */
	public final void testMouseReleased() {
		PlayfieldPanel testPanel = new PlayfieldPanel();
		BoxingGlove bg = new BoxingGlove();
		
		GameWindow.getInstance().setActiveWidget(bg);
		MouseEvent e = new MouseEvent(testPanel, 0, 0l, MouseEvent.SHIFT_DOWN_MASK, 100, 100, 1, false, MouseEvent.BUTTON1);
		testPanel.mouseReleased(e);

		assertTrue(GameWindow.getInstance().getActiveWidget() == bg);
	}

}
