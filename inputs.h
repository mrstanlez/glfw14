// Filename: inputs.h
// Action: read input from mouse, keyboard

bool mouseRightButton = false;
bool mouseMiddleButton = false;
bool mouseLeftButton = false;

struct Vec pos_old;
struct VecDouble pos_start;
//----- FUNCTIONS ------------------------------------------------------

void key(GLFWwindow * window, int key, int scancode, int action,
	int mods);
void mouseButton(GLFWwindow* window, int button, int action, int mods);
void mousePosition(GLFWwindow* window, double xpos, double ypos);
//----------------------------------------------------------------------

void key(GLFWwindow * window, int key, int scancode, int action,
	int mods){
	if(action == GLFW_PRESS) {
		if (key == GLFW_KEY_ESCAPE) {   
			glfwSetWindowShouldClose(window, GL_TRUE);
		} else if (key == GLFW_KEY_P) {  
			g_perspective = true;
		} else if (key == GLFW_KEY_O) {  
			g_perspective = false;
		} else if (key == GLFW_KEY_1) {  
			rot.x = 0;
			rot.y = 0;
			rot.z = 0;
			g_view = "FRONT";
			g_perspective = false;
		} else if (key == GLFW_KEY_2) {
			rot.x = 0;
			rot.y = 0;
			rot.z = 90;
			g_view = "LEFT";
			g_perspective = false;
		} else if (key == GLFW_KEY_3) {   
			rot.x = 0;
			rot.y = 90;
			rot.z = 0;
			g_view = "TOP";
			g_perspective = false;
		} else if (key == GLFW_KEY_4) {   
			rot.x = 0;
			rot.y = 0;
			rot.z = 0;
			g_view = "3D";
			g_perspective = true;
		} else if (key == GLFW_KEY_D) {   
			g_position = true;
			g_state = "POS";
		} else{
			g_position = false;
			g_state = "ROT";
		}
	}
}
//----------------------------------------------------------------------

void mouseButton(GLFWwindow* window, int button, int action, int mods){
	if (action == GLFW_PRESS) {
		if (button == GLFW_MOUSE_BUTTON_LEFT && g_position == true) {
			mouseLeftButton = true;
			pos_old.x = pos.x;
			pos_old.y = pos.y;			
		}else if (button == GLFW_MOUSE_BUTTON_LEFT &&
				g_position == false) {
			mouseLeftButton = true;
			pos_old.x = rot.z;
			pos_old.y = rot.y;			
		} else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
			mouseRightButton = true;
			pos_old.y = g_scale * 100;			
		} else if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
			mouseMiddleButton = true;
		}
		glfwGetCursorPos(window, &pos_start.x, &pos_start.y);
	} else {
		mouseLeftButton = mouseRightButton = mouseMiddleButton = false;
	}
}
//----------------------------------------------------------------------

void mousePosition(GLFWwindow* window, double xpos, double ypos){
    glfwGetCursorPos(window, &xpos, &ypos);
    if(mouseLeftButton && g_position == true){
		pos.x = pos_old.x - pos_start.x + xpos;
        pos.y = pos_old.y - pos_start.y + ypos;
	} else if (mouseLeftButton && g_position == false) {
        rot.z = pos_old.x - pos_start.x + xpos;
        rot.y = pos_old.y - pos_start.y + ypos;
	} else if (mouseRightButton) {
		g_scale = (pos_old.y - pos_start.y + ypos)/100.0f;        
	}
}
