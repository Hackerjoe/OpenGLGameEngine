//
//  Entity.cpp
//  OpenGL
//
//  Created by Joseph Despain on 5/23/15.
//  Copyright (c) 2015 Joseph Despain. All rights reserved.
//

#include "Entity.h"

Entity::Entity(float x,float y,float z)
{
    Components = new std::vector<Component*>;
    Postion =  new Vector3(x,y,z);
}

Entity::~Entity()
{
    for (std::vector<Component*>::iterator it = this->Components->begin() ; it != this->Components->end(); ++it)
    {
        // Delete all Components for memory
        delete (*it);
    }
}

void Entity::Start()
{
    
}

void Entity::Update()
{
    //i += 0.001;
    //this->Postion->y = i;
    for (std::vector<Component*>::iterator it = this->Components->begin() ; it != this->Components->end(); ++it)
    {
        
        (*it)->Update();
        
    }
}

void Entity::AddComponent(Component *comp)
{
    comp->Parent = this;
    Components->push_back(comp);
}
