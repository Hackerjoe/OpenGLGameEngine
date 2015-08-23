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


PointLightSystem::~PointLightSystem(void)
{

}
