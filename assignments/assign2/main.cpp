#include <Windows.h>
#include <iostream>
#include "display.h"
#include "mesh.h"
#include "shader.h"
#include "inputManager.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <array>
#include "GLFW\glfw3.h"




using namespace glm;

static const int DISPLAY_WIDTH = 800;
static const int DISPLAY_HEIGHT = 800;
float alpha=90.0f;
 mat4 M=glm::rotate(1.0f,normalize(vec3(1,1,1)));
Display display(DISPLAY_WIDTH, DISPLAY_HEIGHT, "OpenGL");
vec3 directionX=vec3(1.0f,0.0f,0.0f);
vec3 directionY=vec3(0.0f,1.0f,0.0f);
vec3 directionZ=vec3(0.0f,0.0f,1.0f);
mat4 tmp;
	
mat4 TR[27][2] ={

		{glm::translate(mat4(1),vec3(2.05f,2.05f,2.05f)),mat4(1)},	//0 
		{glm::translate(mat4(1),vec3(0.0f,2.05f,2.05f)),mat4(1)},		//1  
		{glm::translate(mat4(1),vec3(-2.05f,2.05f,2.05f)),mat4(1)}	,	//2  
		{glm::translate(mat4(1),vec3(2.05f,0.0f,2.05f)),mat4(1)},		//3  
		{glm::translate(mat4(1),vec3(0.0f,0.0f,2.05f)),mat4(1)},			//4  
		{glm::translate(mat4(1),vec3(-2.05f,0.0f,2.05f)),mat4(1)},		//5  
		{glm::translate(mat4(1),vec3(2.05f,-2.05f,2.05f)),mat4(1)},	//6  
		{glm::translate(mat4(1),vec3(0.0f,-2.05f,2.05f)),mat4(1)},		//7  
		{glm::translate(mat4(1),vec3(-2.05f,-2.05f,2.05f)),mat4(1)},	//8  
		{glm::translate(mat4(1),vec3(-2.05f,2.05f,0.0f)),mat4(1)	},		//9  
		{glm::translate(mat4(1),vec3(-2.05f,2.05f,-2.05f)),mat4(1)},	//10  
		{glm::translate(mat4(1),vec3(2.05f,2.05f,0.0f)),mat4(1)},		//11 
		{glm::translate(mat4(1),vec3(2.05f,2.05f,-2.05f)),mat4(1)},	//12 
		{glm::translate(mat4(1),vec3(2.05f,-2.05f,0.0f)),mat4(1)},		//13 
		{glm::translate(mat4(1),vec3(2.05f,-2.05f,-2.05f)),mat4(1)}, //14 
		{glm::translate(mat4(1),vec3(-2.05f,-2.05f,0.0f)),mat4(1)},		//15 
		{glm::translate(mat4(1),vec3(-2.05f,-2.05f,-2.05f)),mat4(1)},	//16 
		{glm::translate(mat4(1),vec3(0.0f,-2.05f,0.0f)),mat4(1)},			//17 
		{glm::translate(mat4(1),vec3(0.0f,-2.05f,-2.05f)),mat4(1)},		//18 
		{glm::translate(mat4(1),vec3(-2.05f,0.0f,0.0f)),mat4(1)},			//19 
		{glm::translate(mat4(1),vec3(-2.05f,0.0f,-2.05f)),mat4(1)},		//20 
		{glm::translate(mat4(1),vec3(2.05f,0.0f,0.0f)),mat4(1)},			//21 
		{glm::translate(mat4(1),vec3(2.05f,0.0f,-2.05f)),mat4(1)},		//22 
		{glm::translate(mat4(1),vec3(0.0f,2.05f,0.0f)),mat4(1)},			//23 
		{glm::translate(mat4(1),vec3(0.0f,2.05f,-2.05f)),mat4(1)},		//24 
		{glm::translate(mat4(1),vec3(0.0f,0.0f,-2.05f)),mat4(1)},			//25 
		{glm::translate(mat4(1),vec3(0.0f,0.0f,0.0f)),mat4(1)},         	//26 
	};


	
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			if(action == GLFW_PRESS)
				glfwSetWindowShouldClose(window,GLFW_TRUE);
			break;
		
		case GLFW_KEY_RIGHT:
			if(action == GLFW_PRESS){
				M= glm::rotate(M,45.0f,vec3(0,1.0f,0));
		
			}
			break;

		case GLFW_KEY_LEFT:
			if(action == GLFW_PRESS){
				M= glm::rotate(M,45.0f,vec3(0,-1.0f,0));
			
			}
			break;
	

		case GLFW_KEY_DOWN:
			if(action == GLFW_PRESS){
				
				M= glm::rotate(M,45.0f,vec3(-1.0f,0,0));
			
			}
			break;

		case GLFW_KEY_UP:
			if(action == GLFW_PRESS){
				M=glm::rotate(M,45.0f,vec3(1.0f,0,0));
	
			}
			break;

		case GLFW_KEY_R:
			if(action == GLFW_PRESS){
				for(int j= 0; j<sizeof(TR)/sizeof(TR[0]); j++){
					tmp =TR[j][1]*TR[j][0];					
					if( tmp[3][0]<-0.01f){ 
						TR[j][1]=glm::rotate(mat4(1),alpha,directionX)*TR[j][1];
					}
				}
			}
		break;

		case GLFW_KEY_L:
			if(action == GLFW_PRESS){
				for(int j= 0; j<sizeof(TR)/sizeof(TR[0]); j++){
					tmp =TR[j][1]*TR[j][0];					
					if( tmp[3][0]>0.01f){ 
						TR[j][1]=glm::rotate(mat4(1),alpha,directionX)*TR[j][1];
					}
				}
				
			}	
		break;			

		case GLFW_KEY_U:
			if(action == GLFW_PRESS){		
				for(int j= 0; j<sizeof(TR)/sizeof(TR[0]); j++){
					tmp =TR[j][1]*TR[j][0];
					if( tmp[3][1]>0.01f){ 
						TR[j][1]=glm::rotate(mat4(1),alpha,directionY)*TR[j][1];
					}
				}	
			}
		break;

		case GLFW_KEY_D:
			if(action == GLFW_PRESS){
				for(int j= 0; j<sizeof(TR)/sizeof(TR[0]); j++){
					tmp =TR[j][1]*TR[j][0];
					if( tmp[3][1]<-0.01f){ 
						TR[j][1]=glm::rotate(mat4(1),alpha,directionY)*TR[j][1];
					}
				}	
			}
		break;


		case GLFW_KEY_F:
			if(action == GLFW_PRESS){
				for(int j= 0; j<sizeof(TR)/sizeof(TR[0]); j++){
					tmp =TR[j][1]*TR[j][0];					
					if( tmp[3][2]<-0.01f){ 
						TR[j][1]=glm::rotate(mat4(1),alpha,directionZ)*TR[j][1];
					}
				}
			}
		break;

		case GLFW_KEY_B:
			if(action == GLFW_PRESS){
				for(int j= 0; j<sizeof(TR)/sizeof(TR[0]); j++){
					tmp =TR[j][1]*TR[j][0];					
					if( tmp[3][2]>0.01f){ 
						TR[j][1]=glm::rotate(mat4(1),alpha,directionZ)*TR[j][1];
					}
				}
			}
		break;

		case GLFW_KEY_SPACE:
			if(action == GLFW_PRESS){
				directionX = -directionX;
				directionY = -directionY;
				directionZ = -directionZ;
			}
		break;

		case GLFW_KEY_Z:
			if(action == GLFW_PRESS){
				alpha = alpha/2;
			}
		break;

		case GLFW_KEY_A:
			if(action == GLFW_PRESS){
				if (alpha<=90.0f)
					alpha = alpha*2;
			}
		break;


		default:
			break;
	
	
	}
}
int main(int argc, char** argv)
{
	
	
	Vertex vertices[] =
	{
		Vertex(glm::vec3(-1, -1, -1), glm::vec2(1, 0), glm::vec3(0, 0, -1),glm::vec3(0, 0, 0.5)),
		Vertex(glm::vec3(-1, 1, -1), glm::vec2(0, 0), glm::vec3(0, 0, -1),glm::vec3(0, 0, 0.5)),
		Vertex(glm::vec3(1, 1, -1), glm::vec2(0, 1), glm::vec3(0, 0, -1),glm::vec3(0, 0, 0.5)),
		Vertex(glm::vec3(1, -1, -1), glm::vec2(1, 1), glm::vec3(0, 0, -1),glm::vec3(0, 0, 0.5)),

		Vertex(glm::vec3(-1, -1, 1), glm::vec2(1, 0), glm::vec3(0, 0, 1),glm::vec3(0, 0.5,0.5)),
		Vertex(glm::vec3(-1, 1, 1), glm::vec2(0, 0), glm::vec3(0, 0, 1),glm::vec3(0,0.5, 0.5)),
		Vertex(glm::vec3(1, 1, 1), glm::vec2(0, 1), glm::vec3(0, 0, 1),glm::vec3(0, 0.5, 0.5)),
		Vertex(glm::vec3(1, -1, 1), glm::vec2(1, 1), glm::vec3(0, 0, 1),glm::vec3(0, 0.5, 0.5)),

		Vertex(glm::vec3(-1, -1, -1), glm::vec2(0, 1), glm::vec3(0, -1, 0),glm::vec3(0, 1, 0)),
		Vertex(glm::vec3(-1, -1, 1), glm::vec2(1, 1), glm::vec3(0, -1, 0),glm::vec3(0, 1, 0)),
		Vertex(glm::vec3(1, -1, 1), glm::vec2(1, 0), glm::vec3(0, -1, 0),glm::vec3(0, 1, 0)),
		Vertex(glm::vec3(1, -1, -1), glm::vec2(0, 0), glm::vec3(0, -1, 0),glm::vec3(0, 1, 0)),

		Vertex(glm::vec3(-1, 1, -1), glm::vec2(0, 1), glm::vec3(0, 1, 0),glm::vec3(1, 1, 0)),
		Vertex(glm::vec3(-1, 1, 1), glm::vec2(1, 1), glm::vec3(0, 1, 0),glm::vec3(1, 1, 0)),
		Vertex(glm::vec3(1, 1, 1), glm::vec2(1, 0), glm::vec3(0, 1, 0),glm::vec3(1, 1, 0)),
		Vertex(glm::vec3(1, 1, -1), glm::vec2(0, 0), glm::vec3(0, 1, 0),glm::vec3(1, 1, 0)),

		Vertex(glm::vec3(-1, -1, -1), glm::vec2(1, 1), glm::vec3(-1, 0, 0),glm::vec3(1, 0, 1)),
		Vertex(glm::vec3(-1, -1, 1), glm::vec2(1, 0), glm::vec3(-1, 0, 0),glm::vec3(1, 0, 1)),
		Vertex(glm::vec3(-1, 1, 1), glm::vec2(0, 0), glm::vec3(-1, 0, 0),glm::vec3(1, 0, 1)),
		Vertex(glm::vec3(-1, 1, -1), glm::vec2(0, 1), glm::vec3(-1, 0, 0),glm::vec3(1, 0, 1)),

		Vertex(glm::vec3(1, -1, -1), glm::vec2(1, 1), glm::vec3(1, 0, 0),glm::vec3(1, 0, 0)),
		Vertex(glm::vec3(1, -1, 1), glm::vec2(1, 0), glm::vec3(1, 0, 0),glm::vec3(1, 0, 0)),
		Vertex(glm::vec3(1, 1, 1), glm::vec2(0, 0), glm::vec3(1, 0, 0),glm::vec3(1, 0, 0)),
		Vertex(glm::vec3(1, 1, -1), glm::vec2(0, 1), glm::vec3(1, 0, 0),glm::vec3(1, 0, 0))
	};

	unsigned int indices[] = {0, 1, 2,
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

	Mesh mesh(vertices, sizeof(vertices)/sizeof(vertices[0]), indices, sizeof(indices)/sizeof(indices[0]));
	Shader shader("./res/shaders/basicShader");

	vec3 pos = vec3(0,0,-15);
	vec3 forward = glm::vec3(0.0f, 0.0f, 1.0f);
	vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	mat4 P = glm::perspective(60.0f, (float)DISPLAY_WIDTH/(float)DISPLAY_HEIGHT, 0.1f, 100.0f);
	//mat4 M = glm::rotate(1.0f,glm::normalize(vec3(1,1,1)));//glm::mat4(1);
	P = P * glm::lookAt(pos, pos + forward, up);
	mat4 MVP = P*M;
	
	glfwSetKeyCallback(display.m_window,key_callback);
	
	while(!glfwWindowShouldClose(display.m_window))
	{
		Sleep(3);
		
		display.Clear(1.0f, 1.0f, 1.0f, 1.0f);
		
		for(int i= 0; i<sizeof(TR)/sizeof(TR[0]); i++){
		
				Sleep(15);
				MVP = P*M*TR[i][1]*TR[i][0];
				shader.Bind();
				shader.Update(MVP,M);
				mesh.Draw();
		}
			
		display.SwapBuffers();

		glfwPollEvents();
	}

	return 0;
}