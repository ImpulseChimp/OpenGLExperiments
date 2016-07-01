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
static void handleCursorInput(GLFWwindow* window, double xpos, double ypos);
void handleScrollInput(GLFWwindow* window, double xoffset, double yoffset);

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

int functionSelection = 0;
enum algorithmModes { TRIANGLE_FRACTAL, TRIANGLE_ARMY, SQUARE_WAVE };
int functionModes[] = { TRIANGLE_FRACTAL, TRIANGLE_ARMY, SQUARE_WAVE };

float stepSpeed = 0.0005f;
float animationTime = 0;

float xRotation = 0;
float yRotation = 0;
float zOffset = 0;
double previousMouseX = 0;
double previousMouseY = 0;

int renderCount = 100;
int verticesRendered = 3;

float oldStepSpeed = 0;



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
			snprintf(title, 255, "%s - [FPS: %d][Rendered: %d]", "Fractals: ", fps, renderCount);
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

	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, "C:\\Users\\Christopher\\Documents\\visual studio 2015\\Projects\\OpenGLApplication\\OpenGLApplication\\triangles.vert" },
		{ GL_FRAGMENT_SHADER, "C:\\Users\\Christopher\\Documents\\visual studio 2015\\Projects\\OpenGLApplication\\OpenGLApplication\\triangles.frag" },
		{ GL_NONE, NULL }
	};

	GLuint program = LoadShaders(shaders);
	glUseProgram(program);

	int positionId = glGetUniformLocation(program, "vPosition");
	orthoId = glGetUniformLocation(program, "ortho");
	colorId = glGetUniformLocation(program, "color");
	modelMatrixId = glGetUniformLocation(program, "modelMatrix");
	viewMatrixId = glGetUniformLocation(program, "viewMatrix");
	projectionMatrixId = glGetUniformLocation(program, "projectionMatrix");

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
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

	if (functionModes[functionSelection] == TRIANGLE_FRACTAL)
	{
		for (int i = 0; i < renderCount; ++i)
		{
			mat4 translate = glm::translate(mat4(), vec3(0, 0, ((float)i/100) - 1));
			mat4 rotate = glm::rotate(mat4(), animationTime * 3.14159f * (float)i / 100, vec3(0, 0, 1));
			mat4 scale = glm::scale(mat4(), vec3((float)i / 100));

			mat4 model = translate * rotate * scale;
			glUniformMatrix4fv(modelMatrixId, 1, GL_FALSE, value_ptr(model));

			glDrawArrays(renderModes[renderSelection], 0, verticesRendered);
		}
	}
	else if (functionModes[functionSelection] == TRIANGLE_ARMY)
	{
		for (int i = 0; i < renderCount; ++i)
		{
			float waveOffset = sin((animationTime + (float)i / (float)renderCount)) + 0.5f;

			mat4 translate = glm::translate(mat4(), vec3((float)i / (float)renderCount, waveOffset, 0));
			mat4 rotate = glm::rotate(mat4(), 0.0f, vec3(0, 0, 1));
			mat4 scale = glm::scale(mat4(), vec3((float)i / 100));

			mat4 model = translate * rotate * scale;
			glUniformMatrix4fv(modelMatrixId, 1, GL_FALSE, value_ptr(model));

			glDrawArrays(renderModes[renderSelection], 0, verticesRendered);
		}
	}
	else if (functionModes[functionSelection] == SQUARE_WAVE)
	{
		for (int i = 0; i < renderCount; ++i)
		{
			mat4 translate = glm::translate(mat4(), vec3(xRotation, yRotation, (float)i / 100));
			mat4 rotate = glm::rotate(mat4(), animationTime * 3.14159f * (float)i / 100, vec3(xRotation, yRotation, 1));
			mat4 scale = glm::scale(mat4(), vec3(zOffset - (float)i / 100));

			mat4 model = translate * rotate * scale;
			glUniformMatrix4fv(modelMatrixId, 1, GL_FALSE, value_ptr(model));

			glDrawArrays(renderModes[renderSelection], 0, 3);
		}
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
	case GLFW_KEY_RIGHT:;
		if (action == GLFW_PRESS)
		{
			if (functionSelection + 1 > 2)
				functionSelection = 0;
			else
				functionSelection++;
		}
		break;
	case GLFW_KEY_LEFT:;
		if (action == GLFW_PRESS)
		{
			if (functionSelection - 1 < 0)
				functionSelection = 2;
			else
				functionSelection--;
		}
		break;
	case GLFW_KEY_ESCAPE:
		runGame = false;
		break;
	case GLFW_KEY_S:
		    renderCount++;
		break;
	case GLFW_KEY_A:
		    renderCount--;
		break;
	case GLFW_KEY_W:
		if (action == GLFW_PRESS)
			verticesRendered++;
		break;
	case GLFW_KEY_Q:
		if (action == GLFW_PRESS)
			verticesRendered--;
		break;
	case GLFW_KEY_EQUAL:
		stepSpeed += 0.0001;
		break;
	case GLFW_KEY_MINUS:
		stepSpeed -= 0.0001;
		break;
	case GLFW_KEY_SPACE:
		if (action == GLFW_PRESS)
		{
			if (stepSpeed == 0)
			{
				stepSpeed = oldStepSpeed;
			}
			else
			{
				oldStepSpeed = stepSpeed;
				stepSpeed = 0;
			}
		}
		break;
	}
}

void windowResizeCallback(GLFWwindow* window, int width, int height)
{
	glfwSetWindowSize(window, width, height);
	glViewport(0, 0, width, height);
}

static void handleCursorInput(GLFWwindow* window, double xpos, double ypos)
{
	xRotation = xpos/600.0f;
	yRotation = ypos/400.0f;
}

void handleScrollInput(GLFWwindow* window, double xoffset, double yoffset)
{
	zOffset += yoffset;
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
	glfwSetCursorPosCallback(window, handleCursorInput);
	glfwSetScrollCallback(window, handleScrollInput);

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
