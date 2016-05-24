//
//  ImageLibManager.cpp
//  OpenGL
//
//  Created by Joseph Despain on 7/8/15.
//  Copyright (c) 2015 Joseph Despain. All rights reserved.
//

#include "ImageLibManager.h"


ImageLibManager* ImageLibManager::pInstance = NULL;


ImageLibManager::ImageLibManager()
{

}

ImageLibManager::~ImageLibManager()
{

}

ImageLibManager* ImageLibManager::Instance()
{
	if (!pInstance)
	{
		pInstance = new ImageLibManager();
	}

	return pInstance;
}

void ImageLibManager::Init()
{
	ilutRenderer(ILUT_OPENGL);
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);
}

GLuint ImageLibManager::loadCubemap(std::vector<const GLchar*> faces)
{
	GLuint textureID;
	glGenTextures(1, &textureID);
	glActiveTexture(GL_TEXTURE0);

	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


	for(GLuint i = 0; i < faces.size(); i++)
	{
		ILuint image = ilGenImage();

		// Load with DevIL
		ilBindImage( image );
		if( !ilLoadImage((const ILstring)faces[i]))
		{
			std::cout << "ERROR: Failed to load image " << std::endl;
		}

		ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);


		// Fetch info from DevIL
		int width   = ilGetInteger( IL_IMAGE_WIDTH );
		int height  = ilGetInteger( IL_IMAGE_HEIGHT );
		ILuint format = ilGetInteger( IL_IMAGE_FORMAT );
		ILuint type   = ilGetInteger( IL_IMAGE_TYPE );

		// Send data to OpenGL
		glTexImage2D(
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
			0,
			GL_RGB,
			width,
			height,
			0,
			format,
			type,
			ilGetData() );


		// Get rid of DevIL data
		ilDeleteImage( image );
	}

	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	//glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return textureID;
}


GLuint ImageLibManager::LoadIBLCubeMap(std::string FileNameStarter, GLuint NumOfMips)
{


	GLuint textureID;
	glGenTextures(1, &textureID);
	glActiveTexture(GL_TEXTURE0);

	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);


	for(GLuint CurrentMip = 0; CurrentMip < NumOfMips; CurrentMip++)
	{
		for(GLuint i = 0; i < 6; i++)
		{
			/*
			* This generates the name of the file. For example FILENAME_m00_c00.bmp this is the on the mipmap level 0 and the face 0.
			*/
			std::string FaceImageName = FileNameStarter + "_m0" + std::to_string(CurrentMip) + "_c0" + std::to_string(i) + ".png";
			const char *CharFileName = FaceImageName.c_str();

			ILuint image = ilGenImage();

			// Load with DevIL
			ilBindImage( image );
			if( !ilLoadImage((const ILstring)CharFileName))
			{
				std::cout << "ERROR: Failed to load image " << std::endl;
			}

			ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);


			// Fetch info from DevIL
			int width   = ilGetInteger( IL_IMAGE_WIDTH );
			int height  = ilGetInteger( IL_IMAGE_HEIGHT );
			ILuint format = ilGetInteger( IL_IMAGE_FORMAT );
			ILuint type   = ilGetInteger( IL_IMAGE_TYPE );

			// Send data to OpenGL
			glTexImage2D(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				CurrentMip,
				GL_RGB,
				width,
				height,
				0,
				format,
				type,
				ilGetData() );


			// Get rid of DevIL data
			ilDeleteImage( image );
		}

	}

	return textureID;
}

