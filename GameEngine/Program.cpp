//
//  Program.cpp
//  OpenGL
//
//  Created by Joseph Despain on 5/23/15.
//  Copyright (c) 2015 Joseph Despain. All rights reserved.
//



#include "Program.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>



Program::Program()
{
    ScreenWidth = 1440;
    ScreenHeight = 900;
    testx = 0;
    testy = 0;
}




bool Program::Init(int argc, char** argv)
{
    
    if (!glfwInit())
        return false;
    
    
    // Create GL Context 3.0 and up.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    
    // Create GLFW Window.
    window = glfwCreateWindow(ScreenWidth, ScreenHeight, "Engine", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    

    //Init GL Extension Wrangler.
    glewExperimental = GL_TRUE;
    glewInit();
    
    //Print Out what version of GL im using.
    std::cout << glGetString(GL_VERSION) << std::endl;
    
    
    //Init devil for image reading
    ImageLibManager::Instance()->Init();
    
    //Init Physx Singleton
    PhysxManager::Instance()->Init();
    
    /*
     * We have to get the glfw framebuffer size because
     * I am using a macbook pro with a high DPI screen.
     * The frame buffer gets how many pixels is actually
     * in our window. We change the screen width and height
     * varibles to what is returned out.
     */
    
    
    glfwGetFramebufferSize(window, &ScreenWidth, &ScreenHeight);
    
    // Set the view port to the new screen width and height.
    glViewport(0, 0, ScreenWidth, ScreenHeight);
    
    //cout << "Framebuffer size" << ScreenWidth << ScreenHeight << endl;
    
    
    //TheLevel = new Level(ScreenWidth,ScreenHeight);
    
    //Now we read and compile our shaders for our deferred renderer.
    shaderGeometryPass = new Shader("gbuffer.vert", "gbuffer.frag");
    shaderLightingPass = new Shader("deferredlighting.vert", "deferredlighting.frag");
    stencilPassShader = new Shader("simple.vert","simple.frag");
    
    // Set up the lighting shader to read from the color attachments in the FBO.
    shaderLightingPass->UseShader();
    glUniform1i(glGetUniformLocation(shaderLightingPass->GetShader(), "gPosition"), 0);
    glUniform1i(glGetUniformLocation(shaderLightingPass->GetShader(), "gNormal"), 1);
    glUniform1i(glGetUniformLocation(shaderLightingPass->GetShader(), "gAlbedoSpec"), 2);
    glUniform1i(glGetUniformLocation(shaderLightingPass->GetShader(), "Panorama"), 3);
    
    //Load models using assimp.
    // Load Nanosuit model (Nanosuit model is from Crysis 2).
    Nanosuit = new Model("Nanosuit/nanosuit.obj");
    
    // Load sphere model for the point lights.
    SphereModel = new Model("sphere.obj");

    //Create G-Buffer FBO.
  
    // Generate gBuffer and bind it for drawing.
    glGenFramebuffers(1, &gBuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, gBuffer);
    
    // - Position buffer
    glGenTextures(1, &gPosition);
    glBindTexture(GL_TEXTURE_2D, gPosition);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, ScreenWidth, ScreenHeight, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);
    // - Normal buffer
    glGenTextures(1, &gNormal);
    glBindTexture(GL_TEXTURE_2D, gNormal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, ScreenWidth, ScreenHeight, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);
    // RGB is the albedo and the A is the specularity.
    glGenTextures(1, &gAlbedoSpec);
    glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, ScreenWidth, ScreenHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedoSpec, 0);
    // Final buffer that is drawn to the screen.
    glGenTextures(1, &FinalTexture);
    glBindTexture(GL_TEXTURE_2D, FinalTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ScreenWidth, ScreenHeight, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, FinalTexture, 0);
    // - Create and attach depth texure.
    glGenTextures(1, &rboDepth);
    glBindTexture(GL_TEXTURE_2D, rboDepth);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH32F_STENCIL8, ScreenWidth, ScreenHeight, 0, GL_DEPTH_STENCIL,GL_FLOAT_32_UNSIGNED_INT_24_8_REV, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, rboDepth, 0);
    
    
    // - Finally check if framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer not complete!" << std::endl;
    
    // Detach the framebuffer.
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    
    // Set clear color.
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    // Create projection and view.
    projection = glm::perspective(45.0f, (float)ScreenWidth/(float)ScreenHeight, 0.1f, 100.0f);
    view = glm::lookAt(glm::vec3(0, 0, 4), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    Panorama = new Texture();
    Panorama->id = ImageLibManager::Instance()->loadImage("Panorama/austria.hdr");
    

    
    return true;
}

