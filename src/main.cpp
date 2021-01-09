#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <cstdlib>

#include <loader.h>
#include "camera.h"
#include "shader.h"
#include "skybox.h"
#include "resource.h"
#include "light.h"
#include "hdr.h"
#include "uniforms.h"
#include "shadow_map.h"

sponza::Camera camera(
	glm::vec3(-651.053f, 619.491f, 7.56791f),
	glm::vec3(0.0f, 1.0f, 0.0f),
	0.600428f,
	-31.0703f
);

double lastX, lastY;
float deltaTime = 0.0f;
float lastTime = 0.0f;

void RenderDepthPass(const std::vector<sponza::Mesh> &meshes)
{
	for (const sponza::Mesh &mesh : meshes)
	{
		sponza::DrawMesh(mesh);
	}
}

void RenderScene(
	const std::vector<sponza::Mesh> &meshes,
	const sponza::PointLight &light,
	int depthTexture,
	float farPlane
)
{

	uint32_t currentShader = 0;
	sponza::Material *currentMaterial = nullptr;

	for (const sponza::Mesh &mesh : meshes)
	{
		sponza::Render(
			mesh,
			light,
			depthTexture,
			farPlane,
			mesh.material.get() != currentMaterial,
			mesh.shader.ID != currentShader
		);

		currentMaterial = mesh.material.get();
		currentShader = mesh.shader.ID;
	}
}

void RenderLoop(GLFWwindow *window, const GLFWvidmode *mode, const char *file);

void KeyCallback(GLFWwindow *window, int key, int, int action, int);

void MouseCallback(GLFWwindow *, double x, double y);

int main(int argc, char *argv[])
{

	if (argc != 2 || !glfwInit())
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

	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	gladLoadGL();

	RenderLoop(window, mode, argv[1]);

	glfwDestroyWindow(window);
	glfwTerminate();

	return EXIT_SUCCESS;
}

void RenderLoop(GLFWwindow *window, const GLFWvidmode *mode, const char *file)
{

	sponza::Resource resource(file);

	sponza::Shader shader(
		resource.getFilePath("/render.vert").c_str(),
		resource.getFilePath("/render.frag").c_str()
	);

	sponza::Shader normalMapShader(
		resource.getFilePath("/normal_map_render.vert").c_str(),
		resource.getFilePath("/normal_map_render.frag").c_str()
	);

	sponza::Skybox skybox;
	skybox.load(resource);

	std::vector<sponza::Mesh> meshes = sponza::LoadMesh(resource, "/sponza/sponza.obj");

	for (auto &mesh : meshes)
	{
		sponza::InitialiseMesh(mesh, shader, normalMapShader);
	}

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glViewport(0, 0, mode->width, mode->height);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);

	const float aspect = static_cast<float>(mode->width) / static_cast<float>(mode->height);

	camera.updateProjectionMatrix(60.0f, aspect, 0.1f, 10000.0f);

	sponza::PointLight lights[2];
	sponza::InitialiseLights(lights);

	sponza::Uniforms uniforms(shader.ID, normalMapShader.ID, camera);

	sponza::HDR hdr(mode->width, mode->height);
	hdr.load(resource);

	sponza::ShadowMap shadowMap(1024, 1024);
	shadowMap.load(resource);
	shadowMap.updateProjectionMatrix(camera.getNearPlane(), camera.getFarPlane());

	while (!glfwWindowShouldClose(window))
	{
		const auto currentTime = static_cast<float>(glfwGetTime());
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		uniforms.update(camera);

		bool firstPass = true;

		for (const sponza::PointLight &light : lights)
		{
			shadowMap.bindForWrite(light, camera.getFarPlane());

			RenderDepthPass(meshes);

			// Restore the HDR Buffer as the destination
			hdr.bindForWrite(mode->width, mode->height, firstPass);

			RenderScene(meshes, light, shadowMap.getTextureId(), camera.getFarPlane());

			firstPass = false;
		}

		for (int i = 0; i < 5; ++i)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		skybox.render(camera, camera.getProjectionMatrix());

		hdr.render();

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	// Cleanup all the OpenGL buffers.
	for (auto &mesh : meshes)
	{
		sponza::CleanupMesh(mesh);
	}
}

void KeyCallback(GLFWwindow *window, int key, int, int action, int)
{

	if (action == GLFW_RELEASE)
	{
		return;
	}

	switch (key)
	{
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, GLFW_TRUE);
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

		case GLFW_KEY_P:
			camera.print();
			break;

		default:
			break;
	}
}

void MouseCallback(GLFWwindow *, double x, double y)
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