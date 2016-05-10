//
//  RenderComponent.h
//  OpenGL
//
//  Created by Joseph Despain on 5/23/15.
//  Copyright (c) 2015 Joseph Despain. All rights reserved.
//
#ifndef RENDERNCOMP_H
#define RENDERNCOMP_H

#include "Component.h"
#include <GL/glew.h>
#include <vector>
#include "HIDManager.h"
#include "JDFileManager.h"
#include "JDMath.h"
#include "Shader.h"
#include "Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Mesh.h"

class RenderComponent : public Component
{
public:
	RenderComponent(Camera* camera, Shader NewShader, Mesh NewMesh);
	~RenderComponent();

	virtual void Update();
	virtual void Start();

	Shader* shader;
	Mesh* RenderMesh;

	float test;
private:
	void Draw();
	GLuint VBO;
	GLuint VAO;
	Camera* CurrentCamera;



	//std::vector<unsigned int>* indices;



};


#endif