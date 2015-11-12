//
//  Vector3.cpp
//  OpenGL
//
//  Created by Joseph Despain on 5/23/15.
//  Copyright (c) 2015 Joseph Despain. All rights reserved.
//

#include "Vector3.h"

Vector3::Vector3(float x,float y,float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
};
Vector3::Vector3()
{
    this->x = 0;
    this->y = 0;
    this->z = 0;
};