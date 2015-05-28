//
//  MyEntity.cpp
//  OpenGL
//
//  Created by Joseph Despain on 5/24/15.
//  Copyright (c) 2015 Joseph Despain. All rights reserved.
//

#include "MyEntity.h"

MyEntity::MyEntity(float x,float y,float z)
    :Entity(x,y,z)
{
    this->AddComponent(new RenderComponent());
}

MyEntity::~MyEntity()
{
    
}

void MyEntity::Start()
{
    
}

void MyEntity::Update()
{
    //Super Update
    Entity::Update();
}

