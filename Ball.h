#pragma once
#include <Core/Engine.h>


class Ball {
public:
	Ball()
	{
	}
	Ball(glm::vec3 cor, float radius, char t)
	{
		center = cor;
		r = radius;
		type = t;
		inMovement = false;
		tx = 0;
		tz = 0;
		decX = 0;
		decZ = 0;
	}

	glm::vec3 center;
	float r;
	char type;

	// Velocitatea pe OX si OY
	float tx;
	float tz;

	float speed = 4;

	bool inMovement;
	float decX;
	float decZ;

	bool operator==(const Ball& other) const
	{
		return center == other.center;
	}
};