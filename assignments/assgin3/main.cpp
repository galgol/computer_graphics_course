#include <Windows.h>
#include <iostream>
#include "display.h"
#include "mesh.h"
#include "shader.h"
#include "inputManager.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include "GLFW\glfw3.h"

using namespace glm;
static const int NUM_OF_CUBES = 5;
int picked = -1;
float prevX;
float prevY;
float tmpPrevX;
float tmpPrevY;
static const int DISPLAY_WIDTH = 800;
static const int DISPLAY_HEIGHT = 800;
float transX;
float transY;
mat4 transUpdate = mat4(1);
int transUpdateIDX = -2;
mat4 M = mat4(1);
Display display1(DISPLAY_WIDTH, DISPLAY_HEIGHT, "OpenGL");
Shader shader("./res/shaders/basicShader");
Shader shader1("./res/shaders/test");
vec3 pos = vec3(0, 0, -30);
vec3 forward = glm::vec3(0.0f, 0.0f, 1.0f);
vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
mat4 P = glm::perspective(60.0f, (float)DISPLAY_WIDTH / (float)DISPLAY_HEIGHT, 0.1f, 100.0f)* glm::lookAt(pos, pos + forward, up);
mat4 TR[NUM_OF_CUBES + 1][5];
mat4 MVP = P*M;
bool IK;
int countIK = 0;



Vertex vertices[] =
{
	Vertex(glm::vec3(-1, -1, -1), glm::vec2(1, 0), glm::vec3(0, 0, -1),glm::vec3(0, 0, 1)),
	Vertex(glm::vec3(-1, 1, -1), glm::vec2(0, 0), glm::vec3(0, 0, -1),glm::vec3(0, 0, 1)),
	Vertex(glm::vec3(1, 1, -1), glm::vec2(0, 1), glm::vec3(0, 0, -1),glm::vec3(0, 0, 1)),
	Vertex(glm::vec3(1, -1, -1), glm::vec2(1, 1), glm::vec3(0, 0, -1),glm::vec3(0, 0, 1)),

	Vertex(glm::vec3(-1, -1, 1), glm::vec2(1, 0), glm::vec3(0, 0, 1),glm::vec3(0, 0, 1)),
	Vertex(glm::vec3(-1, 1, 1), glm::vec2(0, 0), glm::vec3(0, 0, 1),glm::vec3(0, 0, 1)),
	Vertex(glm::vec3(1, 1, 1), glm::vec2(0, 1), glm::vec3(0, 0, 1),glm::vec3(0, 0, 1)),
	Vertex(glm::vec3(1, -1, 1), glm::vec2(1, 1), glm::vec3(0, 0, 1),glm::vec3(0, 0, 1)),

	Vertex(glm::vec3(-1, -1, -1), glm::vec2(0, 1), glm::vec3(0, -1, 0),glm::vec3(0, 1, 0)),
	Vertex(glm::vec3(-1, -1, 1), glm::vec2(1, 1), glm::vec3(0, -1, 0),glm::vec3(0, 1, 0)),
	Vertex(glm::vec3(1, -1, 1), glm::vec2(1, 0), glm::vec3(0, -1, 0),glm::vec3(0, 1, 0)),
	Vertex(glm::vec3(1, -1, -1), glm::vec2(0, 0), glm::vec3(0, -1, 0),glm::vec3(0, 1, 0)),

	Vertex(glm::vec3(-1, 1, -1), glm::vec2(0, 1), glm::vec3(0, 1, 0),glm::vec3(0, 1, 0)),
	Vertex(glm::vec3(-1, 1, 1), glm::vec2(1, 1), glm::vec3(0, 1, 0),glm::vec3(0, 1, 0)),
	Vertex(glm::vec3(1, 1, 1), glm::vec2(1, 0), glm::vec3(0, 1, 0),glm::vec3(0, 1, 0)),
	Vertex(glm::vec3(1, 1, -1), glm::vec2(0, 0), glm::vec3(0, 1, 0),glm::vec3(0, 1, 0)),

	Vertex(glm::vec3(-1, -1, -1), glm::vec2(1, 1), glm::vec3(-1, 0, 0),glm::vec3(1, 0, 0)),
	Vertex(glm::vec3(-1, -1, 1), glm::vec2(1, 0), glm::vec3(-1, 0, 0),glm::vec3(1, 0, 0)),
	Vertex(glm::vec3(-1, 1, 1), glm::vec2(0, 0), glm::vec3(-1, 0, 0),glm::vec3(1, 0, 0)),
	Vertex(glm::vec3(-1, 1, -1), glm::vec2(0, 1), glm::vec3(-1, 0, 0),glm::vec3(1, 0, 0)),

	Vertex(glm::vec3(1, -1, -1), glm::vec2(1, 1), glm::vec3(1, 0, 0),glm::vec3(1, 0, 0)),
	Vertex(glm::vec3(1, -1, 1), glm::vec2(1, 0), glm::vec3(1, 0, 0),glm::vec3(1, 0, 0)),
	Vertex(glm::vec3(1, 1, 1), glm::vec2(0, 0), glm::vec3(1, 0, 0),glm::vec3(1, 0, 0)),
	Vertex(glm::vec3(1, 1, -1), glm::vec2(0, 1), glm::vec3(1, 0, 0),glm::vec3(1, 0, 0))
};

