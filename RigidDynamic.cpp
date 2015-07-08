//
//  PhysxComponent.cpp
//  OpenGL
//
//  Created by Joseph Despain on 7/1/15.
//  Copyright (c) 2015 Joseph Despain. All rights reserved.
//

#include "RigidDynamic.h"

RigidDynamic::RigidDynamic(PxGeometry Geometry, PxVec3 Position, PxVec3 Scale) : Component()
{
    PxVec3 dimensions(0.5,0.5,0.5);
    PxBoxGeometry geometry(dimensions);
    mGeometry = &geometry;
    mPositon = &Position;
    mScale = &Scale;
    mMaterial = PhysxManager::Instance()->mPhysics->createMaterial(0.5, 0.5, 0.5);
    PxReal Density = 10.0;
    PxTransform transform(*mPositon, PxQuat::createIdentity());
    
    mActor = PxCreateDynamic(*PhysxManager::Instance()->mPhysics, transform, Geometry, *mMaterial, Density);
    mActor->setAngularDamping(0.75);
    mActor->setLinearVelocity(PxVec3(0,0,0));
    
    
    if(PhysxManager::Instance()->GetCurrentScene() != nullptr)
    {
        PhysxManager::Instance()->GetCurrentScene()->addActor(*mActor);
    }
    else
    {
        std::cerr << "PxScene has not been created.";
    }
    
    
    PxU32 nShapes = mActor->getNbShapes();
    PxShape** shapes=new PxShape*[nShapes];
    mActor->getShapes(shapes, nShapes);
    //mActor->addTorque(PxVec3(4,1,1));
    
    
    PxFilterData filterData;
    filterData.word0 = FilterGroup::eSUBMARINE; // word0 = own ID
    filterData.word1 = FilterGroup::eSUBMARINE;
    
    
    while (nShapes--)
    {
        shapes[nShapes]->setSimulationFilterData(filterData);
    }
    
    delete [] shapes;

}

void RigidDynamic::Start()
{
    
}

void RigidDynamic::Update()
{
    PxU32 nShapes = mActor->getNbShapes();
    PxShape** shapes=new PxShape*[nShapes];
    mActor->getShapes(shapes, nShapes);
    //mActor->addTorque(PxVec3(4,1,1));
    
    
    
    while (nShapes--)
    {
        
    
        PxTransform pT = PxShapeExt::getGlobalPose(*shapes[nShapes], *mActor);

        PxMat33 Quat = PxMat33(pT.q);
        
        Get4x4Matrix(Quat, pT.p, Parent->Matrix);
        
        
    }
    
    delete [] shapes;
    
   
}

void RigidDynamic::Get4x4Matrix(physx::PxMat33 m, physx::PxVec3 t, float *mat)
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

RigidDynamic::~RigidDynamic()
{
    
}