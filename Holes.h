#pragma once
#include <Core/Engine.h>

class Hole {
public:
	Hole()
	{
	}
	Hole(glm::vec3 cor, float radius)
	{
		center = cor;
		r = radius;
	}

	glm::vec3 center;
	float r;

};