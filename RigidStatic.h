//
//  RigidStatic.h
//  OpenGL
//
//  Created by Joseph Despain on 7/4/15.
//  Copyright (c) 2015 Joseph Despain. All rights reserved.
//

#ifndef __OpenGL__RigidStatic__
#define __OpenGL__RigidStatic__

#include "PxPhysicsAPI.h"
#include "PhysxManager.h"
#include "Component.h"

using namespace physx;

class RigidStatic : public Component
{
public:

	PxMaterial*         mMaterial;
	PxGeometry*         mGeometry;
	PxVec3*             mPositon;
	PxVec3*             mScale;

	PxRigidStatic* mActor;

	virtual void Start();
	virtual void Update();

	RigidStatic(PxGeometry Geometry, PxVec3 Position, PxVec3 Scale);
	~RigidStatic();
private:
};


#endif /* defined(__OpenGL__RigidStatic__) */