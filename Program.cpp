//
//  Program.cpp
//  OpenGL
//
//  Created by Joseph Despain on 5/23/15.
//  Copyright (c) 2015 Joseph Despain. All rights reserved.
//



#include "Program.h"
#include <iostream>
#include "HIDManager.h"





Program::Program()
{
    ScreenWidth = 960;
    ScreenHeight = 540;
    MainCamera = new Camera(ScreenWidth,ScreenHeight);
    
    
}

float Program::testx = 0;
float Program::testy = 0;
float Program::testz = 5;



bool Program::Init(int argc, char** argv)
{
    
    if (!glfwInit())
        return false;
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    
    
    window = glfwCreateWindow(ScreenWidth, ScreenHeight, "Engine", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    

    //glEnable(GL_TEXTURE_2D);
    glewExperimental = GL_TRUE;
    glewInit();
    
    std::cout << glGetString(GL_VERSION) << std::endl;
    glEnable(GL_DEPTH_TEST);
    
    //Init devil for image reading
    ImageLibManager::Instance()->Init();
    PhysxManager::Instance()->Init();

    int width, height;
    glfwGetFramebufferSize(window, &ScreenWidth, &ScreenHeight);
    
    //glViewport(0, 0, width, height);
    glViewport(0, 0, ScreenWidth, ScreenHeight);
    cout << "Framebuffer size" << ScreenWidth << ScreenHeight << endl;
    
    
    TheLevel = new Level(ScreenWidth,ScreenHeight);
    shaderGeometryPass = new Shader("gbuffer.vert", "gbuffer.frag");
    shaderLightingPass = new Shader("deferredlighting.vert", "deferredlighting.frag");
    shader = new Shader("simple.vert","simple.frag");
    
    shaderLightingPass->UseShader();
    glUniform1i(glGetUniformLocation(shaderLightingPass->GetShader(), "gPosition"), 0);
    glUniform1i(glGetUniformLocation(shaderLightingPass->GetShader(), "gNormal"), 1);
    glUniform1i(glGetUniformLocation(shaderLightingPass->GetShader(), "gAlbedoSpec"), 2);
    
    // Load models

    Nanosuit = new Model("Nanosuit/nanosuit.obj");
    SphereModel = new Model("sphere.obj");

    

   
    // - Colors
    const GLuint NR_LIGHTS = 20;
    srand(13);
    for (GLuint i = 0; i < NR_LIGHTS; i++)
    {
        // Calculate slightly random offsets
        GLfloat xPos = ((rand() % 100) / 100.0) * 6.0 - 3.0;
        GLfloat yPos = ((rand() % 100) / 100.0) * 6.0 - 4.0;
        GLfloat zPos = ((rand() % 100) / 100.0) * 6.0 - 3.0;
        
        //lightPositions.push_back(glm::vec3(xPos, yPos, zPos));
        // Also calculate random color
        GLfloat rColor = ((rand() % 100) / 200.0f) + 0.5; // Between 0.5 and 1.0
        GLfloat gColor = ((rand() % 100) / 200.0f) + 0.5; // Between 0.5 and 1.0
        GLfloat bColor = ((rand() % 100) / 200.0f) + 0.5; // Between 0.5 and 1.0
        //lightColors.push_back(glm::vec3(rColor, gColor, bColor));
        PointLight newPointLight;
        newPointLight.Position= glm::vec3(xPos,yPos,zPos);
        newPointLight.Color = glm::vec3(rColor,gColor,bColor);
        PointLights.push_back(newPointLight);
    }
    
    // Set up G-Buffer
    // 3 textures:
    // 1. Positions (RGB)
    // 2. Color (RGB) + Specular (A)
    // 3. Normals (RGB)
  
    glGenFramebuffers(1, &gBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
    
    // - Position color buffer
    glGenTextures(1, &gPosition);
    glBindTexture(GL_TEXTURE_2D, gPosition);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, ScreenWidth, ScreenHeight, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);
    // - Normal color buffer
    glGenTextures(1, &gNormal);
    glBindTexture(GL_TEXTURE_2D, gNormal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, ScreenWidth, ScreenHeight, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);
    // - Color + Specular color buffer
    glGenTextures(1, &gAlbedoSpec);
    glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ScreenWidth, ScreenHeight, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedoSpec, 0);
    
    glGenTextures(1, &FinalTexture);
    glBindTexture(GL_TEXTURE_2D, FinalTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ScreenWidth, ScreenHeight, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, FinalTexture, 0);
    
    
    
    // - Tell OpenGL which color attachments we'll use (of this framebuffer) for rendering
    //GLuint attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
    //glDrawBuffers(3, attachments);
    
    
    // - Create and attach depth buffer (renderbuffer)
    GLuint rboDepth;
    glGenRenderbuffers(1, &rboDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH32F_STENCIL8, ScreenWidth, ScreenHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
    // - Finally check if framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    
    mainLoop();
    
        
    return true;
}

void Program::mainLoop()
{
    NBFrames = 0;
    LastTime = glfwGetTime();
    while (!glfwWindowShouldClose(window))
    {
        render();
    }
}

void Program::render()
{
    glEnable(GL_STENCIL_TEST);

    GeoPass();
    
    StencilPass();
    
    LightPass();
    
    glDisable(GL_STENCIL_TEST);
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer);
    glReadBuffer(GL_COLOR_ATTACHMENT3);
    
    glBlitFramebuffer(0, 0, ScreenWidth, ScreenHeight,
                      0, 0, ScreenWidth, ScreenHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);
    
    glfwSwapInterval(0);
    glfwSwapBuffers(window);
    
    glfwPollEvents();
    
    //Calc how long to calculate frame
    CalcMS();
    
    
}

