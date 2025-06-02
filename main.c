/* Linux, build: gcc "%f" -o "%e" -Wall -Wextra -lGL -lglfw -lm
   download: glfw 3.4
			 gcc 14
			 geany 2.0

   Windows, build: gcc "%f" -o "%e" -Wall -Wextra -I
	C:\glfw-3.4.bin.WIN64\include -L C:\glfw-3.4.bin.WIN64\lib-mingw-w64
	-lglfw3dll -lopengl32
			run in terminal:  ./main-test.exe
   copy glfw3.dll from C:\glfw-3.4.bin.WIN64\lib-mingw-w64 to
		C:\Windows\System32
   download: glfw: glfw-3.4.bin.WIN64
			 gcc:  x86_64-15.1.0-release-win32-seh-ucrt-rt_v12-rev0
			 Setup your windows Path: in extended setup system/Variable
				environments/System variables then select item: Path 
				and click on button Change button Add: C:\mingw64\bin
			 IDE:  geany-2.0_setup
*/
#include <GLFW/glfw3.h>

// Custom files
#include "objects.h" // Object 2D & 3D
#include "inputs.h" // Mouse & keyboard inputs

float g_aspect;
double lastTime = 0.0;
int nbFrames = 0;
char fpsCounter[20];
//----- FUNCTIONS ------------------------------------------------------

void error(int error, const char * description);
void perspectiveGL( GLdouble fovY, GLdouble aspect, GLdouble zNear,
					GLdouble zFar );
void display(GLFWwindow * window);
int main();
//----------------------------------------------------------------------

void error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s (%d)\n", description, error);
}
//----------------------------------------------------------------------

void init(){	
	loadChars("data/chars.txt");
	
	//file_Obj_Convert_To_Model("Trafo");
	//loadModel("Cube");
	//loadModel("Trafo");
	loadModel("Grass");
	
	//fileLoadObj("Trafo");	
	//fileLoadObj("Grass");
	
	calcGrass(140, 20, 12, 0.7);
}
//----------------------------------------------------------------------

void perspectiveGL(GLdouble fovY, GLdouble aspect, GLdouble zNear, 
	GLdouble zFar){	
	GLdouble fW, fH;
	fH = tan( fovY / 360 * PI) * zNear;
	fW = fH * aspect;
	
	glFrustum(-fW, fW, -fH, fH, zNear, zFar);
}
//----------------------------------------------------------------------

void display(GLFWwindow * window){
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	glClearColor(0.6, 0.6, 0.6, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	
	//glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);  
	
	// 3D view
	glEnable(GL_DEPTH_TEST);

	if(g_perspective) {	
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		perspectiveGL(45.0, (float)width / height, 0.1, 100.0);	
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();	
	} else {
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();		
		g_aspect = (float)width / height;
		glOrtho(-2.04f * g_aspect, 2.04f * g_aspect, -2.04f, 2.04f, 
			100.0, -100.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}	
	
	glTranslatef(0.0, -0.5,-5.0);
	glTranslatef(pos.x/100.0f, -pos.y/100.0f, pos.z/100.0f);
	glRotatef(rot.x, 0.0, 0.0, 1.0);
	glRotatef(rot.y, 1.0, 0.0, 0.0);
	glRotatef(rot.z, 0.0, 1.0, 0.0);
	//glRotatef((float)glfwGetTime() * 10.0, 0.0, 1.0, 0.0);
	glScalef(g_scale, g_scale, g_scale);
	
	// Draw List	
	drawAxes(10.0, 2);
	drawGrid(10.0, 0.1);
	//drawModel(3);
	//objmtl(3, 0.0,1.43,0.0, 0.0,90.0, 0.0, 0.71); // Trafo
		
	genGrass(140, 20, 12, 0.3, 0.0, 0.0, 0.0, 1.0);
	
	// Draw 2D List
	glDisable(GL_DEPTH_TEST);
	glViewport(0.0f, 0.0f, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();		
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glTranslatef(-1.0, -1.0, 0.0);
	
	//drawGrid2D(2.0, 0.1);		
	//genGrass(140, 15, 1.0, 1.0, 0.0, 0.5);
	
	glColor4f(0, 0, 1, 1);
	drawText("MOUSE LEFT+MX/MY=ROT,POS, RIGHT+MY=SCALE,P-PER,O-ORTHO",
		2, 1.01,1.95,0, 0,0,0, 0.03f);
	drawText("VIEW(1 FRONT, 2 LEFT, 3 TOP, 4 3D) R ROT, D POS",
		2, 1.01,1.9,0, 0,0,0, 0.03f);
	glColor4f(0,1,0,1);
	drawText(g_view, 2, 0.01,1.94,0, 0,0,0, 0.04f);
	drawText(g_state, 2, 0.01,1.88,0, 0,0,0, 0.04f);
	
	// FPS counter
	double currentTime = glfwGetTime();
	nbFrames++;
	if (currentTime - lastTime >= 1.0){
		sprintf(fpsCounter, "FPS:%d", nbFrames);		
		nbFrames = 0;
		lastTime +=1.0;
	}
	drawText(fpsCounter, 2, 0.85,1.95,0, 0,0,0, 0.03f);	
}
//----------------------------------------------------------------------

int main(){
	srand(time(0));
	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
		exit(1);
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    
	GLFWwindow * window = glfwCreateWindow(g_windowwidth,
		g_windowheight, "GLFW", NULL, NULL);

	if (!window){
		glfwTerminate();
		exit(1);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	
	init();
	
	// transparency of image
	glEnable(GL_BLEND);
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	// Hardware inputs
	glfwSetKeyCallback(window, key);
	glfwSetCursorPosCallback(window, mousePosition);
	glfwSetMouseButtonCallback(window, mouseButton);
            
	while (!glfwWindowShouldClose(window)){		
		display(window);
		glfwSwapBuffers(window);
		glfwPollEvents();		
	}

	textureUnbind();
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