unsigned int indices[] = { 0, 1, 2,
0, 2, 3,

6, 5, 4,
7, 6, 4,

10, 9, 8,
11, 10, 8,

12, 13, 14,
12, 14, 15,

16, 17, 18,
16, 18, 19,

22, 21, 20,
23, 22, 20
};
Mesh mesh(vertices, sizeof(vertices) / sizeof(vertices[0]), indices, sizeof(indices) / sizeof(indices[0]));

void updateTRz(int k) {

		TR[0][3] = TR[0][0] * TR[0][1] * TR[0][4] * TR[0][2];
		for (int i = 1; i < k; i++) {
			TR[i][3] = TR[i - 1][3] * TR[i][0] * TR[i][1] * TR[i][4] * TR[i][2];
		}
		TR[k][0] = glm::translate(mat4(1), vec3(0.0f, 0.0f, -2.05f));
		TR[k][2] = glm::translate(mat4(1), vec3(0.0f, 0.0f, 6.15f));
		//TR[k][3] = TR[k - 1][3] * TR[k][0] * TR[k][1] * TR[k][2];

		for (int i = k ; i < NUM_OF_CUBES; i++) {
			TR[i][3] = TR[i - 1][3] * TR[i][0] * TR[i][1] * TR[i][4] * TR[i][2];
		}

		TR[k][0] = glm::translate(mat4(1), vec3(0.0f, 0.0f, 2.05f));
		TR[k][2] = glm::translate(mat4(1), vec3(0.0f, 0.0f, 2.05f));
		
		TR[NUM_OF_CUBES][3] = TR[0][3] * TR[NUM_OF_CUBES][0] * TR[NUM_OF_CUBES][1] * TR[NUM_OF_CUBES][4] * TR[NUM_OF_CUBES][2];

}

void updateTR() {
	TR[0][3] = TR[0][0] * TR[0][1] *TR[0][4]* TR[0][2];

	for (int i = 1; i < NUM_OF_CUBES; i++) {
		TR[i][3] = TR[i - 1][3] * TR[i][0] * TR[i][1] * TR[i][4] * TR[i][2];
	}

	TR[NUM_OF_CUBES][3] = TR[0][3] * TR[NUM_OF_CUBES][0] * TR[NUM_OF_CUBES][1] * TR[NUM_OF_CUBES][4] * TR[NUM_OF_CUBES][2];
	
	if (transUpdateIDX > 0) {
		TR[transUpdateIDX][3] = transUpdate* TR[transUpdateIDX][3];
		transUpdate = mat4(1);
		transUpdateIDX = -2;
	}
	
}

