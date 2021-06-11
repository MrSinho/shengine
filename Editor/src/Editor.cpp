#include <Foggy-Engine.h>

int main() {

	Window* window = new Window(720, 480, "Foggy-Engine Editor");
	while (window->IsActive()) {
		glfwPollEvents();
	}

	return 0;
}