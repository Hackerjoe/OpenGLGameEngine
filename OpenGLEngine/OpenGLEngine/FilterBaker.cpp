#include "FilterBaker.h"
#include "ImageLibManager.h"
#include <vector>
#include "Model.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>

FilterBaker::FilterBaker(std::vector<const GLchar*> faceImages,string outputName,GLint size)
{
	GLuint cubemapTexture = ImageLibManager::Instance()->loadCubemap(faceImages);


	glViewport(0, 0, size, size);

	std::vector<GLubyte> testData(512 * 512 * 4, 255);
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size, size,0, GL_RGB, GL_UNSIGNED_BYTE, &testData[0]);


	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frameBuffer);

	GLuint fbo = 0;
	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frameBuffer);
	glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture, 0);


	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete for baker!" << std::endl;

	double maxMipLevels = log(size) / log(2);


	Shader* shader = new Shader("Resources/Shaders/simple.vert","Resources/Shaders/filterbacker.frag");
	for(int mips = 0; mips <= maxMipLevels; mips++)
	{
		int sizeOfTexture = 128 >> mips;
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, sizeOfTexture, sizeOfTexture,0, GL_RGB, GL_UNSIGNED_BYTE, &testData[0]);
		glViewport(0, 0, sizeOfTexture, sizeOfTexture);

		float roughness;
		if(mips == 0)
			roughness = 0;
		else
			roughness = mips / maxMipLevels;


		for(int face = 0; face <= 5; face++)
		{
			glClearColor(1.0,1.0,0.0,1.0);
			glClear(GL_COLOR_BUFFER_BIT);

			shader->UseShader();

			glUniform1i(glGetUniformLocation(shader->GetShader(),"mipLevel"),mips);
			glUniform1i(glGetUniformLocation(shader->GetShader(),"cubeSize"),sizeOfTexture);
			glUniform1f(glGetUniformLocation(shader->GetShader(),"Roughness"),roughness);
			glUniform1i(glGetUniformLocation(shader->GetShader(),"face"),face);
			glActiveTexture(GL_TEXTURE0);
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

			glBindTexture(GL_TEXTURE_2D, texture);
			std::string fileName = outputName + "_m0" + std::to_string(mips) + "_c0"+ std::to_string(face) + ".png";
			ImageLibManager::Instance()->saveTextureToFile(texture,fileName);
		}
	}
	
	shader = new Shader("Resources/Shaders/simple.vert","Resources/Shaders/EnvironmentBRDF.frag");
	shader->UseShader();

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 256, 256,0, GL_RGB, GL_UNSIGNED_BYTE, &testData[0]);
	glViewport(0, 0, 256, 256);

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

	ImageLibManager::Instance()->saveTextureToFile(texture,"ENVBRDF.png");



}


FilterBaker::~FilterBaker(void)
{

}
