//
//  Camera.h
//  OpenGL
//
//  Created by Joseph Despain on 7/21/15.
//  Copyright (c) 2015 Joseph Despain. All rights reserved.
//

#ifndef __OpenGL__Camera__
#define __OpenGL__Camera__

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>

class Camera
{
public:
    Camera(const GLuint width, const GLuint height);
    ~Camera();
    
    void Update();
    
    glm::vec3 CameraPosition;
    glm::mat4 View;
    glm::mat4 Projection;
    
private:
    GLuint Width;
    GLuint Height;

};

#endif /* defined(__OpenGL__Camera__) */
