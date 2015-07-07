//
//  MyEntity2.h
//  OpenGL
//
//  Created by Joseph Despain on 7/5/15.
//  Copyright (c) 2015 Joseph Despain. All rights reserved.
//

#ifndef __OpenGL__MyEntity2__
#define __OpenGL__MyEntity2__

#include "Entity.h"
#include "RenderComponent.h"
#include "RigidDynamic.h"
#include "RigidStatic.h"
#include "PxPhysicsAPI.h"
using namespace physx;
class MyEntity2 : public Entity
{
public:
    MyEntity2(float x,float y,float z);
    ~MyEntity2();
    
    virtual void Update();
    virtual void Start();
    
    RenderComponent* MyRenderComp;
    RigidDynamic* MyPhysxComp;
    RigidStatic* Plane;
};

#endif /* defined(__OpenGL__MyEntity2__) */
