//
//  Level.cpp
//  OpenGL
//
//  Created by Joseph Despain on 5/26/15.
//  Copyright (c) 2015 Joseph Despain. All rights reserved.
//

#include "Level.h"
#include <glm/glm.hpp>
#include "glm/ext.hpp"
#include "Shader.h"

Level::Level(GLuint Width, GLuint Height):PxSimulationEventCallback()
{
    MainCamera = new Camera(Width,Height);
    
    InitPxScene();
    /*Entities = new std::vector<Entity*>;
    
    PxVec3 dimensions(1,1,1);
    PxBoxGeometry geometry(dimensions);
    Entity* Plane = new Entity(0,0,0, MainCamera);
    Plane->AddComponent(new RigidStatic(geometry,*new PxVec3(0,-1,0),*new PxVec3(0.5,0.5,0.5)));
    //AddEntity(Plane);
    Shader* mShader = new Shader("simple.vert","simple.frag");
    for(int i = 0; i<3000; i++)
    {
        MyEntity* hi = new MyEntity(0,i,0, MainCamera,mShader);
        AddEntity(hi);
    }*/
}

Level::~Level()
{
}

void Level::Start()
{
    
}

void Level::Update()
{
    MainCamera->Update();
    //std::cout<< glm::to_string(MainCamera->View) << std::endl;
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
    //std::cout << "CONTACT BITCHES!!!";
    
}

void Level::onTrigger(physx::PxTriggerPair *pairs, PxU32 count)
{
    //std::cout << "CONTACT BITCHES";
}

PxFilterFlags LevelFilterShader(PxFilterObjectAttributes attributes0, PxFilterData filterData0,
                                PxFilterObjectAttributes attributes1, PxFilterData filterData1,
                                PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
{
    // let triggers through
    if(PxFilterObjectIsTrigger(attributes0) || PxFilterObjectIsTrigger(attributes1))
    {
        pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
        return PxFilterFlag::eDEFAULT;
    }
    // generate contacts for all that were not filtered above
    pairFlags = PxPairFlag::eCONTACT_DEFAULT;
    
    // trigger the contact callback for pairs (A,B) where
    // the filtermask of A contains the ID of B and vice versa.
    if((filterData0.word0 & filterData1.word1) && (filterData1.word0 & filterData0.word1))
        pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND;
    
    return PxFilterFlag::eDEFAULT;
}

void Level::InitPxScene()
{
    
    
    PxSceneDesc sceneDesc(PhysxManager::Instance()->mPhysics->getTolerancesScale());
    sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
    
    if(!sceneDesc.cpuDispatcher)
    {
        
        PxDefaultCpuDispatcher* mCpuDispatcher = PxDefaultCpuDispatcherCreate(4);
        
        if(!mCpuDispatcher)
            std::cerr <<"PxDefaultCpuDispatcherCreate failed!"<<std::endl;
        
        sceneDesc.cpuDispatcher = mCpuDispatcher;
    }

    
    sceneDesc.filterShader  = LevelFilterShader;
    sceneDesc.simulationEventCallback	= this;
    
    mScene = PhysxManager::Instance()->mPhysics->createScene(sceneDesc);
    
    
    if (mScene == NULL)
    {
        std::cerr << "Scene was not created" << std::endl;
    }
    
    
    PhysxManager::Instance()->SetCurrentScene(mScene);
    
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
