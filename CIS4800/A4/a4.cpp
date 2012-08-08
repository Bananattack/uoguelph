// CIS*4800 Computer Graphics
// Andrew Crowell
// acrowell@uoguelph.ca
// 0545826
#include "a4.h"

int mainWindow;
int ticks = 0;
GLUI_StaticText* errorMessageLabel;
GLUI* glui;
Heightmap* map;
Transformation sceneTransform;
Light light;

std::string mapFilename = "greyTextures/mount4.pgm";
Color mapColor = 0xff339911;
int mapRandomize = 1;
float mapScaleZ = Heightmap::SCALE_Z_INITIAL;
float rotationAngle = 0;


void prepareScene()
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

	glPushMatrix();
	glEnable(GL_NORMALIZE);
	sceneTransform.applyToRender();

	if(map)
	{
		map->draw();
	}
	glPopMatrix();

	glutSwapBuffers();
}

void animate(int value)
{
	//ticks++;
	//sceneTransform.rotation.data[1] = sin(ticks * 0.01) * 10;
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
        case 'w':
            rotationAngle -= 1;
            break;
        case 'a':
            rotationAngle += 1;
            break;
        case 27:
            exit(0);
    }
    sceneTransform.rotation.data[2] = rotationAngle;
}

void refreshGeometry(int i)
{
	if(map)
	{
		map->ambient = mapColor;
		map->randomize = mapRandomize != 0;
		map->scaleZ = mapScaleZ;
		map->refreshGeometry();
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

GLUI_Button* createExitButton(GLUI* g, const char* label)
{
	GLUI_Button* button = new GLUI_Button(g, label, -1, exitButtonCallback); 
    return button;
}

#define min(a, b) (((a) < (b)) ? (a) : (b))
void loadMap(int i)
{
	Heightmap* hm = Heightmap::loadPGM(mapFilename);
	
	delete map;
	if(hm)
	{
		map = hm;
		errorMessageLabel->set_text("");
		sceneTransform.translation.data[2] = -min(hm->width, hm->height) / 2;
	}
	else
	{
		map = NULL;
		errorMessageLabel->set_text("*** ERROR: file cannot be opened.");
	}
	refreshGeometry(i);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	mainWindow = glutCreateWindow("Heightmaps");

    

    /*std::cout << "P2" << std::endl;
    std::cout << map->width << std::endl;
    std::cout << map->height << std::endl;
    std::cout << map->depth << std::endl;
    for(int i = 0; i < map->width * map->height; i++)
    {
        std::cout << map->data[i] << std::endl;
    }*/
    
	sceneTransform = Transformation(
		Vector3(0, 0, -100),
		Vector3(0.3, 0.3, 0.3),
		Vector3(0, 0, 0)
	);
	light = Light(Vector4(10, 10, -10, 0));
	prepareScene();    
	sceneTransform.rotation.data[0] = -35;
    
	// Enable depth checking.
	glEnable(GL_DEPTH_TEST);

    glutIdleFunc(idle);
	glutTimerFunc(10, animate, 1);
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);

	glui = GLUI_Master.create_glui("Heightmap Controls", 0, 800, 50);
    GLUI_Panel* mainPan = new GLUI_Panel(glui, "", GLUI_PANEL_NONE);
    
    
    GLUI_Panel* pan;
    createStaticText(mainPan, "Heightmap Utility");
    createStaticText(mainPan, "by Andrew Crowell");
    createStaticText(mainPan, "for CIS*4800 A4, 2010");
    createStaticText(mainPan, "");
    createStaticText(mainPan, "The W and A keys rotate the map.");
    pan = new GLUI_Panel(mainPan, "Heightmap");
    
    createTextField(pan, "Filename", mapFilename, loadMap);
	createFloatSpinner(pan, "R", &mapColor.data[Color::RedChannel], refreshGeometry)->set_float_limits(0, 1);
	createFloatSpinner(pan, "G", &mapColor.data[Color::GreenChannel], refreshGeometry)->set_float_limits(0, 1);
	createFloatSpinner(pan, "B", &mapColor.data[Color::BlueChannel], refreshGeometry)->set_float_limits(0, 1);
	createFloatSpinner(pan, "Z Scale", &mapScaleZ, refreshGeometry);
	createCheckbox(pan, "Randomize", &mapRandomize, refreshGeometry);
	
    pan = new GLUI_Panel(mainPan, "Message");
    errorMessageLabel = createStaticText(pan, "                                                ");
        
    createExitButton(glui, "Exit");
    
	glui->set_main_gfx_window(mainWindow);
	GLUI_Master.set_glutIdleFunc(idle); 

	loadMap(0);
	glutMainLoop();
	
    return 0;
}