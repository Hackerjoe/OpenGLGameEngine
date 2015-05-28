//
//  Vector3.h
//  OpenGL
//
//  Created by Joseph Despain on 5/23/15.
//  Copyright (c) 2015 Joseph Despain. All rights reserved.
//

#ifndef __OpenGL__Vector3__
#define __OpenGL__Vector3__

class Vector3
{
public:
    
    float x;
    float y;
    float z;
    
    Vector3(float x,float y,float z);
    
    bool operator==(const Vector3& other)
    {
        if(this->x == other.x && this->y == other.y && this->z == other.z)
        {
            return true;
        }
        return false;
    }
};

#endif /* defined(__OpenGL__Vector3__) */
