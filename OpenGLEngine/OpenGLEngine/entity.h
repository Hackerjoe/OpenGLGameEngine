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
#include "Camera.h"

class Component;

class Entity
{
public:

	Entity(float x, float y, float z, Camera* cCamera);
	~Entity();

	virtual void Start();
	virtual void Update();
	void AddComponent(Component* comp);
	Vector3 *Postion;
	float Matrix[16];
	Camera* CurrentCamera;
private:
	std::vector<Component*> *Components;

};

#endif