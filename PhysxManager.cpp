//
//  PhysxManager.cpp
//  OpenGL
//
//  Created by Joseph Despain on 6/15/15.
//  Copyright (c) 2015 Joseph Despain. All rights reserved.
//

#include "PhysxManager.h"

PhysxManager* PhysxManager::pInstance = NULL;

PhysxManager::PhysxManager()
{
    
}

PhysxManager::~PhysxManager()
{
    
}

PhysxManager* PhysxManager::Instance()
{
    if(!pInstance)
    {
        pInstance = new PhysxManager();
    }
    
    return pInstance;
}

void PhysxManager::Init()
{
    
    static PxDefaultErrorCallback gDefaultErrorCallback;
    static PxDefaultAllocator gDefaultAllocatorCallback;
    mFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback,
                                     gDefaultErrorCallback);
    if(!mFoundation)
    {
    
    }
    
    
    mPhysics = PxCreatePhysics( PX_PHYSICS_VERSION,*mFoundation,PxTolerancesScale());
    
    if(!PxInitExtensions(*mPhysics))
    {
        printf("PXInitExtensions Failed\n");
    }
    else
    {
        printf("PXInitExtensions Init\n");
    }
    
    
    
}

void PhysxManager::ShutDown()
{
    
}