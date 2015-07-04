//
//  PhysxComponent.h
//  OpenGL
//
//  Created by Joseph Despain on 7/1/15.
//  Copyright (c) 2015 Joseph Despain. All rights reserved.
//

#ifndef __OpenGL__PhysxComponent__
#define __OpenGL__PhysxComponent__
#include <iostream>
#include "Component.h"
#include "PhysxManager.h"
#include "PxPhysicsAPI.h"
#include "Entity.h"

using namespace physx;

class PhysxComponent : public Component
{
public:
    PxMaterial*         mMaterial;
    PxGeometry*         mGeometry;
    PxVec3*             mPositon;
    PxVec3*             mScale;
    
    virtual void Start();
    virtual void Update();
    
    
    PhysxComponent(PxGeometry Geometry, PxVec3 Position, PxVec3 Scale);
    ~PhysxComponent();
    
private:
    
    PxRigidDynamic*     mActor;
    inline void Get4x4Matrix(PxMat33 m, PxVec3 t, float* mat);
};

#endif /* defined(__OpenGL__PhysxComponent__) */
