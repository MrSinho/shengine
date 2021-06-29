#include <Foggy-Engine.h>

int main() {

	VkData computeData = VKDataInitPrerequisites(720, 480, "Foggy-Engine Editor");

	InitVulkan(&computeData);

	while (IsWindowActive(computeData.window.window)) {
		PollEvents();
	}

	return 0;
}