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

int main()
{

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

	sponza::LoadMesh("/home/daniel/Workspace/Sponza/data/sponza/sponza.obj");

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
