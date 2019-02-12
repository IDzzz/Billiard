#pragma once
#include <Core/Engine.h>
class Table {
public:
	Table()
	{
	}

	Table(glm::vec3 cor, float h, float l, float w, float marginW)
	{
		center = cor;
		height = h;
		length = l;
		width = w;
		marginWidth = marginW;
	}

	glm::vec3 center;

	float height, length, width;

	float marginWidth;
};