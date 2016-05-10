#include "RenderManager.h"

RenderManager* RenderManager::pInstance = nullptr;

RenderManager* RenderManager::Instance()
{
    if(!pInstance)
    {
		pInstance = new RenderManager();
        
    }
    
    return pInstance;
}

RenderManager::RenderManager(void)
{
}

RenderManager::~RenderManager(void)
{
}

void RenderManager::Init()
{
}

void RenderManager::GeometryPass()
{
}

void RenderManager::StencilPass()
{
}

void RenderManager::LightPass()
{
}

void RenderManager::CalcMS()
{

}