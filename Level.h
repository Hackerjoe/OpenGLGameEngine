//
//  Level.h
//  OpenGL
//
//  Created by Joseph Despain on 5/26/15.
//  Copyright (c) 2015 Joseph Despain. All rights reserved.
//

#ifndef __OpenGL__Level__
#define __OpenGL__Level__

#include "Entity.h"
#include "MyEntity.h"
#include <vector>

class Level
{
public:
    Level();
    ~Level();
    
    void AddEntity(Entity* TheEntity);
    void Start();
    void Update();
    
private:
    std::vector<Entity*> *Entities;
    
};


#endif /* defined(__OpenGL__Level__) */
