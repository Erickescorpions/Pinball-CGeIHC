#pragma once
#include<stdio.h>
#include<glew.h>
#include<glfw3.h>

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);
	int Initialise();
	GLfloat getBufferWidth() { return bufferWidth; }
	GLfloat getBufferHeight() { return bufferHeight; }
	GLfloat getXChange();
	GLfloat getYChange();
	GLfloat getmuevex() { return muevex; }
	GLfloat getClickDerecho() { return clickDerecho;  }
	bool getCamara() { return camara; }
	bool getFlipper1() { return flipper1;  }
	bool getFlipper2() { return flipper2; }
	bool getFlipper3() { return flipper3; }
	bool imprimirPosicion() { return imprimePosicion; }

	bool getShouldClose() {
		return  glfwWindowShouldClose(mainWindow);}
	bool* getsKeys() { return keys; }
	void swapBuffers() { return glfwSwapBuffers(mainWindow); }
	
	~Window();
private: 
	GLFWwindow *mainWindow;
	GLint width, height;
	bool keys[1024];
	GLint bufferWidth, bufferHeight;
	void createCallbacks();
	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	GLfloat muevex;
	bool camara;
	bool mouseFirstMoved;
	bool clickDerecho = false;
	bool flipper1 = false, flipper2 = false, flipper3 = false;
	bool imprimePosicion = false;
	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);
	static void ManejaClickRaton(GLFWwindow* window, int button, int action, int mods);
};

