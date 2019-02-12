#pragma once
#include <Component/SimpleScene.h>
#include "LabCamera.h"
#include "Table.h"
#include "Wall.h"
#include "Holes.h"
#include "Ball.h"
#include "Tac.h"
#include <list>

class Tema2 : public SimpleScene
{
	public:
		Tema2();
		~Tema2();

		void Init() override;

	private:
		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		void FrameEnd() override;

		void RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix) override;
		void RenderMeshColor(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix, const glm::vec3 col);
		void RenderMeshTac(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix, const glm::vec3 col);

		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;
		void OnKeyRelease(int key, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
		void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
		void OnWindowResize(int width, int height) override;
		void SetUp();
		void SetDown();

	protected:
		Laborator::Camera *camera;
		glm::mat4 projectionMatrix;
		bool renderCameraTarget;

		Table mytable;
		Wall walls[4];
		Hole holes[6];
		std::list<Ball> balls;
		Ball whiteball;
		Tac tac;
};
