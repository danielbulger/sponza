#include <light.h>

namespace sponza
{
	void InitialiseLights(sponza::PointLight lights[4])
	{

		lights[0].position = glm::vec4(323.411,218.598,55.6029, 0.0);
		lights[0].ambient = glm::vec4(0.0f, 1.0f, 0.0f, 0.0);

		lights[1].position = glm::vec4(-617.901, 188.135, 143.653, 0.0);
		lights[1].ambient = glm::vec4(1.0f, 0.0f, 0.0f, 0.0);

		lights[2].position = glm::vec4(-36.8992,769.785,34.8048, 0.0);
		lights[2].ambient = glm::vec4(0.0f, 0.0f, 1.0f, 0.0);

		lights[3].position = glm::vec4(211.22,214.578,-56.1841, 0.0);
		lights[3].ambient = glm::vec4(0.5f, 0.3f, 0.2f, 0.0);

		for (size_t i = 0; i < 4; ++i)
		{
			lights[i].diffuse = glm::vec4(0.8f, 0.8f, 0.8f, 0.0);
			lights[i].specular = glm::vec4(1.0f, 1.0f, 1.0f, 0.0);
			lights[i].coef = glm::vec4(1.0f, 0.09, 0.032, 0.0);
		}
	}
}