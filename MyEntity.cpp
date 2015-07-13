//
//  MyEntity.cpp
//  OpenGL
//
//  Created by Joseph Despain on 5/24/15.
//  Copyright (c) 2015 Joseph Despain. All rights reserved.
//

#include "MyEntity.h"

MyEntity::MyEntity(float x,float y,float z)
    :Entity(x,y,z)
{
    std::string test;
    MyRenderComp = new RenderComponent("simple.vert","simple.frag");
    PxVec3 dimensions(0.5,0.5,0.5);
    PxBoxGeometry geometry(dimensions);
    //PxMaterial* mMaterial = PhysxManager::Instance()->mPhysics->createMaterial(0.5, 0.5, 0.5);//phymPhysics->createMaterial(0.5,0.5,0.5);
    MyPhysxComp = new RigidDynamic(geometry,*new PxVec3(x,y,z),*new PxVec3(0.5,0.5,0.5));
    //Plane = new RigidStatic(geometry,*new PxVec3(0,0,0),*new PxVec3(0.5,0.5,0.5));;
    this->AddComponent(MyRenderComp);
    this->AddComponent(MyPhysxComp);
    
}

MyEntity::~MyEntity()
{
    
}

void MyEntity::Start()
{
    
}

void MyEntity::Update()
{
    //Super Update
    Entity::Update();
    if(HIDManager::Instance()->GetKey('f'))
    {
        MyPhysxComp->mActor->addForce(*new PxVec3(0,100,0));
    }
    if(HIDManager::Instance()->GetKey('g'))
    {
        MyPhysxComp->mActor->addTorque(*new PxVec3(0,0,10));
    }
}

