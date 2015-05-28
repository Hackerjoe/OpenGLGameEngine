//
//  Entity.h
//  OpenGL
//
//  Created by Joseph Despain on 5/23/15.
//  Copyright (c) 2015 Joseph Despain. All rights reserved.
//


#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include "Component.h"
#include "JDMath.h"
#include <iostream>


class Component;

class Entity
{
public:
    
    Entity(float x,float y,float z);
    ~Entity();
    
    virtual void Start();
    virtual void Update();
    void AddComponent(Component* comp);
    Vector3 *Postion;
    float i = 0;
private:
    std::vector<Component*> *Components;

};

#endif