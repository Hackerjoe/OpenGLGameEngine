#include "WindowManager.h"

WindowManager* WindowManager::pInstance = nullptr;

WindowManager* WindowManager::Instance()
{
	if(!pInstance)
    {
		pInstance = new WindowManager();
    }
    
    return pInstance;
}

WindowManager::WindowManager(void)
{
}


WindowManager::~WindowManager(void)
{
}

bool WindowManager::Init()
{
	if(!glfwInit())
		return false;
}


GLFWwindow* WindowManager::CreateWindow(GLuint WindowWidth, GLuint WindowHeight, string WindowName, int ScreenMode)
{
	const char* windowName = WindowName.c_str();
	GLFWwindow* NewWindow = nullptr;

	if(ScreenMode == WINDOW_WINDOWED || ScreenMode < 0 || ScreenMode > 2)
		NewWindow = glfwCreateWindow(WindowWidth, WindowHeight, windowName, NULL, NULL);
	else if(ScreenMode == WINDOW_FULLWINDOW)
	{
		const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		glfwWindowHint(GLFW_RED_BITS, mode->redBits);
		glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
		glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
		glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
		
		NewWindow = glfwCreateWindow(mode->width, mode->height, windowName, glfwGetPrimaryMonitor(), NULL);
	}
	else if(ScreenMode == WINDOW_FULLSCREEN )
	{
		NewWindow = glfwCreateWindow(WindowWidth, WindowHeight, windowName, glfwGetPrimaryMonitor(), NULL);
	}

	SetContext(NewWindow);
	return NewWindow;
}

void WindowManager::SetContext(GLFWwindow* window)
{
	glfwMakeContextCurrent(window);
}

void WindowManager::SetVsync(bool vsync)
{
	glfwSwapInterval(vsync);
}

void WindowManager::WindowSwapBuffers(GLFWwindow* window)
{
	glfwSwapBuffers(window);
    glfwPollEvents();
}