void Program::GeoPass()
{
    //Bind gBuffer
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, gBuffer);
    glDrawBuffer(GL_COLOR_ATTACHMENT3);
    glClear(GL_COLOR_BUFFER_BIT);
    
    GLuint attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
    glDrawBuffers(3, attachments);
    
    glDepthMask(GL_TRUE);
    // Clear Buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glEnable(GL_DEPTH_TEST);
    
    
    //Create Projection and View
    glm::mat4 projection = glm::perspective(45.0f, (float)ScreenWidth/(float)ScreenHeight, 0.1f, 100.0f);
    glm::mat4 view = glm::lookAt(glm::vec3(testx, 2.5, 4), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    
    
    //Use ShaderGeo shader to draw into gBuffer
    shaderGeometryPass->UseShader();
    
    // Create Model 4x4Matix for nanosuit model
    glm::mat4 model;
    model = glm::translate(model, glm::vec3(0.0f, -1.75, 0.0f)); // Translate it down a bit so it's at the center of the scene
    model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// It's a bit too big for our scene, so scale it down
    
    // Insert values into shader progam
    glUniformMatrix4fv(glGetUniformLocation(shaderGeometryPass->GetShader(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(shaderGeometryPass->GetShader(), "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaderGeometryPass->GetShader(), "model"), 1, GL_FALSE, glm::value_ptr(model));
    
    
    //Draw into model into gBuffer
    Nanosuit->Draw(*shaderGeometryPass);
    
    glDepthMask(GL_FALSE);
    
    //Unbind gBuffer
    //glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Program::StencilPass()
{
    glDrawBuffer(GL_NONE);
    
    glEnable(GL_DEPTH_TEST);
    
    glDisable(GL_CULL_FACE);
    
    glClear(GL_STENCIL_BUFFER_BIT);
    
    // We need the stencil test to be enabled but we want it
    // to succeed always. Only the depth test matters.
    glStencilFunc(GL_ALWAYS, 0, 0);
    
    glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
    glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);
    
    glm::mat4 projection = glm::perspective(45.0f, (float)ScreenWidth/(float)ScreenHeight, 0.1f, 100.0f);
    glm::mat4 view = glm::lookAt(glm::vec3(testx, 2.5, 4), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    
    glm::mat4 model;
    model = glm::translate(model, glm::vec3(0.0f, -1.75, 0.0f)); // Translate it down a bit so it's at the center of the scene
    model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));	// It's a bit too big for our scene, so scale it down
    
    shader->UseShader();
    
    // Insert values into shader progam
    glUniformMatrix4fv(glGetUniformLocation(shader->GetShader(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(shader->GetShader(), "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shader->GetShader(), "model"), 1, GL_FALSE, glm::value_ptr(model));

    SphereModel->Draw(*shader);
    
}

void Program::LightPass()
{
    glDrawBuffer(GL_COLOR_ATTACHMENT3);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gPosition);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, gNormal);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
    
    glStencilFunc(GL_NOTEQUAL, 0, 0xFF);
    
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_ONE, GL_ONE);
    
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    
    glm::mat4 projection = glm::perspective(45.0f, (float)ScreenWidth/(float)ScreenHeight, 0.1f, 100.0f);
    glm::mat4 view = glm::lookAt(glm::vec3(testx, 2.5, 4), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    
    glm::mat4 model;
    model = glm::translate(model, glm::vec3(0.0f, -1.75, 0.0f)); // Translate it down a bit so it's at the center of the scene
    model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));	// It's a bit too big for our scene, so scale it down
    
    shaderLightingPass->UseShader();
    
    // Insert values into shader progam
    glUniformMatrix4fv(glGetUniformLocation(shaderLightingPass->GetShader(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(shaderLightingPass->GetShader(), "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaderLightingPass->GetShader(), "model"), 1, GL_FALSE, glm::value_ptr(model));
    
    SphereModel->Draw(*shaderLightingPass);
    
    glCullFace(GL_BACK);
    
    glDisable(GL_BLEND);
}

void Program::RenderQuad()
{
    if (quadVAO == 0)
    {
        GLfloat quadVertices[] = {
            // Positions        // Texture Coords
            -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
            1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
            1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        // Setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

void Program::CalcMS()
{
    CurrentTime = glfwGetTime();
    NBFrames++;
    if ( CurrentTime - LastTime >= 1.0 ){ // If last prinf() was more than 1 sec ago
        // printf and reset timer
        printf("%f ms/frame\n", 1000/double(NBFrames));
        NBFrames = 0;
        LastTime += 1.0;
    }
}

void Program::reshape(GLFWwindow* window, int width, int height)
{
    
}

void Program::SetLevel(Level* level)
{
    TheLevel = level;
}

Program::~Program()
{
    glfwTerminate();
}
