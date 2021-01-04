#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <cstdlib>

#include <loader.h>
#include "camera.h"
#include "shader.h"

sponza::Camera camera(
	glm::vec3(-3807.26f, 2704.69f, 102.875f),
	glm::vec3(0.0f, 1.0f, 0.0f),
	0.600428f,
	-31.0703f
);

double lastX, lastY;
float deltaTime = 0.0f;
float lastTime = 0.0f;

void key_callback(GLFWwindow *window, int key, int, int action, int)
{
	switch (key)
	{
		case GLFW_KEY_ESCAPE:
			if (action == GLFW_PRESS)
			{
				glfwSetWindowShouldClose(window, GLFW_TRUE);
			}
			break;

		case GLFW_KEY_W:
			camera.move(sponza::CameraMovement::FORWARD, deltaTime);
			break;

		case GLFW_KEY_S:
			camera.move(sponza::CameraMovement::BACKWARD, deltaTime);
			break;

		case GLFW_KEY_A:
			camera.move(sponza::CameraMovement::LEFT, deltaTime);
			break;

		case GLFW_KEY_D:
			camera.move(sponza::CameraMovement::RIGHT, deltaTime);
			break;

		default:
			break;
	}
}

void mouse_callback(GLFWwindow *, double x, double y)
{
	const double xOffset = x - lastX;

	// Flip the Y to match OpenGL screen space.
	const double yOffset = lastY - y;

	camera.rotate(
		static_cast<float>(xOffset), static_cast<float>(yOffset)
	);

	lastX = x;
	lastY = y;
}

int main(int argc, char *argv[])
{

	if (argc != 2)
	{
		return EXIT_FAILURE;
	};

	if (!glfwInit())
	{
		return EXIT_FAILURE;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	if (!mode)
	{
		return EXIT_FAILURE;
	}

	// Center the mouse position into the screen.
	lastX = mode->width / 2.0;
	lastY = mode->height / 2.0;

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

	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	gladLoadGL();

	sponza::Shader shader(
		"/home/daniel/Workspace/Sponza/data/render.vert",
		"/home/daniel/Workspace/Sponza/data/render.frag"
	);

	std::vector<sponza::Mesh> meshes = sponza::LoadMesh(argv[1]);

	for (auto &mesh : meshes)
	{
		sponza::InitialiseMesh(mesh);
	}

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glViewport(0, 0, mode->width, mode->height);
	glEnable(GL_DEPTH_TEST);

	const float aspect = static_cast<float>(mode->width) / static_cast<float>(mode->height);

	const glm::mat4 projection = glm::perspective(
		glm::radians(45.0f),
		aspect,
		0.1f,
		10000.0f
	);

	while (!glfwWindowShouldClose(window))
	{
		const auto currentTime = static_cast<float>(glfwGetTime());
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.use();

		shader.setMat4("projection", projection);
		shader.setMat4("view", camera.getViewMatrix());
		shader.setMat4("model", glm::mat4(1.0f));

		for (const auto &mesh : meshes)
		{
			shader.setVec3("ambient", mesh.material->ambient);
			shader.setVec3("diffuse", mesh.material->diffuse);
			shader.setVec3("specular", mesh.material->specular);
			shader.setFloat("specular_exponent", mesh.material->specular_exponent);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, mesh.material->ambient_texture.id);
			shader.setInt("ambientTexture", 0);

			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, mesh.material->diffuse_texture.id);
			shader.setInt("diffuseTexture", 1);

			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, mesh.material->specular_texture.id);
			shader.setInt("specularTexture", 2);

			glBindVertexArray(mesh.vao_id);

			glDrawArrays(GL_TRIANGLES, 0, mesh.vertices.size());

//			glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, nullptr);

			glBindVertexArray(0);

			glBindTexture(GL_TEXTURE_2D, 0);

			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, 0);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	// Cleanup all the OpenGL buffers.
	for (auto &mesh : meshes)
	{
		sponza::CleanupMesh(mesh);
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return EXIT_SUCCESS;
}