void updatePicked(float xpos, float ypos) {
	unsigned char res[4];
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	glReadPixels(xpos, viewport[3] - ypos, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &res);
	GLint id;
	glGetIntegerv(GL_CURRENT_PROGRAM, &id);
	float depth;
	glReadPixels(xpos, viewport[3] - ypos, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
	float z = 100.0f + depth*(0.1f - 100.0f);
	if (prevX == NULL) {
		prevX = xpos;
	}
	if (prevY == NULL) {
		prevY = ypos;
	}
	float xrel =  xpos- prevX;
	float yrel =  ypos- prevY;
	float M_PI = 3.141592654;
	transX = 1 * (xrel) / (float)(viewport[2])*0.1f*2.0*tan(60.0f*M_PI / 360.0)*(100.0f / z);
	transY = (yrel) / (float)(viewport[3])*0.1f*2.0*tan(60.0f*M_PI / 360.0)*(100.0f / z);
	prevX = xpos;
	prevY = ypos;
	switch (res[0]) {
	case(255):picked = 5;
		break;
	case(128):picked = 1;
		break;
	case(85):picked = 2;
		break;
	case(64):picked = 3;
		break;
	case(51):picked = 4;
		break;
	case(0):picked = -1;
		break;
	default:
		printf("\n");
	}
}
void rotateZ(int i, float angle, float clock) {
	if (i == -1) {
		TR[0][4] = glm::rotate(mat4(1), angle, vec3(0.0f, 0.0f, clock))*TR[0][4];
		updateTR();
	}
	else if (i == NUM_OF_CUBES||i==1) {
		TR[i][4] = glm::rotate(mat4(1), angle, vec3(0.0f, 0.0f, clock))*TR[i][4];
		updateTR();
	}
	else {
		TR[i][4] = glm::rotate(mat4(1), angle, vec3(0.0f, 0.0f, clock))*TR[i][4];
		updateTRz(i);
	}
	

}
void rotateX(int i, float angle,float clock) {
	if (i == -1) {
		TR[0][1] = glm::rotate(mat4(1), angle, vec3(clock, 0.0f, 0.0f))*TR[0][1];
	}
	else {
		TR[i][1] = glm::rotate(mat4(1), angle, vec3(clock, 0.0f, 0.0f))*TR[i][1];
		
	}
	updateTR();

}

void SolveIK() {

	mat4 tmp_0 = TR[NUM_OF_CUBES][3];
	mat4 tmpE = TR[NUM_OF_CUBES - 1][3];
	vec3 D = vec3(tmp_0[3][0], tmp_0[3][1], tmp_0[3][2]);
	vec3 E = vec3(tmpE[3][0], tmpE[3][1], tmpE[3][2] + 2.05f);
	vec3 distance = D - E;
	float currDistance = glm::length(distance);
	if (currDistance > 16) {
		printf("cannot reach\n");
		IK = false;
	}
	if (currDistance <= 0.1f) {
		printf("end of IK solver\n");
		IK = false;
	}


	for (int i = NUM_OF_CUBES - 1; i > 0 && IK; i--) {
		mat4 tmp = TR[i][3];
		vec3 R = vec3(tmp[3][0], tmp[3][1], tmp[3][2] - 2.05f);
		tmpE = TR[NUM_OF_CUBES - 1][3];
		E = vec3(tmpE[3][0], tmpE[3][1], tmpE[3][2] + 2.05f);
		vec3 RE = glm::normalize(E - R);
		vec3 RD = glm::normalize(D - R);
		vec3 crossed = vec3(glm::cross(RE, RD));
		float cosA = glm::clamp(dot(RE, RD), -1.0f, 1.0f);
		float angle = glm::acos(cosA);
		TR[i][1] = glm::rotate(mat4(1), angle, crossed)*TR[i][1];
		updateTR();
		distance = D - E;
	}

}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	switch (key)
	{
	case GLFW_KEY_SPACE:
		if (action == GLFW_PRESS) {
			if (countIK % 2 == 0)
				IK = true;
			else
				IK = false;
			countIK++;
		}
		break;
	case GLFW_KEY_ESCAPE:
		if (action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		break;

	case GLFW_KEY_UP:
		if (action == GLFW_PRESS) {
			rotateX(picked,5.0f, 1.0f);

		}
		break;

	case GLFW_KEY_DOWN:
		if (action == GLFW_PRESS) {
			rotateX(picked, 5.0f, -1.0f);

		}
		break;
	case GLFW_KEY_LEFT:
		if (action == GLFW_PRESS) {
			rotateZ(picked, 5.0f, -1.0f);

		}
		break;

	case GLFW_KEY_RIGHT:
		if (action == GLFW_PRESS) {
			rotateZ(picked, 5.0f, 1.0f);
		}
		break;

	default:
		break;
	}
}
void scroll_callback(GLFWwindow* window,double xpos,
	double ypos) {

}


void cursor_position_callback(GLFWwindow* window, double xpos,
	double ypos)
{

	display1.Clear(1.0f, 1.0f, 1.0f, 1.0f);
	vec3 color = vec3(1.0f, 1.0f, 1.0f);
	MVP = P*M*TR[NUM_OF_CUBES][3];
	shader1.Bind();
	shader1.Update(MVP, M, color);
	mesh.Draw();

	for (int i = 1; i < NUM_OF_CUBES; i++) {
		
		MVP = P*M*TR[i][3]* glm::scale(mat4(1), vec3(1.0f, 1.0f, 2.0f));
		color = vec3(1.0f / (i + 1), 1.0f / (i + 2), 1.0f / (i + 3));


		shader1.Bind();
		shader1.Update(MVP, M, color);
		mesh.Draw();

	}
	float deltaX =abs(xpos - prevX);
	float deltaY = abs(ypos - prevY);
	tmpPrevX = prevX;
	tmpPrevY = prevY;
	updatePicked(xpos, ypos);

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_3) ==
		GLFW_REPEAT)
	{
		
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) ==
		GLFW_PRESS)
	{
		//updatePicked(xpos, ypos);
		if (picked == NUM_OF_CUBES) {
			TR[NUM_OF_CUBES][3] = glm::translate(mat4(1), vec3(-transX, -transY/2, 0.0f ))*TR[NUM_OF_CUBES][3];
			//updateTR();
		}
		else {
			for (int j = 1; j < NUM_OF_CUBES; j++) {
				TR[j][3] = glm::translate(mat4(1), vec3(-transX, -transY/2,0.0f))*TR[j][3];
				//transUpdateIDX = j;
				//updateTR();
			}
		}
		

	}
	else
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) ==
			GLFW_PRESS)
		{
			
			if (deltaX > deltaY) {
				if (xpos > tmpPrevX) {
					rotateZ(picked, 0.5f, 1.0f);
				}
				else {
					rotateZ(picked, 0.5f, -1.0f);
				}
			}
			else {
				if (ypos > tmpPrevY) {
					rotateX(picked, 0.5f, -1.0f);
				}
				else {
					rotateX(picked, 0.5f, 1.0f);
				}
			}

		}
}

