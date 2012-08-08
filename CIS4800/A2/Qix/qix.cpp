// CIS*4800 Computer Graphics
// Andrew Crowell
// acrowell@uoguelph.ca
// 0545826
#include "qix.h"

int mainWindow;
int ticks;
std::vector<Qix::Wall> walls;
Qix::Bouncy bouncy;
Qix::Transformation sceneTransform;
Qix::Light light;
int ballAngle = -1;

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
	gluPerspective(45.0, (GLfloat) width / (GLfloat) height, 1.0, 10.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glShadeModel(GL_SMOOTH);
	glMaterialf(GL_FRONT, GL_SHININESS, 30);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glPushMatrix();
	glEnable(GL_NORMALIZE);
	sceneTransform.applyToRender();

    for(size_t i = 0; i < walls.size(); i++)
    {
        walls[i].render();
    }
    bouncy.render();
	glPopMatrix();

	glutSwapBuffers();
}

void animate(int value)
{
	ticks++;
    bouncy.update(walls);
//	sceneTransform.rotation.data[1] = sin(ticks * 0.01) * 10;
	glutTimerFunc(10, animate, 1); 
}

void idle()
{
	if(glutGetWindow() != mainWindow)
	{
		glutSetWindow(mainWindow);
	}
	prepareScene();
    
	glutPostRedisplay();
}

void quit()
{
	walls.clear();
	exit(0);
}

void resetButtonCallback(int i)
{
    if(ballAngle == -1)
    {
        bouncy.angle = (double) rand() / ((double) RAND_MAX + 1.0) * M_PI * 2;
    }
    else
    {
        bouncy.angle = ballAngle * M_PI / 180;
    }
    bouncy.shape.transformation().translation = Qix::Vector3(0, 0, 0);
    bouncy.bounceCount = 0;
}

void exitButtonCallback(int i)
{
	quit();
}

GLUI_Spinner* createColorSpinner(GLUI_Panel* pan, const char* label, float* pointer)
{
	GLUI_Spinner* spinner = new GLUI_Spinner(pan, label, pointer);
	spinner->set_float_limits(0, 1);
    return spinner;
}

GLUI_Button* createExitButton(GLUI_Panel* g, const char* label)
{
	GLUI_Button* button = new GLUI_Button(g, label, -1, exitButtonCallback);
    return button;
}

GLUI_Button* createResetButton(GLUI_Panel* g, const char* label)
{
	GLUI_Button* button = new GLUI_Button(g, label, -1, resetButtonCallback);
    return button;
}

GLUI_EditText* createAngleSelector(GLUI_Panel* pan, const char* label, int* pointer)
{
	GLUI_EditText* spinner = new GLUI_EditText(pan, label, pointer);
	spinner->set_int_limits(-1, 360);
    return spinner;
}

void init(int* argc, char** argv)
{
	glutInit(argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	mainWindow = glutCreateWindow("Qix");

	const Qix::Color FrameColor = 0xff999999;
	const Qix::Color PlayerColor = 0xffff0000;
    
    

	sceneTransform = Qix::Transformation(
		Qix::Vector3(0, 0, -2),
		Qix::Vector3(0.1, 0.1, 0.1),
		Qix::Vector3(0, 0, 0)
	);

	light = Qix::Light(Qix::Vector4(6, 8, 0, 0));
	light.ambient = 0xff333333;

	walls.push_back(Qix::Wall(Qix::Cube(
			Qix::Transformation(
				Qix::Vector3(0, -6, 0),
				Qix::Vector3(18, 1, 1),
				Qix::Vector3(0, 0, 0)
			),
			FrameColor
		)
	));
	walls.push_back(Qix::Wall(Qix::Cube(
			Qix::Transformation(
				Qix::Vector3(-8.5, 0, 0),
				Qix::Vector3(1, 11, 1),
				Qix::Vector3(0, 0, 0)
			),
			FrameColor
		)
	));
	walls.push_back(Qix::Wall(Qix::Cube(
			Qix::Transformation(
				Qix::Vector3(8.5, 0, 0),
				Qix::Vector3(1, 11, 1),
				Qix::Vector3(0, 0, 0)
			),
			FrameColor
		)
	));
	walls.push_back(Qix::Wall(Qix::Cube(
			Qix::Transformation(
				Qix::Vector3(0, 6, 0),
				Qix::Vector3(18, 1, 1),
				Qix::Vector3(0, 0, 0)
			),
			FrameColor
		)
	));
	bouncy = Qix::Bouncy(Qix::Sphere(
			Qix::Transformation(
				Qix::Vector3(0, 0, 0),
				Qix::Vector3(0.4, 0.4, 0.4),
				Qix::Vector3(0, 0, 0)
			),
			PlayerColor
		)
	);
    bouncy.angle = (double) rand() / ((double) RAND_MAX + 1.0) * M_PI * 2;

	prepareScene();

	// Enable depth checking.
	glEnable(GL_DEPTH_TEST);

	//glutIdleFunc(idle);
	glutTimerFunc(10, animate, 1);
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);

	GLUI* glui = GLUI_Master.create_glui("Controller", 0, 800, 50);
    GLUI_Panel* mainPan = new GLUI_Panel(glui, "Light Settings");
    
    GLUI_Panel* pan;
    

    pan = new GLUI_Panel(mainPan, "Ambient");
    new GLUI_Column(mainPan, false);
	createColorSpinner(pan, "R", &(light.ambient.data[Qix::Color::RedChannel]));
	createColorSpinner(pan, "G", &(light.ambient.data[Qix::Color::GreenChannel]));
	createColorSpinner(pan, "B", &(light.ambient.data[Qix::Color::BlueChannel]));
    
    pan = new GLUI_Panel(mainPan, "Diffuse");
    new GLUI_Column(mainPan, false);
	createColorSpinner(pan, "R", &(light.diffuse.data[Qix::Color::RedChannel]));
	createColorSpinner(pan, "G", &(light.diffuse.data[Qix::Color::GreenChannel]));
	createColorSpinner(pan, "B", &(light.diffuse.data[Qix::Color::BlueChannel]));    
    
    pan = new GLUI_Panel(mainPan, "Specular");
    new GLUI_Column(mainPan, false);
	createColorSpinner(pan, "R", &(light.specular.data[Qix::Color::RedChannel]));
	createColorSpinner(pan, "G", &(light.specular.data[Qix::Color::GreenChannel]));
	createColorSpinner(pan, "B", &(light.specular.data[Qix::Color::BlueChannel]));
    
    mainPan = new GLUI_Panel(glui, "", GLUI_PANEL_NONE);
    pan = new GLUI_Panel(mainPan, "Sphere Settings");
    pan->set_alignment(GLUI_ALIGN_LEFT);
    mainPan->set_w(500);
    new GLUI_Column(mainPan, false);
    createAngleSelector(pan, "Initial Angle", &ballAngle);
    createResetButton(pan, "Reset");

    pan = new GLUI_Panel(mainPan, "", GLUI_PANEL_NONE);
    new GLUI_StaticText(pan, "                                                ");
    new GLUI_Column(mainPan, false);

    pan = new GLUI_Panel(mainPan, "Other");
    new GLUI_Column(mainPan, false);
    pan->set_alignment(GLUI_ALIGN_RIGHT);
    createExitButton(pan, "Exit");
	

	glui->set_main_gfx_window(mainWindow);
	GLUI_Master.set_glutIdleFunc(idle); 

	glutMainLoop();
}

int main(int argc, char** argv)
{
	init(&argc, argv);
	quit();
	
	return 0;
}