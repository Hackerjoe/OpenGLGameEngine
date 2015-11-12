//
//  Component.cpp
//  OpenGL
//
//  Created by Joseph Despain on 5/23/15.
//  Copyright (c) 2015 Joseph Despain. All rights reserved.
//

#include "Component.h"


Component::Component()
{
    Parent = new Entity(0,0,0, nullptr);
}

Component::~Component()
{
    
}

void Component::Start()
{
    
}

void Component::Update()
{

}