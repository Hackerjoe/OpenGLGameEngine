
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
	if (!pInstance)
	{
		pInstance = new PhysxManager();
	}

	return pInstance;
}

void PhysxManager::Init()
{
	static PxDefaultErrorCallback gDefaultErrorCallback;
	static PxDefaultAllocator gDefaultAllocatorCallback;

	mFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);


	mPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *mFoundation, PxTolerancesScale());

	if (!PxInitExtensions(*mPhysics))
	{
		printf("PXInitExtensions Failed\n");
	}
	else
	{
		printf("PXInitExtensions Init\n");
	}
	


}

void PhysxManager::SetCurrentScene(physx::PxScene *Scene)
{
	CurrentScene = Scene;
}

PxScene* PhysxManager::GetCurrentScene()
{
	return CurrentScene;
}

void PhysxManager::ShutDown()
{
	mPhysics->release();
	mFoundation->release();
}

void PhysxManager::GetPhysxMatrix(PxMat33 m, PxVec3 t, float* mat)
{

	mat[0] = m.column0[0];
	mat[1] = m.column0[1];
	mat[2] = m.column0[2];
	mat[3] = 0;

	mat[4] = m.column1[0];
	mat[5] = m.column1[1];
	mat[6] = m.column1[2];
	mat[7] = 0;

	mat[8] = m.column2[0];
	mat[9] = m.column2[1];
	mat[10] = m.column2[2];
	mat[11] = 0;

	mat[12] = t[0];
	mat[13] = t[1];
	mat[14] = t[2];
	mat[15] = 1;
}