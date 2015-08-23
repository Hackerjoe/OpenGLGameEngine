//
//  MyEntity.h
//  OpenGL
//
//  Created by Joseph Despain on 5/24/15.
//  Copyright (c) 2015 Joseph Despain. All rights reserved.
//

#include "Entity.h"
#include "RenderComponent.h"
#include "RigidDynamic.h"
#include "RigidStatic.h"
#include "PxPhysicsAPI.h"
#include "Shader.h"

using namespace physx;

class MyEntity : public Entity
{
public:
	MyEntity(float x, float y, float z, Camera *cCamera, Shader* mShader);
	~MyEntity();

	virtual void Update();
	virtual void Start();

	RenderComponent* MyRenderComp;
	RigidDynamic* MyPhysxComp;
	//RigidStatic* Plane;
};