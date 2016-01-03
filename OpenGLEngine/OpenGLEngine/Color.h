
//
//  Color.h
//  OpenGL
//
//  Created by Joseph Despain on 7/5/15.
//  Copyright (c) 2015 Joseph Despain. All rights reserved.
//

#ifndef __OpenGL__Color__
#define __OpenGL__Color__

class Color
{
public:

	Color(float _r, float _g, float _b, float _a);
	~Color();

	float r;
	float g;
	float b;
	float a;

private:

};

#endif /* defined(__OpenGL__Color__) */