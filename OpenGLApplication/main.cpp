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

float time = 0;
int orthoId;
int colorId;
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

int renderSelection = 0;
int renderModes[] = {GL_TRIANGLES, GL_LINES, GL_POINTS};

float stepSpeed = 0.005f;
float animationTime = 0;

int main() {

	initilizeWindow();
	initializeDependencies();
	player = new Player(-1, -1, 1, 1);

	double startTime = getCurrentTime();
	int fps = 0;

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
		fps++;

		double currentTime = getCurrentTime();
		if ((currentTime - startTime) >= 1)
		{
			startTime = currentTime;

			char title[256];
			title[255] = '\0';
			snprintf(title, 255, "%s - [FPS: %d]", "Fractals: ", fps);
			glfwSetWindowTitle(window, title);

			fps = 0;
		}
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
	colorId = glGetUniformLocation(program, "color");
	modelMatrixId = glGetUniformLocation(program, "modelMatrix");
	viewMatrixId = glGetUniformLocation(program, "viewMatrix");
	projectionMatrixId = glGetUniformLocation(program, "projectionMatrix");
}


void redrawScreen(GLFWwindow* window)
{
	animationTime += stepSpeed;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 ortho = glm::ortho(-aspectRatio, aspectRatio, -1.f, 1.f, -1.f, 1.f);
	glUniformMatrix4fv(orthoId, 1, GL_FALSE, value_ptr(ortho));

	mat4 view = lookAt(vec3(0, 0, 2), vec3(0, 0, 0), vec3(0, 1, 0));
	mat4 projection = perspective(3.14159f/3.0f, aspectRatio, 0.1f, -10.0f);

	glUniformMatrix4fv(viewMatrixId, 1, GL_FALSE, value_ptr(view));
	glUniformMatrix4fv(projectionMatrixId, 1, GL_FALSE, value_ptr(projection));

	glBindVertexArray(vaoID[Triangles]);

	for (int i = 0; i < 100; ++i)
	{
		mat4 translate = glm::translate(mat4(), vec3((float)sin(animationTime), 0.0f, (float)i/100));
		mat4 rotate = glm::rotate(mat4(), animationTime * 3.14159f * (float)i / 100, vec3(0, 0, 1));
		mat4 scale = glm::scale(mat4(), vec3(1.0f - (float)i / 100));

		mat4 model = translate * rotate * scale;
		glUniformMatrix4fv(modelMatrixId, 1, GL_FALSE, value_ptr(model));

		glDrawArrays(renderModes[renderSelection], 0, 3);
	}

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
	case GLFW_KEY_UP:
		if (action == GLFW_PRESS)
		{
			if (renderSelection + 1 > 2)
				renderSelection = 0;
			else
				renderSelection++;
		}
		break;
	case GLFW_KEY_DOWN:
		if (action == GLFW_PRESS)
		{
			if (renderSelection - 1 < 0)
				renderSelection = 2;
			else
				renderSelection--;
		}
		break;
	case GLFW_KEY_ESCAPE:
		runGame = false;
		break;
	case GLFW_KEY_SPACE:
		if (action == GLFW_PRESS)
		{
			if (stepSpeed == 0)
				stepSpeed = 0.005f;
			else
				stepSpeed = 0;
		}
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
	glewInit();

	//Set initial OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glLineWidth(3);
	glPointSize(5);
	glClearColor(0.2f, 0.2f, 0.2f, 1);

	//Print Startup Info
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);
}
