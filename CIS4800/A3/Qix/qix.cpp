// CIS*4800 Computer Graphics
// Andrew Crowell
// acrowell@uoguelph.ca
// 0545826
#include "qix.h"

int mainWindow;
int ticks;
//std::vector<Qix::Wall> walls;
Qix::Bouncy bouncy;
Qix::Player player;
Qix::Transformation sceneTransform;
Qix::Light light;
Qix::Map map;
Qix::Texture* frameTexture;

double keyAngles[4] = {
    M_PI / 2,
    M_PI,
    M_PI * 3 / 2,
    0,
};
bool keys[4];
bool space = false;
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

    /*for(size_t i = 0; i < walls.size(); i++)
    {
        walls[i].render();
    }*/
    map.render();
    bouncy.render();
    player.render();
	glPopMatrix();

	glutSwapBuffers();
}

void fill()
{

    map.replaceTile(Qix::Map::UnfinishedLine, Qix::Map::Edge);

    Qix::Vector2 v = bouncy.getMapPosition();
    int x, y;
    
    int bouncyX = (int) v.data[Qix::Vector2::X];
    int bouncyY = (int) v.data[Qix::Vector2::Y];
    
    x = player.wallStartX - 1;
    y = player.wallStartY;
    
    //printf("Arf %d %d vs. %d %d.\n", x, y, bouncyX, bouncyY);
    
    if(map.getTile(x, y) == Qix::Map::Empty)
    {
        printf("Empty\n");
        map.store();
        if(map.fill(x, y, bouncyX, bouncyY))
        {
            return;
        }
        map.recall();        
    }
    
    x = player.wallStartX + 1;
    y = player.wallStartY;
    //printf("Arf2 %d %d vs. %d %d.\n", x, y, bouncyX, bouncyY);
    
    if(map.getTile(x, y) == Qix::Map::Empty)
    {
        printf("Empty\n");
        map.store();
        if(map.fill(x, y, bouncyX, bouncyY))
        {
            return;
        }
        map.recall();   
    }
    
    x = player.wallStartX;
    y = player.wallStartY - 1;
    //printf("Arf3 %d %d vs. %d %d.\n", x, y, bouncyX, bouncyY);
    
    if(map.getTile(x, y) == Qix::Map::Empty)
    {
        //printf("Empty\n");
        map.store();
        if(map.fill(x, y, bouncyX, bouncyY))
        {
            return;
        }
        map.recall();   
    }
    
    x = player.wallStartX;
    y = player.wallStartY + 1;
    //printf("Arf4 %d %d vs. %d %d.\n", x, y, bouncyX, bouncyY);  
    
    if(map.getTile(x, y) == Qix::Map::Empty)
    {
        //printf("Empty\n");
        map.store();
        if(map.fill(x, y, bouncyX, bouncyY))
        {
            return;
        }
        map.recall();   
    }
    
}

void animate(int value)
{
	ticks++;
    bouncy.update(map.walls);
    player.moving = false;
    for(int i = 0; i < 4; i++)
    {
        if(keys[i])
        {
            player.angle = keyAngles[i];
            player.moving = true;
        }
    }
    player.building = space;
    player.update(map);
    if(player.fillMe)
    {
        fill();
        player.fillMe = false;
    }
	//sceneTransform.rotation.data[0] = sin(ticks * 0.01) * 10;
    //sceneTransform.rotation.data[0]-= 0.7;
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
//	walls.clear();
	exit(0);
}

void keyboard(unsigned char k, int x, int y)
{
    switch(k)
    {
        case 27:
            quit();
            break;
        case 'w':
        case 'W':
            keys[0] = true;
            break;            
        case 'a':
        case 'A':
            keys[1] = true;
            break;
        case 's':
        case 'S':
            keys[2] = true;
            break;
        case 'd':
        case 'D':
            keys[3] = true;
            break;
        case ' ':
            space = true;
            break;
    }
}

void release(unsigned char k, int x, int y)
{
    switch(k)
    {
        case 27:
            quit();
            break;
        case 'w':
        case 'W':
            keys[0] = false;
            break;            
        case 'a':
        case 'A':
            keys[1] = false;
            break;
        case 's':
        case 'S':
            keys[2] = false;
            break;
        case 'd':
        case 'D':
            keys[3] = false;
            break;
        case ' ':
            space = false;
            break;             
    }
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
    player.init();
    map.init(frameTexture);
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
    memset(keys, 0, sizeof(keys));

	glutInit(argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	mainWindow = glutCreateWindow("Qix");

	const Qix::Color FrameColor = 0xff999999;
	const Qix::Color BouncyColor = 0xffff0000;
    
    
    frameTexture = Qix::Texture::loadImage("grasstiles.dat");

	sceneTransform = Qix::Transformation(
		Qix::Vector3(0, 0, -2),
		Qix::Vector3(0.1, 0.1, 0.1),
		Qix::Vector3(0, 0, 0)
	);

	light = Qix::Light(Qix::Vector4(6, 8, 0, 4));
	light.ambient = 0xff666666;

    map.init(frameTexture);
	bouncy = Qix::Bouncy(Qix::Sphere(
			Qix::Transformation(
				Qix::Vector3(0, 0, 0),
				Qix::Vector3(0.2, 0.2, 0.2),
				Qix::Vector3(0, 0, 0)
			),
			BouncyColor
		)
	);
    bouncy.angle = (double) rand() / ((double) RAND_MAX + 1.0) * M_PI * 2;
    
	player.init();
    
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
	

    glutIgnoreKeyRepeat(true);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(release);
    
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