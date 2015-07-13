//
//  MyEntity2.cpp
//  OpenGL
//
//  Created by Joseph Despain on 7/5/15.
//  Copyright (c) 2015 Joseph Despain. All rights reserved.
//

#include "MyEntity2.h"


#include "MyEntity.h"

MyEntity2::MyEntity2(float x,float y,float z)
:Entity(x,y,z)
{
    
    MyRenderComp = new RenderComponent("simple2.vert","simple2.frag");
    MyRenderComp->setDiffuseTexture(MyRenderComp->Program,ImageLibManager::Instance()->loadImage("test.png"));
    PxVec3 dimensions(0.5,0.5,0.5);
    PxBoxGeometry geometry(dimensions);
    PxMaterial* mMaterial = PhysxManager::Instance()->mPhysics->createMaterial(0.5, 0.5, 0.5);//phymPhysics->createMaterial(0.5,0.5,0.5);
    MyPhysxComp = new RigidDynamic(geometry,*new PxVec3(x,y,z),*new PxVec3(0.5,0.5,0.5));
    
    this->AddComponent(MyRenderComp);

    //this->AddComponent(MyPhysxComp);
    
}

MyEntity2::~MyEntity2()
{
    
}

void MyEntity2::Start()
{
    
}

void MyEntity2::Update()
{
    //Super Update
    Entity::Update();
    
   /* if(HIDManager::Instance()->GetKey('f'))
    {
        MyPhysxComp->mActor->addForce(*new PxVec3(0,100,0));
    }
    if(HIDManager::Instance()->GetKey('g'))
    {
        MyPhysxComp->mActor->addTorque(*new PxVec3(0,0,10));
    }*/
}