void initialize() {
	TR[0][0] = mat4(1);
	TR[0][1] = rotate(mat4(1), -90.0f, vec3(1.0f, 0, 0));
	TR[0][2] = mat4(1);
	TR[0][4] = mat4(1);
	TR[1][0] = glm::translate(mat4(1), vec3(0.0f, 0.0f, -2.05f));
	TR[1][1] = mat4(1);
	TR[1][2] = glm::translate(mat4(1), vec3(0.0f, 0.0f, 2.05f));
	TR[1][4] = mat4(1);
	TR[NUM_OF_CUBES][0] = glm::translate(mat4(1), vec3(5.0f, 0.0f, 0.0f));
	TR[NUM_OF_CUBES][1] = mat4(1);
	TR[NUM_OF_CUBES][2] = mat4(1);
	TR[NUM_OF_CUBES][4] = mat4(1);
	
	for (int i = 2; i < NUM_OF_CUBES; i++) {
		TR[i][0] = glm::translate(mat4(1), vec3(0.0f, 0.0f, 2.05f));
		TR[i][1] = mat4(1);
		TR[i][2] = glm::translate(mat4(1), vec3(0.0f, 0.0f, 2.05f));
		TR[i][4] = mat4(1);
	}

}



int main(int argc, char** argv)
{

	
	glfwSetKeyCallback(display1.m_window, key_callback);
	glfwSetCursorPosCallback(display1.m_window, cursor_position_callback);
	glfwSetScrollCallback(display1.m_window, scroll_callback);
	initialize();
	updateTR();

	while (!glfwWindowShouldClose(display1.m_window))
	{
		Sleep(3);
		display1.Clear(1.0f, 1.0f, 1.0f, 1.0f);
		if (IK) {
			SolveIK();
		}
		shader.Bind();
		MVP = P*M*TR[NUM_OF_CUBES][3];
		shader.Update(MVP, M, vec3(1.0f, 1.0f, 1.0f));
		mesh.Draw();

		for (int i = 1; i < NUM_OF_CUBES; i++) {
			
			MVP = P*M*TR[i][3]*glm::scale(mat4(1),vec3(1.0f,1.0f,2.0f));
			shader.Bind();
			shader.Update(MVP, M, vec3(1.0f, 1.0f, 1.0f));

			glEnable(GL_COLOR_MATERIAL);
			glBegin(GL_LINES);
			//X Axis
			glVertex3f(-5.0, 0.0, -1.025);
			glVertex3f(5.0, 0.0, -1.025);
			//Y Axix
			glVertex3f(0.0, -5.0, -1.025);
			glVertex3f(0.0, 5.0, -1.025);
			//Z Axis
			glVertex3f(0.0, 0.0, -5.0);
			glVertex3f(0.0, 0.0, 5.0);
			glEnd();
			glDisable(GL_COLOR_MATERIAL);

			mesh.Draw();
			
		}

		display1.SwapBuffers();
		glfwPollEvents();

	}

	return 0;
}





