/**
 * 
 */
package Test;

import tim.*;
import Widgets.*;
import junit.framework.TestCase;

/**
 * @author doshea
 *
 */
public class WidgetPanelButtonTest extends TestCase {

	/**
	 * @param name
	 */
	public WidgetPanelButtonTest(String name) {
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
	 * Test method for {@link tim.WidgetPanelButton#WidgetPanelButton(Widgets.Widget, int)}.
	 */
	public final void testWidgetPanelButtonWidgetInt() {
		
		WidgetPanelButton testButton = new WidgetPanelButton(new BoxingGlove(), 1);
		assertTrue(testButton != null);
	}

	/**
	 * Test method for {@link tim.WidgetPanelButton#WidgetPanelButton(Widgets.Widget)}.
	 */
	public final void testWidgetPanelButtonWidget() {
		WidgetPanelButton testButton = new WidgetPanelButton(new BoxingGlove());
		assertTrue(testButton != null);
	}

	/**
	 * Test method for {@link tim.WidgetPanelButton#actionPerformed(java.awt.event.ActionEvent)}.
	 */
	public final void testActionPerformed() {
		BoxingGlove bg = new BoxingGlove();
		WidgetPanelButton testButton = new WidgetPanelButton(bg, 1);
		testButton.actionPerformed(null);
		assertTrue(bg == GameWindow.getInstance().getActiveWidget());
	}

}
