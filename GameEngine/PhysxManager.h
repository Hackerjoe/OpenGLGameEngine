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
    
    PxPhysics *mPhysics = NULL;
    PxFoundation *mFoundation = NULL;
    
    void GetPhysxMatrix(PxMat33 m, PxVec3 t, float* mat);
    PxScene* GetCurrentScene();
    void SetCurrentScene(PxScene *Scene);
    
    
private:
    PhysxManager();
    ~PhysxManager();
    
    static PhysxManager* pInstance;
    PxScene* CurrentScene;
    
};

#endif /* defined(__OpenGL__PhysxManager__) */
