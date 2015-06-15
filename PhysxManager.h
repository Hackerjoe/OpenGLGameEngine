//
//  PhysxManager.h
//  OpenGL
//
//  Created by Joseph Despain on 6/15/15.
//  Copyright (c) 2015 Joseph Despain. All rights reserved.
//

#ifndef __OpenGL__PhysxManager__
#define __OpenGL__PhysxManager__

#include "PxPhysicsAPI.h"
#include "extensions/PxExtensionsAPI.h"
#include "extensions/PxDefaultErrorCallback.h"
#include "extensions/PxDefaultAllocator.h"
#include "extensions/PxDefaultSimulationFilterShader.h"
#include "extensions/PxDefaultCpuDispatcher.h"
#include "extensions/PxShapeExt.h"
#include "extensions/PxSimpleFactory.h"
#include "foundation/PxMat33.h"
#include "foundation/PxFoundation.h"
#include "PxSimulationEventCallback.h"


using namespace physx;




class PhysxManager
{
public:
    static PhysxManager* Instance();
    void Init();
    void ShutDown();
    
    
private:
    PhysxManager();
    ~PhysxManager();
    
    PxPhysics *mPhysics = NULL;
    PxFoundation *mFoundation = NULL;
    
    
    
    static PhysxManager* pInstance;
    
};

#endif /* defined(__OpenGL__PhysxManager__) */
