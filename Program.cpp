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
    ScreenWidth = 1920;
    ScreenHeight = 1080;
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

    //int width, height;
    //glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, ScreenWidth, ScreenHeight);
    
    
    TheLevel = new Level(ScreenWidth,ScreenHeight);
    shaderGeometryPass = new Shader("gbuffer.vert", "gbuffer.frag");
    shaderLightingPass = new Shader("deferredlighting.vert", "deferredlighting.frag");
    
    shaderLightingPass->UseShader();
    glUniform1i(glGetUniformLocation(shaderLightingPass->GetShader(), "gPosition"), 0);
    glUniform1i(glGetUniformLocation(shaderLightingPass->GetShader(), "gNormal"), 1);
    glUniform1i(glGetUniformLocation(shaderLightingPass->GetShader(), "gAlbedoSpec"), 2);
    
    // Load models
    ourModel = new Model("Nanosuit/nanosuit.obj");

   
    // - Colors
    const GLuint NR_LIGHTS = 32;
    srand(13);
    for (GLuint i = 0; i < NR_LIGHTS; i++)
    {
        // Calculate slightly random offsets
        GLfloat xPos = ((rand() % 100) / 100.0) * 6.0 - 3.0;
        GLfloat yPos = ((rand() % 100) / 100.0) * 6.0 - 4.0;
        GLfloat zPos = ((rand() % 100) / 100.0) * 6.0 - 3.0;
        lightPositions.push_back(glm::vec3(xPos, yPos, zPos));
        // Also calculate random color
        GLfloat rColor = ((rand() % 100) / 200.0f) + 0.5; // Between 0.5 and 1.0
        GLfloat gColor = ((rand() % 100) / 200.0f) + 0.5; // Between 0.5 and 1.0
        GLfloat bColor = ((rand() % 100) / 200.0f) + 0.5; // Between 0.5 and 1.0
        lightColors.push_back(glm::vec3(rColor, gColor, bColor));
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
    // - Tell OpenGL which color attachments we'll use (of this framebuffer) for rendering
    GLuint attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
    glDrawBuffers(3, attachments);
    // - Create and attach depth buffer (renderbuffer)
    GLuint rboDepth;
    glGenRenderbuffers(1, &rboDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, ScreenWidth, ScreenHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
    // - Finally check if framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    mainLoop();
    
        
    return true;
}

void Program::mainLoop()
{
    while (!glfwWindowShouldClose(window))
    {
        render();
    }
}

void Program::render()
{
    if (testx > 2) {
        testy = 1;
    } else if(testx < -2)
    {
        testy = 0;
    }
    
    if(testy == 1)
    {
        testx -= 0.01;
    }
    else
    {
        testx += 0.01;
    }
    
    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //shader->UseShader();   // <-- Don't forget this one!
    // Transformation matrices
    glm::mat4 projection = glm::perspective(45.0f, (float)ScreenWidth/(float)ScreenHeight, 0.1f, 100.0f);
    glm::mat4 view = glm::lookAt(glm::vec3(testx, 2.5, 4), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    shaderGeometryPass->UseShader();
    glUniformMatrix4fv(glGetUniformLocation(shaderGeometryPass->GetShader(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(shaderGeometryPass->GetShader(), "view"), 1, GL_FALSE, glm::value_ptr(view));
    
    // Draw the loaded model
    glm::mat4 model;
    model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // Translate it down a bit so it's at the center of the scene
    model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// It's a bit too big for our scene, so scale it down
    glUniformMatrix4fv(glGetUniformLocation(shaderGeometryPass->GetShader(), "model"), 1, GL_FALSE, glm::value_ptr(model));
    ourModel->Draw(*shaderGeometryPass);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //TheLevel->Update();
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shaderLightingPass->UseShader();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gPosition);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, gNormal);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
    // Also send light relevant uniforms
    for (GLuint i = 0; i < lightPositions.size(); i++)
    {
        glUniform3fv(glGetUniformLocation(shaderLightingPass->GetShader(), ("lights[" + std::to_string(i) + "].Position").c_str()), 1, &lightPositions[i][0]);
        glUniform3fv(glGetUniformLocation(shaderLightingPass->GetShader(), ("lights[" + std::to_string(i) + "].Color").c_str()), 1, &lightColors[i][0]);
        // Update attenuation parameters and calculate radius
        const GLfloat constant = 1.0; // Note that we don't send this to the shader, we assume it is always 1.0 (in our case)
        const GLfloat linear = 0.7;
        const GLfloat quadratic = 1.8;
        glUniform1f(glGetUniformLocation(shaderLightingPass->GetShader(), ("lights[" + std::to_string(i) + "].Linear").c_str()), linear);
        glUniform1f(glGetUniformLocation(shaderLightingPass->GetShader(), ("lights[" + std::to_string(i) + "].Quadratic").c_str()), quadratic);
        // Then calculate radius of light volume/sphere
        const GLfloat maxBrightness = std::fmaxf(std::fmaxf(lightColors[i].r, lightColors[i].g), lightColors[i].b);
        GLfloat radius = (-linear + std::sqrtf(linear * linear - 4 * quadratic * (constant - (256.0 / 5.0) * maxBrightness))) / (2 * quadratic);
        glUniform1f(glGetUniformLocation(shaderLightingPass->GetShader(), ("lights[" + std::to_string(i) + "].Radius").c_str()), radius);
    }
    
    float campos[] = { testx, 2.5, 4};
    glUniform3fv(glGetUniformLocation(shaderLightingPass->GetShader(), "viewPos"), 1, campos);
    // Finally render quad
    RenderQuad();
    
    glfwSwapBuffers(window);
    glfwPollEvents();
    
    
    
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
