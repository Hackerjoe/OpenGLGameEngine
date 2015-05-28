//
//  Program.cpp
//  OpenGL
//
//  Created by Joseph Despain on 5/23/15.
//  Copyright (c) 2015 Joseph Despain. All rights reserved.
//

#include "Program.h"
#include <iostream>
Program::Program()
{
    ScreenWidth = 640;
    ScreenHeight = 480;
    TheLevel = new Level();
}

bool Program::Init(int argc, char** argv)
{
    //Init Glut and a window
    glutInit(&argc, argv);
    //glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(ScreenWidth,ScreenHeight);
    glutCreateWindow("My Engine");
    //glutGameModeString("1920x1200:32");
    glEnable(GL_TEXTURE_2D);
    glClearColor (0.0, 1.0, 1.0, 1.0);
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, 640, 480   );
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.,(GLfloat) ScreenHeight/(GLfloat) ScreenWidth,0.1, 800.0);
    gluLookAt(0.0,0.0,5.0, 0.0, 0.0, 0.0, 0,1,0);
    
    //Init devil for image reading
    /*ilutRenderer(ILUT_OPENGL);
    ilInit();
    iluInit();
    ilutInit();
    ilutRenderer(ILUT_OPENGL);*/
    
    //Setup glut callbacks
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyb);
    
    printf("\n%s\n", glGetString(GL_VERSION));
    
    //Check for support
    glewInit();
    
    if (glewIsSupported("GL_VERSION_2_1"))
        printf("Ready for OpenGL 2.1\n");
    else {
        printf("OpenGL 2.1 not supported\n");
        return false;
    }
    
    if (GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader)
        printf("Ready for GLSL - vertex, fragment\n");
    else {
        printf("Not Supported :(");
        return false;
    }
    
        
    return true;
}


void Program::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    TheLevel->Update();
    glutSwapBuffers();
}

void Program::reshape(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.,(GLfloat) width/(GLfloat) height,0.1, 800.0);
    gluLookAt(0.0,0.0,5.0, 0.0, 0.0, 0.0, 0,1,0);
    glMatrixMode(GL_MODELVIEW);
}

void Program::keyb(unsigned char key, int x, int y)
{
    switch (key){
        case 'a':
            glutFullScreen();
            break;
        case 'q':
            glutReshapeWindow(640, 480);
            glutPositionWindow(100, 100);
            break;
        case '`':
            //glutLeaveMainLoop();
            break;
    }
}

void Program::SetLevel(Level* level)
{
    TheLevel = level;
}

Program::~Program()
{
    //glutLeaveMainLoop();
}