void Program::mainLoop()
{
    //NBFrames and last time is for calculating frame milliseconds.
    NBFrames = 0;
    LastTime = glfwGetTime();
    //ImageLibManager::Instance()->loadImage("test.png");
    //While window is open render.
    while (!glfwWindowShouldClose(window))
    {
        render();
    }
}

void Program::render()
{
    
    // To move the camera.
    if(testx <= 0.0)
    {
        bSwitch = false;
    }
    else if(testx >= 1.0)
    {
        bSwitch = true;
    }
    
    if (bSwitch == false)
    {
        testx += 0.001;
    }
    else
    {
        testx -= 0.001;
    }
    
    testy += 0.005;
    
    // Move Camera.
    view = glm::lookAt(glm::vec3(0, 1, 2), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    
    //Set gbuffer to draw to.
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, gBuffer);
    //Draw to the final buffer.
    glDrawBuffer(GL_COLOR_ATTACHMENT3);
    //Clear the final buffer color.
    glClear(GL_COLOR_BUFFER_BIT);
    
    
    // Enable setencil test for stencil pass.
    glEnable(GL_STENCIL_TEST);
    
    
    GeoPass();

    StencilPass();

    LightPass();
    
    glDisable(GL_STENCIL_TEST);
    
    
    
    // Detach the gBuffer.
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    
    // Attach the gbuffer for reading only.
    glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer);
    
    // Read from final buffer.
    glReadBuffer(GL_COLOR_ATTACHMENT3);
    
    // Draw the final buffer to the screen.
    glBlitFramebuffer(0, 0, ScreenWidth, ScreenHeight,0, 0, ScreenWidth, ScreenHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    
    // Detach the buffer.
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    
    // Swap buffers.
    glfwSwapInterval(0);
    glfwSwapBuffers(window);
    
    glfwPollEvents();
    
    //Calc how long to calculate frame
    CalcMS();
    

    
}