GLuint ImageLibManager::loadImage(const char* theFileName)
{
	ILuint imageID;				// Create an image ID as a ULuint

	GLuint textureID;			// Create a texture ID as a GLuint

	ILboolean success;			// Create a flag to keep track of success/failure

	ILenum error;				// Create a flag to keep track of the IL error state

	ilGenImages(1, &imageID); 		// Generate the image ID

	ilBindImage(imageID); 			// Bind the image

	success = ilLoadImage((const ILstring)theFileName); 	// Load the image file

	// If we managed to load the image, then we can start to do things with it...
	if (success)
	{
		// If the image is flipped (i.e. upside-down and mirrored, flip it the right way up!)
		ILinfo ImageInfo;
		iluGetImageInfo(&ImageInfo);
		/*if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
		{
		//iluFlipImage();
		}*/

		// Convert the image into a suitable format to work with
		// NOTE: If your image contains alpha channel you can replace IL_RGB with IL_RGBA
		success = ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);

		// Quit out if we failed the conversion
		if (!success)
		{
			error = ilGetError();
			std::cout << "Image conversion failed - IL reports error: " << error << " - " << iluErrorString(error) << std::endl;
			exit(-1);
		}

		// Generate a new texture
		glGenTextures(1, &textureID);

		// Bind the texture to a name
		glBindTexture(GL_TEXTURE_2D, textureID);

		// Set texture clamping method
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		// Set texture interpolation method to use linear interpolation (no MIPMAPS)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		/*
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		*/

		// Specify the texture specification
		glTexImage2D(GL_TEXTURE_2D, 				// Type of texture
			0,				// Pyramid level (for mip-mapping) - 0 is the top level
			ilGetInteger(IL_IMAGE_FORMAT),	// Internal pixel format to use. Can be a generic type like GL_RGB or GL_RGBA, or a sized type
			ilGetInteger(IL_IMAGE_WIDTH),	// Image width
			ilGetInteger(IL_IMAGE_HEIGHT),	// Image height
			0,				// Border width in pixels (can either be 1 or 0)
			ilGetInteger(IL_IMAGE_FORMAT),	// Format of image pixel data
			GL_UNSIGNED_BYTE,		// Image data type
			ilGetData());			// The actual image data itself

		glGenerateMipmap(GL_TEXTURE_2D);


	}
	else // If we failed to open the image file in the first place...
	{
		error = ilGetError();
		std::cout << "Image load failed - IL reports error: " << theFileName << " - " << iluErrorString(error) << std::endl;
		std::cout << "Image load failed - IL reports error: " << error << " - " << iluErrorString(error) << std::endl;
		exit(-1);
	}

	ilDeleteImages(1, &imageID); // Because we have already copied image data into texture data we can release memory used by image.

	//std::cout << "Texture creation successful." << std::endl;

	return textureID; // Return the GLuint to the texture so you can use it!
}

bool ImageLibManager::saveTextureToFile(GLuint textureID, std::string fileName)
{


	// Get Texture Information
	GLint width,height,internalFormat;
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_COMPONENTS, &internalFormat);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);

	
	GLint numBytes = 0;
	switch(internalFormat) 
	{
	case GL_RGB:
		numBytes = width * height * 3;
		break;
	case GL_RGBA:
		numBytes = width * height * 4;
		break;
	case GL_RG16:
		numBytes = width * height * 4;
		break;
	default: 
		break;
	}

	if(numBytes)
	{
		// Allocate space for our pixel data.
		unsigned char *pixels = (unsigned char*)malloc(numBytes); // allocate image data into RAM

		// Get pixel data!
		glGetTexImage(GL_TEXTURE_2D, 0, internalFormat, GL_UNSIGNED_BYTE, pixels);

	
		// Create DEVIL image.
		ILuint imageID = ilGenImage();
		ilBindImage(imageID);

		ilTexImage(

			width,
			height,

			1,

			3,  

			IL_RGB,  

			IL_UNSIGNED_BYTE, 

			pixels  

			) ;

		// allow openIL to overwrite the file
		ilEnable(IL_FILE_OVERWRITE);
		
		if(!ilSave(IL_PNG, fileName.c_str()))
		{
			free(pixels);
			return false;
		}
		else
		{
			free(pixels);
			return true;
		}
	}

	return false;
}