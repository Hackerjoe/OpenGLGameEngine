//
//  MyEntity.h
//  OpenGL
//
//  Created by Joseph Despain on 5/24/15.
//  Copyright (c) 2015 Joseph Despain. All rights reserved.
//

#include "Entity.h"
#include "RenderComponent.h"
#include "PhysxComponent.h"
#include "PxPhysicsAPI.h"
using namespace physx;
class MyEntity : public Entity
{
public:
    MyEntity(float x,float y,float z);
    ~MyEntity();
    
    virtual void Update();
    virtual void Start();
    
    RenderComponent* MyRenderComp;
    PhysxComponent* MyPhysxComp;
};