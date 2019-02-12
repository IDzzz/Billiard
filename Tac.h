#pragma once
#pragma once
#include <Core/Engine.h>
class Tac {
public:
	Tac()
	{
	}

	Tac(glm::vec3 cor, float l)
	{
		center = cor;
		length = l;
		angle = 0;
	}

	glm::vec3 center;

	float length;
	float angle;
};