void Program::GeoPass()
{
    //Bind gBuffer
    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
        // Draw to color attachments.
    GLuint attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
    glDrawBuffers(3, attachments);
    
    // Enable Depth.
    glDepthMask(GL_TRUE);
    
    // Clear Buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glEnable(GL_DEPTH_TEST);
    
    glStencilFunc(GL_ALWAYS, 0, 0x00);
    
    //Use ShaderGeo shader to draw into gBuffer
    shaderGeometryPass->UseShader();
    
    // Create Model 4x4Matix for nanosuit model
    glm::mat4 model;
    model = glm::translate(model, glm::vec3(0, -1.75, 0.1f)); // Translate it down a bit so it's at the center of the scene
    model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// It's a bit too big for our scene, so scale it down
    
    model = glm::rotate(model, testy, glm::vec3(0,1,0));
    
    // Insert values into shader progam
    glUniformMatrix4fv(glGetUniformLocation(shaderGeometryPass->GetShader(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(shaderGeometryPass->GetShader(), "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaderGeometryPass->GetShader(), "model"), 1, GL_FALSE, glm::value_ptr(model));
    
    //Draw into models into gBuffer
    Nanosuit->Draw(*shaderGeometryPass);
    
    //model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
    //glUniformMatrix4fv(glGetUniformLocation(shaderGeometryPass->GetShader(), "model"), 1, GL_FALSE, glm::value_ptr(model));
    
    //Draw into models into gBuffer
    //Nanosuit->Draw(*shaderGeometryPass);
    
    glDepthMask(GL_FALSE);
    
}

void Program::StencilPass()
{
    /*
     * This pass I am doing volume stencil lighting.
     */
    
    // Allow everything into the stencil.
    
    glStencilFunc(GL_ALWAYS, 0, 0xff);

    
    // Only draw to the depth stencil buffer.
    glDrawBuffer(GL_NONE);

    glEnable(GL_DEPTH_TEST);
    
    // We disable cull face because the camera could be inside a point light.
    // So we must account for being inside a sphere model.
    glDisable(GL_CULL_FACE);
    
    glClear(GL_STENCIL_BUFFER_BIT);
    
    // The stencil operation for the back faces of the sphere model.
    glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
    // The stencil operation for the front faces of the sphere model.
    glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);
    
    // Create our model 4x4 matrix.
    glm::mat4 model;
    model = glm::translate(model, glm::vec3(0.0f, 1, .7));
    model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
    
    //Grab a basic shader to render the spheres.
    stencilPassShader->UseShader();
    
    // Insert values into the shader.
    glUniformMatrix4fv(glGetUniformLocation(stencilPassShader->GetShader(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(stencilPassShader->GetShader(), "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(stencilPassShader->GetShader(), "model"), 1, GL_FALSE, glm::value_ptr(model));
    
    //Draw light one.
    SphereModel->Draw(*stencilPassShader);
    
    glm::mat4 light2;
    light2 = glm::translate(light2, glm::vec3(0.0f, 0, 1));
    light2 = glm::scale(light2, glm::vec3(1.0f, 1.0f, 1.0f));
    
    glUniformMatrix4fv(glGetUniformLocation(stencilPassShader->GetShader(), "model"), 1, GL_FALSE, glm::value_ptr(light2));
    
    // Draw light two.
    SphereModel->Draw(*stencilPassShader);
    
    /*glm::mat4 light3;
    light3 = glm::translate(light3, glm::vec3(0.0f, 0, 1));
    light3 = glm::scale(light3, glm::vec3(1.0f, 1.0f, 1.0f));
    
    glUniformMatrix4fv(glGetUniformLocation(stencilPassShader->GetShader(), "model"), 1, GL_FALSE, glm::value_ptr(light3));
    
    // Draw light two.
    SphereModel->Draw(*stencilPassShader);*/
    
}

void Program::LightPass()
{
    // Draw to our final buffer.
    glDrawBuffer(GL_COLOR_ATTACHMENT3);
    
    // Bind our textures for reading to the shader.
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gPosition);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, gNormal);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
    
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, Panorama->id);
    
    
    // Stencil out what we dont want.
    glStencilFunc(GL_NOTEQUAL, 0,0xff);
    
    glDisable(GL_DEPTH_TEST);
    
    // Enable blending for the lights.
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_ONE, GL_ONE);
    
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    
    
    // Get the lighting shader.
    shaderLightingPass->UseShader();
    
    // Pass in screenwidth and height.
    glUniform2f(glGetUniformLocation(shaderLightingPass->GetShader(),"screensize"),ScreenWidth,ScreenHeight);
    
    // Insert values into shader progam
    glUniformMatrix4fv(glGetUniformLocation(shaderLightingPass->GetShader(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(shaderLightingPass->GetShader(), "view"), 1, GL_FALSE, glm::value_ptr(view));
    
    
    // Pass in the position of the camera.
    glUniform3fv(glGetUniformLocation(shaderLightingPass->GetShader(), "viewPos"), 1, glm::value_ptr(glm::vec3(0, 1, 2)));
   
    
    
    // Update attenuation for the light.
    const GLfloat linear = .3;
    const GLfloat quadratic = 1.0;
    glUniform1f(glGetUniformLocation(shaderLightingPass->GetShader(), "light.Linear"), linear);
    glUniform1f(glGetUniformLocation(shaderLightingPass->GetShader(), "ligh.Quadratic"), quadratic);
    
    // This will soon be its own buffer.
    // The roughness value of the model.
    glUniform1f(glGetUniformLocation(shaderLightingPass->GetShader(), "roughnessValue"),0.0f);
    // And F0 and k values for fresnel.
    glUniform1f(glGetUniformLocation(shaderLightingPass->GetShader(), "F0"), 1.0f);
    //glUniform1f(glGetUniformLocation(shaderLightingPass->GetShader(), "k"),0.2f);
    
    std::cout << testx <<endl;
    
    // Create 4x4 matrix for sphere model/light1.
    glm::mat4 light1;
    light1 = glm::translate(light1, glm::vec3(0.0f,1,1));
    light1 = glm::scale(light1, glm::vec3(1.0f, 1.0f, 1.0f));
    
    // The color of the light in for the shader.
    glUniform3fv(glGetUniformLocation(shaderLightingPass->GetShader(), "light.Color"), 1, glm::value_ptr(glm::vec3(1,1,1)));
    // Light position
    glUniform3fv(glGetUniformLocation(shaderLightingPass->GetShader(), "light.Position"), 1, glm::value_ptr(glm::vec3(0.0f, 1, 1)));
    // Light Model matrix.
    glUniformMatrix4fv(glGetUniformLocation(shaderLightingPass->GetShader(), "model"), 1, GL_FALSE, glm::value_ptr(light1));
    
    // Draw light1
    SphereModel->Draw(*shaderLightingPass);
    
    // Light 2 model matrix.
    glm::mat4 light2;
    light2 = glm::translate(light2, glm::vec3(0.0f, 0, 1));
    light2 = glm::scale(light2, glm::vec3(1.0f, 1.0f, 1.0f));
    // Light 2 color.
    glUniform3fv(glGetUniformLocation(shaderLightingPass->GetShader(), "light.Color"), 1, glm::value_ptr(glm::vec3(1,1,0)));
    // Light 2 position.
    glUniform3fv(glGetUniformLocation(shaderLightingPass->GetShader(), "light.Position"), 1, glm::value_ptr(glm::vec3(0,0,1)));
    // Light Model matrix.
    glUniformMatrix4fv(glGetUniformLocation(shaderLightingPass->GetShader(), "model"), 1, GL_FALSE, glm::value_ptr(light2));
    //Draw light 2.
    SphereModel->Draw(*shaderLightingPass);
    
    /*glm::mat4 light3;
    light3 = glm::translate(light3, glm::vec3(0.0f, 0, 1));
    light3 = glm::scale(light3, glm::vec3(1.0f, 1.0f, 1.0f));
    // Light 2 color.
    glUniform3fv(glGetUniformLocation(shaderLightingPass->GetShader(), "light.Color"), 1, glm::value_ptr(glm::vec3(1,1,0)));
    // Light 2 position.
    glUniform3fv(glGetUniformLocation(shaderLightingPass->GetShader(), "light.Position"), 1, glm::value_ptr(glm::vec3(0,0,1)));
    // Light Model matrix.
    glUniformMatrix4fv(glGetUniformLocation(shaderLightingPass->GetShader(), "model"), 1, GL_FALSE, glm::value_ptr(light2));
    //Draw light 2.
    SphereModel->Draw(*shaderLightingPass);
     */
    

    
    glCullFace(GL_BACK);
    
    glDisable(GL_BLEND);
}



void Program::CalcMS()
{
    CurrentTime = glfwGetTime();
    NBFrames++;
    if ( CurrentTime - LastTime >= 1.0 ){
        printf("%f ms/frame\n", 1000/double(NBFrames));
        
        NBFrames = 0;
        LastTime += 1.0;
    }
}



void Program::SetLevel(Level* level)
{
    TheLevel = level;
}

void Program::RenderQuad()
{
    GLuint quadVAO = 0;
    GLuint quadVBO;
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

Program::~Program()
{
    PhysxManager::Instance()->ShutDown();
    
    
    
    delete shaderGeometryPass;
    delete shaderLightingPass;
    delete stencilPassShader;
    delete Nanosuit;
    delete SphereModel;
    glfwDestroyWindow(window);
    
    glfwTerminate();
    
   
}
