#include <GLFW/glfw3.h>
#include <cstdlib>

#include <loader.h>

void key_callback(GLFWwindow *window, int key, int, int action, int)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}

int main(int argc, char *argv[])
{

	if(argc != 2) {
		return 1;
	};

	if (!glfwInit())
	{
		return EXIT_FAILURE;
	}

	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	if(!mode)
	{
		return EXIT_FAILURE;
	}

	GLFWwindow *window = glfwCreateWindow(
		mode->width,
		mode->height,
		"Sponza Test",
		nullptr,
		nullptr
	);

	if (!window)
	{
		glfwTerminate();
		return EXIT_FAILURE;
	}

	sponza::LoadMesh(argv[1]);

	glfwSetKeyCallback(window, key_callback);

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	while (!glfwWindowShouldClose(window))
	{
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return EXIT_SUCCESS;
}
