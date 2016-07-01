#include <GL/glew.h>
#include <GL/GLU.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <stdio.h>
#include "player.cpp"
#include "LoadShaders.h"

using namespace std;
using namespace glm;

void initilizeWindow();
void initializeDependencies(void);
void redrawScreen(GLFWwindow* window);
void updateGameState();
int getCurrentTime();
void windowResizeCallback(GLFWwindow* window, int width, int height);
void handleKeyboardInput(GLFWwindow* window, int key, int scancode, int action, int mode);

int orthoId;
GLint modelMatrixId;
GLint viewMatrixId;
GLint projectionMatrixId;

bool runGame = true;

GLfloat test[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
};

enum VAO_IDs { Triangles = 0, VAOSize = 1 };
enum Buffer_IDs { ArrayBuffer = 0, VBOSize = 1};
enum Attrib_IDs { vPosition = 0 };
enum Game_Loop { MS_PER_UPDATE = 16 };

Player* player;

unsigned int vaoID[VAOSize];
unsigned int vboID[VBOSize];

float xOrigin = 0;
float yOrigin = 0;

GLFWwindow* window;

GLuint vertexArrays[VAOSize];
GLuint buffers[VBOSize];
bool isFullscreen = false;

float aspectRatio = 16.0f / 9.0f;
float windowHeight = 400;
float windowWidth = windowHeight * aspectRatio;

int main() {

	initilizeWindow();
	initializeDependencies();
	player = new Player(-1, -1, 1, 1);

	double previous = getCurrentTime();
	double lag = 0.0;
	double current = 0;
	double elapsed = 0;

	while (runGame)
	{
		current = getCurrentTime();
		elapsed = current - previous;
		previous = current;
		lag += elapsed;

		glfwPollEvents();

		while (lag >= MS_PER_UPDATE)
		{
			updateGameState();
			lag -= MS_PER_UPDATE;
		}

		redrawScreen(window);
	}

	glfwTerminate();
	return 0;
}

void initializeDependencies(void) {


	glGenVertexArrays(VAOSize, &vaoID[Triangles]);
	glBindVertexArray(vaoID[Triangles]);

	glGenBuffers(VBOSize, &vboID[ArrayBuffer]);
	glBindBuffer(GL_ARRAY_BUFFER, vboID[ArrayBuffer]);
	glBufferData(GL_ARRAY_BUFFER, 12 * 3 * sizeof(GLfloat), test, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vboID[ArrayBuffer]);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, "C:\\Users\\Christopher\\Documents\\visual studio 2015\\Projects\\OpenGLApplication\\OpenGLApplication\\triangles.vert" },
		{ GL_FRAGMENT_SHADER, "C:\\Users\\Christopher\\Documents\\visual studio 2015\\Projects\\OpenGLApplication\\OpenGLApplication\\triangles.frag" },
		{ GL_NONE, NULL }
	};

	GLuint program = LoadShaders(shaders);
	glUseProgram(program);

	orthoId = glGetUniformLocation(program, "ortho");
	modelMatrixId = glGetUniformLocation(program, "modelMatrix");
	viewMatrixId = glGetUniformLocation(program, "viewMatrix");
	projectionMatrixId = glGetUniformLocation(program, "projectionMatrix");
}


void redrawScreen(GLFWwindow* window)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 ortho = glm::ortho(-aspectRatio, aspectRatio, -1.f, 1.f, -1.f, 1.f);
	glUniformMatrix4fv(orthoId, 1, GL_FALSE, value_ptr(ortho));

	mat4 view = lookAt(vec3(0, 0, 2), vec3(0, 0, 0), vec3(0, 1, 0));
	mat4 projection = perspective(3.14159f/3.0f, aspectRatio, 0.1f, -10.0f);

	glUniformMatrix4fv(viewMatrixId, 1, GL_FALSE, value_ptr(view));
	glUniformMatrix4fv(projectionMatrixId, 1, GL_FALSE, value_ptr(projection));

	glBindVertexArray(vaoID[Triangles]);

	mat4 translate
		mat4 rotate = rotate(mat4(), time * 3.14159 * 0.2f);
		mat4 scale = scale(mat4(), vec3(1.0f - 0.2f));

	mat4 model = translate * rotate * scale;
	glUniformMatrix4fv(modelMatrixId, 1, GL_FALSE, value_ptr(model));

	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);

	glfwSwapBuffers(window);
	glFlush();
}

void updateGameState()
{
}

int getCurrentTime()
{
	return glfwGetTime();
}

void handleKeyboardInput(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	switch (key) 
	{
	case GLFW_KEY_D:
		break;
	case GLFW_KEY_A:
		break;
	case GLFW_KEY_SPACE:
		if (action == GLFW_PRESS)
		break;
	}
}

void windowResizeCallback(GLFWwindow* window, int width, int height)
{
	glfwSetWindowSize(window, width, height);
	glViewport(0, 0, width, height);
}

void initilizeWindow()
{
	//Initilize GLFW
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		exit(-1);
	}

	window = glfwCreateWindow(windowWidth, windowHeight, "Shark Week", NULL, NULL);
	if (!window) {
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		exit(-1);
	}

	//Set GLFW Callbacks
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, handleKeyboardInput);
	glfwSetWindowSizeCallback(window, windowResizeCallback);

	//Setup and initilize GLEW
	glewExperimental = GL_TRUE;
	!glewInit();

	//Set initial OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glLineWidth(3);
	glClearColor(0.2f, 0.2f, 0.2f, 1);

	//Print Startup Info
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);
}
