#pragma once
#include "PointLight.h"
#include "Shader.h"
#include "Model.h"
#include "glm\glm.hpp"
#include "Vector3.h"
class PointLightSystem
{
public:
	PointLightSystem(Shader shader);
	~PointLightSystem(void);

	Shader PointLightShader;

	void StencilPass();
	void LightPassDraw();

private:
	std::vector<PointLight*> PointLights;
};

