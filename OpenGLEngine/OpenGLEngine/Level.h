//
//  Level.h
//  OpenGL
//
//  Created by Joseph Despain on 5/26/15.
//  Copyright (c) 2015 Joseph Despain. All rights reserved.
//

#ifndef __OpenGL__Level__
#define __OpenGL__Level__


//#include "MyEntity2.h"
#include <vector>

#include <PxPhysicsAPI.h>
#include <extensions/PxExtensionsAPI.h>
#include <extensions/PxDefaultErrorCallback.h>
#include <extensions/PxDefaultAllocator.h>
#include <extensions/PxDefaultSimulationFilterShader.h>
#include <extensions/PxDefaultCpuDispatcher.h>
#include <extensions/PxShapeExt.h>
#include <extensions/PxSimpleFactory.h>
#include <foundation/PxMat33.h>
#include <foundation/PxFoundation.h>
#include <PxSimulationEventCallback.h>

#include "PhysxManager.h"
#include "Camera.h"
#include "Entity.h"
#include "MyEntity.h"

using namespace physx;

class Entity;

class Level : public PxSimulationEventCallback
{
public:
	Level(GLuint Width, GLuint Height);
	~Level();

	void AddEntity(Entity* TheEntity);
	void Start();
	void Update();

	////////////////////////////////////////////////////////////

	// For PxSimulationEventCallback

	virtual void							onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs);
	virtual void							onTrigger(PxTriggerPair* pairs, PxU32 count);
	virtual void							onConstraintBreak(PxConstraintInfo*, PxU32) {}
	virtual void							onWake(PxActor**, PxU32) {}
	virtual void							onSleep(PxActor**, PxU32){}

	///////////////////////////////////////////////////////////

	// Physx Scene

	PxScene *mScene;

	void InitPxScene();

	PxSimulationFilterShader gDefaultFilterShader;

	Camera* MainCamera;



private:


	void StepPhysx();



	PxReal PhysxTimeStep;

	std::vector<Entity*> *Entities;

};


#endif /* defined(__OpenGL__Level__) */
