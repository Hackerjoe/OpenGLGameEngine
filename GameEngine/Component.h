//
//  Component.h
//  OpenGL
//
//  Created by Joseph Despain on 5/23/15.
//  Copyright (c) 2015 Joseph Despain. All rights reserved.
//
#ifndef COMP_H
#define COMP_H
#pragma once
#include <iostream>
#include "JDMath.h"
#include "Entity.h"


class Entity;

class Component
{
public:

    Component();
    ~Component();

    virtual void Start();
    virtual void Update();
    Entity* Parent;
};


#endif
