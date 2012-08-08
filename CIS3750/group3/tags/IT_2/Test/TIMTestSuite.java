package Test;

import junit.framework.Test;
import junit.framework.TestSuite; 


public class TIMTestSuite { 


     public static Test suite() { 
          TestSuite suite = new TestSuite("TIM Tests");
          suite.addTestSuite(HelpGUITest.class);
          suite.addTestSuite(WidgetPanelButtonTest.class);
          suite.addTestSuite(GameMenuBarTest.class);
          suite.addTestSuite(GameSettingsChangerTest.class);
          suite.addTestSuite(PromptBoxTest.class);
          
          
          

          return suite; 
     }

}
