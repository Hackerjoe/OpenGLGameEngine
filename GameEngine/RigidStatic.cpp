//
//  RigidStatic.cpp
//  OpenGL
//
//  Created by Joseph Despain on 7/4/15.
//  Copyright (c) 2015 Joseph Despain. All rights reserved.
//

#include "RigidStatic.h"

RigidStatic::RigidStatic(PxGeometry Geometry, PxVec3 Position, PxVec3 Scale):Component()
{
    mMaterial = PhysxManager::Instance()->mPhysics->createMaterial(0.5, 0.5, 0.5);
    
    PxTransform pose = PxTransform(PxVec3(Position.x, Position.y, Position.z),PxQuat(PxHalfPi, PxVec3(0.0f, 0.0f, 1.0f)));
    
    PxRigidStatic* plane = PhysxManager::Instance()->mPhysics->createRigidStatic(pose);//mPhysics->createRigidStatic(pose);
    
    PxShape* shape = plane->createShape(PxPlaneGeometry(), *mMaterial);
    
    if (shape == NULL)
        printf("Failed");
    
    PhysxManager::Instance()->GetCurrentScene()->addActor(*plane);//mScene->addActor(*plane);

}

void RigidStatic::Start()
{
    
}

void RigidStatic::Update()
{
    
}

RigidStatic::~RigidStatic()
{
    
}

