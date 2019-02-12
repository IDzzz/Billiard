#pragma once
#include <Core/Engine.h>

Mesh* CreateMyMesh(std::string name, glm::vec3 center, float length, float width, float height, glm::vec3 color)
{

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(center + glm::vec3(-width / 2, 0 , +length / 2), color),
		VertexFormat(center + glm::vec3(width / 2, 0 , +length / 2), color),
		VertexFormat(center + glm::vec3(-width / 2, height, +length / 2), color),
		VertexFormat(center + glm::vec3(width / 2, height , +length / 2), color),

		VertexFormat(center + glm::vec3(-width / 2, 0 , -length / 2), color),
		VertexFormat(center + glm::vec3(width / 2, 0 , -length / 2), color),
		VertexFormat(center + glm::vec3(-width / 2, height, -length / 2), color),
		VertexFormat(center + glm::vec3(width / 2, height , -length / 2), color),

	};

	Mesh* square = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3, 7, 1, 5, 4, 7, 6, 2, 4, 0, 1 };
	square->SetDrawMode(GL_TRIANGLE_STRIP);

	square->InitFromData(vertices, indices);
	return square;
}