#pragma once
#include <Core/Engine.h>
class Wall {
public:
	Wall()
	{
	}

	Wall(glm::vec3 cor, float h, float l, float w, bool lateral)
	{
		center = cor;
		height = h;
		length = l;
		width = w;
		isLateral = lateral;
	}

	glm::vec3 center;

	bool isLateral;
	float height, length, width;

};