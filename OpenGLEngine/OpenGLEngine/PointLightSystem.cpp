#include "PointLightSystem.h"


PointLightSystem::PointLightSystem(Shader shader)
{
	this->PointLightShader = shader;
}

void PointLightSystem::StencilPass()
{
	for(auto it = this->PointLights.begin(); it != this->PointLights.end(); it++)
	{
		//Stencil
	}

}

void PointLightSystem::LightPassDraw()
{
	this->PointLightShader.UseShader();

	for(auto it = this->PointLights.begin(); it != this->PointLights.end(); it++)
	{
		//Get point light properties and put them in the shader

		//Draw

	}

}


PointLightSystem::~PointLightSystem(void)
{

}
