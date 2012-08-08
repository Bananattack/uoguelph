// CIS*4800 Computer Graphics
// Andrew Crowell
// acrowell@uoguelph.ca
// 0545826
#include "a5.h"

int mainWindow;
int ticks = 0;
GLUI_StaticText* errorMessageLabel;
GLUI* glui;
Polygon* activePolygon;
Transformation sceneTransform;
Light light;

std::string polyFilename = "ply/beethoven.ply";
Color polyColor = 0xff999999;
int polyLightW = 1;


void refreshLight()
{
	light.load(GL_LIGHT0);

	// Enable lighting.
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}

void reshape(int width, int height)
{
	glViewport(0, 0, (GLsizei) width, (GLsizei) height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (GLfloat) width / (GLfloat) height, 1.0, 1000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glShadeModel(GL_SMOOTH);
	glMaterialf(GL_FRONT, GL_SHININESS, 30);
	
	glEnable(GL_NORMALIZE);

	if(activePolygon)
	{
		glPushMatrix();
		sceneTransform.applyToRender();
		refreshLight();
		activePolygon->draw(light);

		glDisable(GL_LIGHTING);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		
		const float LightSourceVertices[] = {
			-1, 0, 0,
			0, -1, 0,
			1, 0, 0,
			0, 1, 0,
		};
		const float LightSourceColors[] = {
			1, 0, 0,
			1, 1, 0,
			0, 1, 0,
			0, 1, 1,
		};

		glPushMatrix();
		//activePolygon->specularOn.applyToRender();
		light.diffuse.applyToRender();
		glEnableClientState(GL_VERTEX_ARRAY);

		float x, y, z;
		x = light.position.data[Vector4::X];
		y = light.position.data[Vector4::Y];
		z = light.position.data[Vector4::Z];

		glTranslatef(-x, -y, -z);
		//glScalef(0.6, 0.4, 0.6);
		glRotatef(atan2(z, x) * 180 / M_PI + 90, 0, 1, 0);
		glVertexPointer(3, GL_FLOAT, 0, LightSourceVertices);
		
		glutSolidSphere(0.5, 30, 30);
		//glDrawArrays(GL_QUADS, 0, 4);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glPointSize(5.0);
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(3, GL_FLOAT, 0, LightSourceColors);
		glDrawArrays(GL_POINTS, 0, 4);
		glPointSize(1.0);
		
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		glEnable(GL_LIGHTING);
		
		glPopMatrix();
		glPopMatrix();
	}
	glutSwapBuffers();
}

void animate(int value)
{
	ticks++;
	if(activePolygon)
	{
        //sceneTransform.rotation.data[1] = sin(ticks * 0.01) * 10;
        light.position.data[Vector4::X] = sin(ticks * 0.01) * 5;
        light.position.data[Vector4::Y] = activePolygon->minY - 5;
        light.position.data[Vector4::Z] = cos(ticks * 0.01) * 5;
        light.position.data[Vector4::W] = polyLightW;
    }
	glutTimerFunc(10, animate, 1);     
}


void idle()
{
	if(glutGetWindow() != mainWindow)
	{
		glutSetWindow(mainWindow);
	}
	glutPostRedisplay();
}

void exitButtonCallback(int i)
{
	exit(-1);
}

void keyboard(unsigned char key, int x, int y)
{
    switch(key)
    {
        case '-':
		case 'z':
			if(sceneTransform.scale.data[Vector3::X] - 0.08 > 0.25)
			{
				sceneTransform.scale.subtract(Vector3(0.08, 0.08, 0.08));
			}
            break;
        case '=':
		case 'x':
            sceneTransform.scale.add(Vector3(0.08, 0.08, 0.08));
            break;  
        case 27:
            exit(0);
    }
}

void keyboardSpecial(int key, int x, int y)
{
	switch(key)
	{
		case GLUT_KEY_UP:
			//if(sceneTransform.rotation.data[0] - 2 > 0)
			{
				sceneTransform.rotation.data[0] -= 2;
			}
			break;
		case GLUT_KEY_DOWN:
			//if(sceneTransform.rotation.data[0] - 2 < 180)
			{
				sceneTransform.rotation.data[0] += 2;
			}
			break;
		case GLUT_KEY_LEFT:
			sceneTransform.rotation.data[1] -= 2;
			break;
		case GLUT_KEY_RIGHT:
			sceneTransform.rotation.data[1] += 2;
			break; 
	}
}

bool mouseDown = false;
int mouseButton = 0;
int mouseStartX, mouseStartY;
void mouse(int button, int state, int x, int y)
{
	if(button == GLUT_RIGHT_BUTTON)
	{
		mouseDown = state == GLUT_DOWN;
		mouseButton = button;
		mouseStartX = x;
		mouseStartY = y;
	}
}

void mouseMotion(int x, int y)
{
	if(mouseDown)
	{
		if(mouseButton == GLUT_RIGHT_BUTTON)
		{
			sceneTransform.rotation.data[1] += (mouseStartX - x) / 2.0;

			double destAngle = sceneTransform.rotation.data[0] - (mouseStartY - y) / 2.0;
			//if(destAngle > 0 && destAngle < 180)
			{
				sceneTransform.rotation.data[0] = destAngle;
			}
		}
		mouseStartX = x;
		mouseStartY = y;
	}
}

void refreshGeometry(int i)
{
	if(activePolygon)
	{
		activePolygon->refreshGeometry();
	}
}

void refreshColors(int i)
{
	if(activePolygon)
	{
		activePolygon->fillColor = polyColor;
		activePolygon->refreshColors();
	}
}

GLUI_StaticText* createStaticText(GLUI_Panel* pan, const char* label)
{
	GLUI_StaticText* st = new GLUI_StaticText(pan, label);
    return st;
}

GLUI_EditText* createTextField(GLUI_Panel* pan, const char* label, std::string& pointer, void(*callback)(int))
{
	GLUI_EditText* spinner = new GLUI_EditText(pan, label, pointer, -1, callback);
    return spinner;
}

GLUI_Spinner* createFloatSpinner(GLUI_Panel* pan, const char* label, float* pointer, void(*callback)(int))
{
	GLUI_Spinner* spinner = new GLUI_Spinner(pan, label, pointer, -1, callback);
    return spinner;
}

GLUI_Checkbox* createCheckbox(GLUI_Panel* pan, const char* label, int* pointer, void(*callback)(int))
{
	GLUI_Checkbox* cb = new GLUI_Checkbox(pan, label, pointer, -1, callback);
    return cb;
}

GLUI_RadioGroup* createRadioGroup(GLUI_Panel* pan, int* pointer, void(*callback)(int))
{
	GLUI_RadioGroup* cb = new GLUI_RadioGroup(pan, pointer, -1, callback);
    return cb;
}

GLUI_RadioButton* createRadioButton(GLUI_RadioGroup* group, const char *name)
{
	GLUI_RadioButton* cb = new GLUI_RadioButton(group, name);
    return cb;
}

GLUI_Button* createExitButton(GLUI* g, const char* label)
{
	GLUI_Button* button = new GLUI_Button(g, label, -1, exitButtonCallback); 
    return button;
}

void loadPolygon(int i)
{
    Polygon* poly = Polygon::loadPolygon(polyFilename);
	
	delete activePolygon;
	if(poly)
	{
		activePolygon = poly;
		errorMessageLabel->set_text("");
		sceneTransform.translation.data[2] = -MAX(poly->width, poly->height) * 2 - poly->depth * 2;
		sceneTransform.scale.setData(1, 1, 1);
		sceneTransform.rotation.setData(0, 0, 0);
	}
	else
	{
		activePolygon = NULL;
		errorMessageLabel->set_text("*** ERROR: file cannot be opened.");
	}
	refreshGeometry(i);
    refreshColors(i);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	mainWindow = glutCreateWindow("Heightmaps");

    
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
	sceneTransform = Transformation(
		Vector3(0, 0, 0),
		Vector3(1, 1, 1),
		Vector3(0, 0, 0)
	);
	light = Light(Vector4(0, 0, 0, 0));
	refreshLight();    
    
	// Enable depth checking.
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

    glutIdleFunc(idle);
	glutTimerFunc(10, animate, 1);
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(keyboardSpecial);
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMotion);

	glui = GLUI_Master.create_glui("Controls", 0, 900, 50);
    GLUI_Panel* mainPan = new GLUI_Panel(glui, "", GLUI_PANEL_NONE);
    
    
    GLUI_Panel* pan;
    createStaticText(mainPan, "Fake Shadow Rendering Utility");
    createStaticText(mainPan, "by Andrew Crowell");
    createStaticText(mainPan, "for CIS*4800 A5, 2010");
    createStaticText(mainPan, "");
    createStaticText(mainPan, "To rotate, Right Click and drag or use Arrow Keys.");
    createStaticText(mainPan, "To zoom, use -/+ or Z/X");
    pan = new GLUI_Panel(mainPan, "File");
    pan->set_w(500);
    
    GLUI_EditText* et = createTextField(pan, "Filename", polyFilename, loadPolygon);
    et->set_w(200);
    pan = new GLUI_Panel(mainPan, "Color");
    pan->set_w(500);
	createFloatSpinner(pan, "R", &polyColor.data[Color::RedChannel], refreshColors)->set_float_limits(0, 1);
	createFloatSpinner(pan, "G", &polyColor.data[Color::GreenChannel], refreshColors)->set_float_limits(0, 1);
	createFloatSpinner(pan, "B", &polyColor.data[Color::BlueChannel], refreshColors)->set_float_limits(0, 1);
	pan = new GLUI_Panel(mainPan, "Light");
	pan->set_w(500);
	createFloatSpinner(pan, "R", &light.diffuse.data[Color::RedChannel], refreshColors)->set_float_limits(0, 1);
	createFloatSpinner(pan, "G", &light.diffuse.data[Color::GreenChannel], refreshColors)->set_float_limits(0, 1);
	createFloatSpinner(pan, "B", &light.diffuse.data[Color::BlueChannel], refreshColors)->set_float_limits(0, 1);
	GLUI_RadioGroup* group = createRadioGroup(pan, &polyLightW, NULL);
	createRadioButton(group, "Directional / Infinite (w = 0)");
	createRadioButton(group, "Positional / Local (w = 1)");
	//createCheckbox(pan, "Positional (on: w = 1, off: w = 0)", &polyLightW, NULL);
	
    pan = new GLUI_Panel(mainPan, "Message");
    pan->set_w(500);
    errorMessageLabel = createStaticText(pan, "                                                ");
        
    createExitButton(glui, "Exit");
    
	glui->set_main_gfx_window(mainWindow);
	GLUI_Master.set_glutIdleFunc(idle); 

	loadPolygon(0);
	glutMainLoop();
	
    return 0;
}