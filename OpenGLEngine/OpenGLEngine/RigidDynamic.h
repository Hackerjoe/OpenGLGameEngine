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

struct FilterGroup
{
	enum Enum
	{
		eSUBMARINE = (1 << 0),
		eMINE_HEAD = (1 << 1),
		eMINE_LINK = (1 << 2),
		eCRAB = (1 << 3),
		eHEIGHTFIELD = (1 << 4),
	};
};

class RigidDynamic : public Component
{
public:
	PxMaterial*         mMaterial;
	PxGeometry*         mGeometry;
	PxVec3*             mPositon;
	PxVec3*             mScale;

	virtual void Start();
	virtual void Update();
	PxRigidDynamic*     mActor;

	RigidDynamic(PxGeometry Geometry, PxVec3 Position, PxVec3 Scale);
	~RigidDynamic();

private:


	inline void Get4x4Matrix(PxMat33 m, PxVec3 t, float* mat);
};

#endif /* defined(__OpenGL__PhysxComponent__) */