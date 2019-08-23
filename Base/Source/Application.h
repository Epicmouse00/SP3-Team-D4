#ifndef APPLICATION_H
#define APPLICATION_H

#include "timer.h"
//#include <irrKlang.h>

//using namespace irrklang;

struct GLFWwindow;

class Application
{
public:
	static Application& GetInstance()
	{
		static Application app;
		return app;
	}
	void Init();
	void Run();
	void Exit();

	void UpdateInput();
	void PostInputUpdate();
	
	static void MouseButtonCallbacks(GLFWwindow* window, int button, int action, int mods);
	static void MouseScrollCallbacks(GLFWwindow* window, double xoffset, double yoffset);

	int GetWindowHeight();
	int GetWindowWidth();
	
private:
	Application();
	~Application();

	static bool IsKeyPressed(unsigned short key);

	// Just change this when you want to change screen size
	const static int m_window_width = 360;
	const static int m_window_height = 240;

	//Declare a window object
	StopWatch m_timer;
};

#endif