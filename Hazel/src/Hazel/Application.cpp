#include "Application.h"
#include <iostream>

namespace Hazel {

	Application::Application()
	{
	}


	Application::~Application()
	{
	}

	void Application::Run()
	{
		std::cout << "Hazel running..." << std::endl;
		while (true);
	}

}