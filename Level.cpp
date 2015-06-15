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
}

void Level::AddEntity(Entity *TheEntity)
{
    std::cout << TheEntity->Postion->x;
    Entities->push_back(TheEntity);
}

void Level::onContact(const physx::PxContactPairHeader &pairHeader, const physx::PxContactPair *pairs, PxU32 nbPairs)
{
    
}

void Level::onTrigger(physx::PxTriggerPair *pairs, PxU32 count)
{
    
}

