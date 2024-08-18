#pragma once
#include <string>

namespace Sandbox 
{
	struct AppSpec {
		int width, height;
		std::string title;
	};

	class App
	{
	public:
		App(const AppSpec& spec);

		void run();
	private:
		AppSpec _spec;
	};
}
