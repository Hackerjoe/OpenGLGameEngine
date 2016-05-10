#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#define WINDOW_WINDOWED 0
#define WINDOW_FULLWINDOW 1
#define WINDOW_FULLSCREEN 2

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <string>

using namespace std;

class WindowManager
{
public:
	static WindowManager* Instance();
	bool Init();
	GLFWwindow* CreateWindow(GLuint WindowWidth, GLuint WindowHeight, string WindowName, int ScreenMode);
	void SetWindowKeyCallback(GLFWwindow* window);
	void WindowSwapBuffers(GLFWwindow* window);
	void SetVsync(bool vsync);
	void SetContext(GLFWwindow* window);

private:
	WindowManager(void);
	~WindowManager(void);
	static WindowManager* pInstance;

	//TODO List of all open windows.
};
#endif

/*
#ifndef RENDERMANAGER_H
#define RENDERMANAGER_H

class RenderManager
{

public:
	static RenderManager* Instance();
	void Init();
	
private:
	RenderManager(void);
	~RenderManager(void);

	void GeometryPass();
	void StencilPass();
	void LightPass();

	void CalcMS();

	static RenderManager* pInstance;
};
#endif
*/