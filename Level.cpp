//
//  Level.cpp
//  OpenGL
//
//  Created by Joseph Despain on 5/26/15.
//  Copyright (c) 2015 Joseph Despain. All rights reserved.
//

#include "Level.h"


Level::Level():PxSimulationEventCallback()
{
    InitPxScene();
    Entities = new std::vector<Entity*>;
    MyEntity* hi = new MyEntity(0,0,0);
    AddEntity(hi);
    
    AddEntity(new MyEntity(1,0,0));
    
}

Level::~Level()
{
}

void Level::Start()
{
    
}

void Level::Update()
{
    for (std::vector<Entity*>::iterator it = this->Entities->begin() ; it != this->Entities->end(); ++it)
    {
        (*it)->Update();
    }
    StepPhysx();
}

void Level::AddEntity(Entity *TheEntity)
{
    
    Entities->push_back(TheEntity);
}

void Level::onContact(const physx::PxContactPairHeader &pairHeader, const physx::PxContactPair *pairs, PxU32 nbPairs)
{
    
}

void Level::onTrigger(physx::PxTriggerPair *pairs, PxU32 count)
{
    
}

void Level::InitPxScene()
{
    
    
    PxSceneDesc sceneDesc(PhysxManager::Instance()->mPhysics->getTolerancesScale());
    sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
    
    if(!sceneDesc.cpuDispatcher) {
        
        PxDefaultCpuDispatcher* mCpuDispatcher = PxDefaultCpuDispatcherCreate(1);
        
        if(!mCpuDispatcher)
            std::cerr <<"PxDefaultCpuDispatcherCreate failed!"<<std::endl;
        
        sceneDesc.cpuDispatcher = mCpuDispatcher;
    }
    
    if(!sceneDesc.filterShader)
        sceneDesc.filterShader  = gDefaultFilterShader;
    
    mScene = PhysxManager::Instance()->mPhysics->createScene(sceneDesc);
    
    if (mScene == NULL) {
        std::cerr << "Scene was not created" << std::endl;
    }
    
    mScene->setVisualizationParameter(PxVisualizationParameter::eSCALE,     1.0);
    mScene->setVisualizationParameter(PxVisualizationParameter::eCOLLISION_SHAPES, 1.0f);
    

    
    
    
}

void Level::StepPhysx()
{
    mScene->simulate(PhysxTimeStep);
    
    while(!mScene->fetchResults())
    {
        
    }
     
}
