#include "Tema2.h"
#include "Wall.h"
#include "Ball.h"

#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>
#include "Meshes.h"

using namespace std;
static float fov = 45.0f;

static float length = 30;
static float width = 10;
static float height = 5;
static float marginW = 0.5;

static float ballRadius = 0.6f;
static float holeRadius = 1.0f;

static float tacangle = 0;
static float tacdistance = 0;

static float renderTac = true;

static int turn = 1;
static char player1 = 'x';
static char player2 = 'x';
static bool fault = false;

static char firstBall = 'x';

static bool renderBall = true;

static int red = 0;
static int yellow = 0;

static bool notempty;
static bool cameraIsUp;

Tema2::Tema2()
{
}

Tema2::~Tema2()
{
}

void Tema2::Init()
{
	renderCameraTarget = false;
	renderTac = true;

	camera = new Laborator::Camera();
	camera->Set(glm::vec3(0, height + 2, length / 2 ), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));

	mytable = Table(glm::vec3(0, 0, 0), height, length, width, marginW);
	// modelMatrix = glm::translate(modelMatrix, glm::vec3(-width / 2 - marginW / 2, 0, 0));

	walls[0] = Wall(glm::vec3(-width / 2 - marginW / 2, 0, 0), height + marginW, length, marginW, true);
	walls[1] = Wall(glm::vec3(+width / 2 + marginW / 2, 0, 0), height + marginW, length, marginW, true);
	walls[2] = Wall(glm::vec3(0, 0, -length/2 - marginW/2), height + marginW, width, marginW, false);
	walls[3] = Wall(glm::vec3(0, 0, +length / 2 + marginW / 2), height + marginW, width, marginW, false);

	holes[0] = Hole(glm::vec3(-width / 2, height, -length / 2), 1);
	holes[1] = Hole(glm::vec3(width / 2, height, -length / 2), 1);
	holes[2] = Hole(glm::vec3(-width / 2, height, 0), 1);
	holes[3] = Hole(glm::vec3(width / 2, height, 0), 1);
	holes[4] = Hole(glm::vec3(-width / 2, height, length / 2), 1);
	holes[5] = Hole(glm::vec3(width/2, height, length / 2), 1);

	whiteball = Ball(glm::vec3(0, height + ballRadius/2, length / 3), ballRadius, 'w');
	whiteball.tx = 0;
	whiteball.tz = 0;
	
	float auxL = 8 * length / 2 / 10;
	float auxW = - 2 * (ballRadius + 0.2f);

	balls.push_back(Ball(glm::vec3(auxW, height + ballRadius / 2, -auxL), ballRadius, 'r'));
	balls.push_back(Ball(glm::vec3(auxW + (ballRadius + 0.1f), height + ballRadius / 2, -auxL), ballRadius, 'y'));
	balls.push_back(Ball(glm::vec3(auxW + 2 * (ballRadius + 0.1f), height + ballRadius / 2, -auxL), ballRadius, 'y'));
	balls.push_back(Ball(glm::vec3(auxW + 3 * (ballRadius + 0.1f), height + ballRadius / 2, -auxL), ballRadius, 'r'));
	balls.push_back(Ball(glm::vec3(auxW + 4 * (ballRadius + 0.1f), height + ballRadius / 2, -auxL), ballRadius, 'r'));

	auxW = -1.5f * (ballRadius + 0.2f);
	auxL -= ballRadius + 0.1f;
	balls.push_back(Ball(glm::vec3(auxW, height + ballRadius / 2, -auxL), ballRadius, 'y'));
	balls.push_back(Ball(glm::vec3(auxW + (ballRadius + 0.1f), height + ballRadius / 2, -auxL), ballRadius, 'r'));
	balls.push_back(Ball(glm::vec3(auxW + 2 * (ballRadius + 0.1f), height + ballRadius / 2, -auxL), ballRadius, 'r'));
	balls.push_back(Ball(glm::vec3(auxW + 3 * (ballRadius + 0.1f), height + ballRadius / 2, -auxL), ballRadius, 'y'));

	auxW = -1 * (ballRadius + 0.2f);
	auxL -= ballRadius + 0.1f;
	balls.push_back(Ball(glm::vec3(auxW , height + ballRadius / 2, -auxL), ballRadius, 'r'));
	balls.push_back(Ball(glm::vec3(auxW + (ballRadius + 0.1f), height + ballRadius / 2, -auxL), ballRadius, 'n'));
	balls.push_back(Ball(glm::vec3(auxW + 2 * (ballRadius + 0.1f), height + ballRadius / 2, -auxL), ballRadius, 'y'));

	auxW = -0.5f *(ballRadius + 0.2f);
	auxL -= (ballRadius + 0.1f);
	balls.push_back(Ball(glm::vec3(auxW, height + ballRadius / 2, -auxL), ballRadius, 'y'));
	balls.push_back(Ball(glm::vec3(auxW + (ballRadius + 0.1f), height + ballRadius / 2, -auxL), ballRadius, 'r'));

	auxW = 0;
	auxL -= (ballRadius + 0.1f);
	balls.push_back(Ball(glm::vec3(auxW, height + ballRadius / 2, -auxL), ballRadius, 'r'));

	tac = Tac(whiteball.center, length/2);

	projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 1.0f, 200.0f);

	// Meshes and shaders
	{
		{
			Mesh* mesh = CreateMyMesh("table", glm::vec3(0, 0, 0), length, width, height, glm::vec3(0.044f, 0.4f, 0.14f));
			meshes[mesh->GetMeshID()] = mesh;
		}

		{
			Mesh* mesh = CreateMyMesh("latwall", glm::vec3(0, 0, 0), length, marginW, height + marginW, glm::vec3(0.368f, 0.256f, 0.204));
			meshes[mesh->GetMeshID()] = mesh;
		}

		{
			Mesh* mesh = CreateMyMesh("fswall", glm::vec3(0, 0, 0), marginW, width + 2 * marginW, height + marginW, glm::vec3(0.368f, 0.256f, 0.204));
			meshes[mesh->GetMeshID()] = mesh;
		}

		{
			Mesh* mesh = new Mesh("hole");
			mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
			meshes[mesh->GetMeshID()] = mesh;
		}

		{
			Mesh* mesh = new Mesh("ball");
			mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
			meshes[mesh->GetMeshID()] = mesh;
		}

		{
			Mesh* mesh = new Mesh("tac");
			mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "tac.obj");
			meshes[mesh->GetMeshID()] = mesh;
		}



		{
			Shader *shader = new Shader("MyVertex");
			shader->AddShader("Source/Laboratoare/Tema2/VertexShaderColor.glsl", GL_VERTEX_SHADER);
			shader->AddShader("Source/Laboratoare/Tema2/FragmentShaderColor.glsl", GL_FRAGMENT_SHADER);
			shader->CreateAndLink();
			shaders[shader->GetName()] = shader;
		}

		{
			Shader *shader = new Shader("MyVertexTac");
			shader->AddShader("Source/Laboratoare/Tema2/VertexShaderTac.glsl", GL_VERTEX_SHADER);
			shader->AddShader("Source/Laboratoare/Tema2/FragmentShaderColor.glsl", GL_FRAGMENT_SHADER);
			shader->CreateAndLink();
			shaders[shader->GetName()] = shader;
		}
	}

	camera->Set(glm::vec3(whiteball.center.x, height + 2, whiteball.center.z + 5 * ballRadius), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
	projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, 0.1f, 200.0f);
	SetDown();

}

void Tema2::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Tema2::Update(float deltaTimeSeconds)
{
	// Render table
	{
		glm::mat4 modelMatrix = glm::mat4(1);

		// Attention! The RenderMesh function overrides the usual RenderMesh that we used until now
		// It uses the viewMatrix from Laborator::Camera instance and the local projectionMatrix
		RenderMesh(meshes["table"], shaders["VertexColor"], modelMatrix);
	}

	// Render walls
	{
		for each (Wall wall in walls) {
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, wall.center);
			if (wall.isLateral == true) {
				RenderMesh(meshes["latwall"], shaders["VertexColor"], modelMatrix);
			}
			else {
				RenderMesh(meshes["fswall"], shaders["VertexColor"], modelMatrix);
			}
		}
	}

	// Render holes
	{
		for each (Hole hole in holes) {
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, hole.center);
			modelMatrix = glm::scale(modelMatrix, glm::vec3(holeRadius, holeRadius, holeRadius));
			RenderMeshColor(meshes["hole"], shaders["MyVertex"], modelMatrix, glm::vec3(0, 0, 0));
		}
		
	}


	vector<Ball> todelete;
	vector<pair<Ball*, Ball*>> collidingBalls;

	// Render whiteBall
	if(renderBall == true)
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		//modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 6, 0));
		modelMatrix = glm::translate(modelMatrix, whiteball.center);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(ballRadius, ballRadius, ballRadius));
		RenderMeshColor(meshes["ball"], shaders["MyVertex"], modelMatrix, glm::vec3(1, 1, 1));

		if (whiteball.decX * whiteball.tx <= 0) {
			whiteball.tx = 0;
		}

		if (whiteball.decZ * whiteball.tz <= 0) {
			whiteball.tz = 0;
		}

		if (whiteball.tx == 0 && whiteball.tz == 0 && whiteball.inMovement == true) {
			whiteball.inMovement = false;

		}

		if (whiteball.inMovement == true) {
			renderTac = false;

			for each (Wall wall in walls)
			{
				if (wall.isLateral == true) {
					if (wall.center.x < 0) {
						if (whiteball.center.x - ballRadius / 2 <= wall.center.x + marginW / 2) {
							whiteball.tx = -whiteball.tx;
							whiteball.decX = -whiteball.decX;
						}
					}
					else {
						if (whiteball.center.x + ballRadius / 2 >= wall.center.x - marginW / 2) {
							whiteball.tx = -whiteball.tx;
							whiteball.decX = -whiteball.decX;
						}
					}

				}
				else {
					if (wall.center.z < 0) {
						if (whiteball.center.z - ballRadius/2 <= wall.center.z + marginW/2) {
							whiteball.tz = -whiteball.tz;
							whiteball.decZ = -whiteball.decZ;
						}
					}
					else {
						if (whiteball.center.z + ballRadius/2 >= wall.center.z - marginW/2) {
							whiteball.tz = -whiteball.tz;
							whiteball.decZ = -whiteball.decZ;
						}
					}
				}
			}

			for (auto &ball : balls) {
				float xd = ball.center.x - whiteball.center.x;
				float zd = ball.center.z - whiteball.center.z;
				float dist = sqrtf(xd * xd + zd * zd);

				float mindist = ballRadius;
				if (dist <= mindist) {
					
					if (firstBall == 'x') {
						firstBall = ball.type;
					}

					float overlap = 0.5f * (dist - ballRadius);

					collidingBalls.push_back({&whiteball, &ball});

					whiteball.center.x -= overlap * (whiteball.center.x - ball.center.x) / dist;
					whiteball.center.z -= overlap * (whiteball.center.z - ball.center.z) / dist;
				
					ball.center.x += overlap * (whiteball.center.x - ball.center.x) / dist;
					ball.center.z += overlap * (whiteball.center.z - ball.center.z) / dist;
				}
			}
		}
	}



	// Render balls
	{
		for (auto &ball : balls) {
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, ball.center);
			modelMatrix = glm::scale(modelMatrix, glm::vec3(ballRadius, ballRadius, ballRadius));
			glm::vec3 color = glm::vec3(0, 0, 0);
			if (ball.type == 'r') {
				color = glm::vec3(0.948f, 0.164f, 0.228f);
			} else if (ball.type == 'y') {
				color = glm::vec3(1, 1, 0);
			}
			RenderMeshColor(meshes["ball"], shaders["MyVertex"], modelMatrix, color);
			
			if (ball.inMovement == true) {
				if (ball.decX * ball.tx <= 0) {
					ball.tx = 0;
				}

				if (ball.decZ * ball.tz <= 0) {
					ball.tz = 0;
				}

				if (ball.tx == 0 && ball.tz == 0 && ball.inMovement == true) {
					ball.inMovement = false;

				}

				for each (Wall wall in walls)
				{
					if (wall.isLateral == true) {
						if (wall.center.x < 0) {
							if (ball.center.x - ballRadius / 2 <= wall.center.x + marginW / 2) {
								ball.tx = -ball.tx;
								ball.decX = -ball.decX;
							}
						}
						else {
							if (ball.center.x + ballRadius / 2 >= wall.center.x - marginW / 2) {
								ball.tx = -ball.tx;
								ball.decX = -ball.decX;
							}
						}

					}
					else {
						if (wall.center.z < 0) {
							if (ball.center.z - ballRadius / 2 <= wall.center.z + marginW / 2) {
								ball.tz = -ball.tz;
								ball.decZ = -ball.decZ;
							}
						}
						else {
							if (ball.center.z + ballRadius / 2 >= wall.center.z - marginW / 2) {
								ball.tz = -ball.tz;
								ball.decZ = -ball.decZ;
							}
						}
					}
				}


				for (auto &ballx : balls) {
					if (&ballx != &ball) {
						float xd = ballx.center.x - ball.center.x;
						float zd = ballx.center.z - ball.center.z;
						float dist = sqrtf(xd * xd + zd * zd);

						float mindist = ballRadius;
						if (dist <= mindist) {
							collidingBalls.push_back({ &ball, &ballx });

							float overlap = 1.5f * (dist - ballRadius);

							ball.center.x -= overlap * (ball.center.x - ballx.center.x) / dist;
							ball.center.z -= overlap * (ball.center.z - ballx.center.z) / dist;

							ballx.center.x += overlap * (ball.center.x - ballx.center.x) / dist;
							ballx.center.z += overlap * (ball.center.z - ballx.center.z) / dist;

						}
					}
				}

				float xd = whiteball.center.x - ball.center.x;
				float zd = whiteball.center.z - ball.center.z;
				float dist = sqrtf(xd * xd + zd * zd);

				float mindist = ballRadius;
				if (dist <= mindist) {
					collidingBalls.push_back({ &ball, &whiteball });

					float overlap = 1.5f * (dist - ballRadius);

					ball.center.x -= overlap * (ball.center.x - whiteball.center.x) / dist;
					ball.center.z -= overlap * (ball.center.z - whiteball.center.z) / dist;

					whiteball.center.x += overlap * (ball.center.x - whiteball.center.x) / dist;
					whiteball.center.z += overlap * (ball.center.z - whiteball.center.z) / dist;

				}

			}
		}
	}

	// Lets collide
	for (auto c : collidingBalls) {
		Ball *ball = c.first;
		Ball *ballx = c.second;

		float xd = ballx->center.x - ball->center.x;
		float zd = ballx->center.z - ball->center.z;
		float dist = sqrtf(xd * xd + zd * zd);

		float nx = (ballx->center.x - ball->center.x) / dist;
		float nz = (ballx->center.z - ball->center.z) / dist;

		float tx = -nz;
		float tz = nx;

		float dpTan1 = ball->tx * tx + ball->tz * tz;
		float dpTan2 = ballx->tx * tx + ballx->tz * tz;

		float dpNorm1 = ball->tx * nx + ball->tz * nz;
		float dpNorm2 = ballx->tx * nx + ballx->tz * nz;

		float m1 = dpNorm1;
		float m2 = dpNorm2;

		ball->tx = tx * dpTan1 + nx * m2;
		ball->tz = tz * dpTan1 + nz * m2;
		ball->decX = ball->tx * 0.3f;
		ball->decZ = ball->tz * 0.3f;

		ballx->tx = tx * dpTan2 + nx * m1;
		ballx->tz = tz * dpTan2 + nz * m1;
		ballx->inMovement = true;
		ballx->decX = ballx->tx * 0.3f;
		ballx->decZ = ballx->tz * 0.3f;
	}

	// Lets move 
	{
		for (auto &ball : balls) {
			if (ball.inMovement) {
				ball.center -= glm::vec3(ball.tx * deltaTimeSeconds, 0, ball.tz * deltaTimeSeconds);
				ball.tx -= ball.decX * deltaTimeSeconds;
				ball.tz -= ball.decZ * deltaTimeSeconds;
				for each(Hole hole in holes) {
					float dist = sqrtf((ball.center.x - hole.center.x) * (ball.center.x - hole.center.x) +
						(ball.center.z - hole.center.z)*(ball.center.z - hole.center.z));
					if (dist < ballRadius / 2 + holeRadius / 2) {
						todelete.push_back(ball);
					}
				}
			}
		}

		if (whiteball.inMovement) {
			whiteball.center -= glm::vec3(whiteball.tx * deltaTimeSeconds, 0, whiteball.tz * deltaTimeSeconds);
			whiteball.tx -= whiteball.decX * deltaTimeSeconds;
			whiteball.tz -= whiteball.decZ * deltaTimeSeconds;
			for each(Hole hole in holes) {
				float dist = sqrtf((whiteball.center.x - hole.center.x) * (whiteball.center.x - hole.center.x) +
					(whiteball.center.z - hole.center.z)*(whiteball.center.z - hole.center.z));
				if (dist < ballRadius / 2 + holeRadius / 2) {
					todelete.push_back(whiteball);
				}
			}
		}
	}


	
	// Interpret strike
	{
		if (!todelete.empty()) {
			notempty = true;
			fault = false;
			for (auto& ball : todelete) {
				if (ball == whiteball) {
					fault = true;
					whiteball.inMovement = false;
					renderBall = false;
				}
				else {
					if (turn == 1) {
						if (player1 == 'x') {
							player1 = ball.type;
							player2 = (player1 == 'y' ? 'r' : 'y');
						}
						else {
							if (ball.type != player1) {
								fault = true;
							}
						}
					}
					else {
						if (player2 == 'x') {
							player2 = ball.type;
							player1 = (player2 == 'y' ? 'r' : 'y');
						}
						else {
							if (ball.type != player2) {
								fault = true;
							}
						}
					}
				}
				if (ball.type == 'r')
					red++;
				if (ball.type == 'y')
					yellow++;
				if (ball.type == 'n')
					exit(turn);
				balls.remove(ball);
			}
			if (fault == false) {
				turn = (turn == 1 ? 2 : 1);
			}
			todelete.clear();
		}
	}

	// Render tac and move ball on fault
	if (whiteball.inMovement == false) {
		bool ok = true;
		for (auto &ball : balls) {
			if (ball.inMovement == true) {
				ok = false;
				break;
			}
		}
		if (ok == true && renderTac != true) {
			if (firstBall == 'x') {
				fault = true;
			}
			else {
				if (todelete.empty() && notempty == false) {
					fault == false;
					if (turn == 1) {
						if (player1 != 'x') {
							if (player1 != firstBall) {
								fault = true;
							}
						}
					}
					else {
						if (player2 != 'x') {
							if (player2 != firstBall) {
								fault = true;
							}
						}
					}
				}
			}

			turn = (turn == 1 ? 2 : 1);
			cout << "Player1 " << player1 << "\n";
			cout << "Player2 " << player2 << "\n";
			cout << "FirstBall " << firstBall << "\n";
			cout << "NextTurn " << turn << "\n";
			cout << "Red " << red << "  Yellow " << yellow << "\n";
			cout << "Fault " << fault << "\n";
			cout << "\n";
			firstBall = 'x';
			if (fault == true) {
				whiteball.center = glm::vec3(0, height + ballRadius / 2, length / 3);
				whiteball.decX = 0;
				whiteball.decZ = 0;
				whiteball.tx = 0;
				whiteball.tz = 0;
				fault = false;
			}
			renderBall = true;
			SetDown();
			renderTac = true;
		}
	}


	// Render TAC
	if(renderTac == true && cameraIsUp == false) {
		glm::mat4 modelMatrix = glm::mat4(1);
		
		float auxX = length / 4 * sin(tacangle);
		float auxZ = length / 4 * cos(tacangle);

		modelMatrix = glm::translate(modelMatrix, glm::vec3(whiteball.center.x + auxX, whiteball.center.y, whiteball.center.z + auxZ));
		modelMatrix = glm::rotate(modelMatrix, tacangle, glm::vec3(0, 1, 0));
		modelMatrix = glm::rotate(modelMatrix, 3.14f, glm::vec3(0,1,0));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, -length / 4));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.15f, 0.15f, 0.0955f));
		RenderMeshTac(meshes["tac"], shaders["MyVertexTac"], modelMatrix, glm::vec3(1, 0.5f, 0.1f));
	}


	// Render score
	{
		float auxX = - width / 2 - 2;
		float auxZ = - length / 2;
		
		if (player1 == 'r') {
			for (int i = 0; i < red; i++) {
				glm::mat4 modelMatrix = glm::mat4(1);
				modelMatrix = glm::translate(modelMatrix, glm::vec3(auxX, 6, auxZ));
				modelMatrix = glm::scale(modelMatrix, glm::vec3(ballRadius, ballRadius, ballRadius));
				glm::vec3 color = glm::vec3(0.948f, 0.164f, 0.228f);
				RenderMeshColor(meshes["ball"], shaders["MyVertex"], modelMatrix, color);
				auxZ += ballRadius + 0.2f;
			}
			auxX = width / 2 + 2;
			auxZ = -length / 2;;
			for (int i = 0; i < yellow; i++) {
				glm::mat4 modelMatrix = glm::mat4(1);
				modelMatrix = glm::translate(modelMatrix, glm::vec3(auxX, 6, auxZ));
				modelMatrix = glm::scale(modelMatrix, glm::vec3(ballRadius, ballRadius, ballRadius));
				glm::vec3 color = glm::vec3(1, 1, 0);
				RenderMeshColor(meshes["ball"], shaders["MyVertex"], modelMatrix, color);
				auxZ += ballRadius + 0.2f;
			}
		}

		if (player1 == 'y') {
			for (int i = 0; i < yellow; i++) {
				glm::mat4 modelMatrix = glm::mat4(1);
				modelMatrix = glm::translate(modelMatrix, glm::vec3(auxX, 6, auxZ));
				modelMatrix = glm::scale(modelMatrix, glm::vec3(ballRadius, ballRadius, ballRadius));
				glm::vec3 color = glm::vec3(1, 1, 0);
				RenderMeshColor(meshes["ball"], shaders["MyVertex"], modelMatrix, color);
				auxZ += ballRadius + 0.2f;
			}
			auxX = width/2 + 2;
			auxZ = -length / 2;;
			for (int i = 0; i < red; i++) {
				glm::mat4 modelMatrix = glm::mat4(1);
				modelMatrix = glm::translate(modelMatrix, glm::vec3(auxX, 6, auxZ));
				modelMatrix = glm::scale(modelMatrix, glm::vec3(ballRadius, ballRadius, ballRadius));
				glm::vec3 color = glm::vec3(0.948f, 0.164f, 0.228f);
				RenderMeshColor(meshes["ball"], shaders["MyVertex"], modelMatrix, color);
				auxZ += ballRadius + 0.2f;
			}
		}
	}
}


void Tema2::FrameEnd()
{
	DrawCoordinatSystem(camera->GetViewMatrix(), projectionMatrix);
}

void Tema2::RenderMeshColor(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix, glm::vec3 col)
{
	if (!mesh || !shader || !shader->program)
		return;

	// render an object using the specified shader and the specified position
	shader->Use();
	glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
	glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	int location = glGetUniformLocation(shader->program, "col");
	glUniform3fv(location,1, glm::value_ptr(col));



	mesh->Render();
}

void Tema2::RenderMeshTac(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix, glm::vec3 col)
{
	if (!mesh || !shader || !shader->program)
		return;

	// render an object using the specified shader and the specified position
	shader->Use();
	glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
	glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	int location = glGetUniformLocation(shader->program, "col");
	glUniform3fv(location, 1, glm::value_ptr(col));

	float auxZ = tacdistance;

	glm::vec3 dist = glm::vec3(0, 0, 3 * auxZ);
	location = glGetUniformLocation(shader->program, "distance");
	glUniform3fv(location, 1, glm::value_ptr(dist));

	location = glGetUniformLocation(shader->program, "tac");
	glUniform1f(location, tacdistance);

	mesh->Render();
}

void Tema2::RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix)
{
	if (!mesh || !shader || !shader->program)
		return;

	// render an object using the specified shader and the specified position
	shader->Use();
	glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
	glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	mesh->Render();
}

// Documentation for the input functions can be found in: "/Source/Core/Window/InputController.h" or
// https://github.com/UPB-Graphics/Framework-EGC/blob/master/Source/Core/Window/InputController.h

void Tema2::OnInputUpdate(float deltaTime, int mods)
{
	if (window->MouseHold(GLFW_MOUSE_BUTTON_LEFT))
	{
		if (tacdistance < 3) {
			tacdistance += 3 * deltaTime;
		}
	} else
	// move the camera only if MOUSE_RIGHT button is pressed
	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		float cameraSpeed = 6.0f;

		if (window->KeyHold(GLFW_KEY_W)) {
			// TODO : translate the camera forward

			camera->TranslateForward(cameraSpeed * deltaTime);
		}

		if (window->KeyHold(GLFW_KEY_A)) {
			// TODO : translate the camera to the left
			camera->TranslateRight(-cameraSpeed * deltaTime);
		}

		if (window->KeyHold(GLFW_KEY_S)) {
			// TODO : translate the camera backwards
			camera->TranslateForward(-cameraSpeed * deltaTime);
		}

		if (window->KeyHold(GLFW_KEY_D)) {
			// TODO : translate the camera to the right
			camera->TranslateRight(cameraSpeed * deltaTime);
		}

		if (window->KeyHold(GLFW_KEY_Q)) {
			// TODO : translate the camera down
			camera->TranslateUpword(-cameraSpeed * deltaTime);
		}

		if (window->KeyHold(GLFW_KEY_E)) {
			// TODO : translate the camera up
			camera->TranslateUpword(cameraSpeed * deltaTime);
		}

		if (window->KeyHold(GLFW_KEY_R)) {
			// TODO : fov+
			fov += deltaTime * cameraSpeed * 5;
			projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, 0.01f, 200.0f);
		}

		if (window->KeyHold(GLFW_KEY_F)) {
			// TODO : fov-
			fov -= deltaTime * cameraSpeed * 5;
			projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, 0.01f, 200.0f);
		}

	}
	else {
		float movementSpeed = 6.0f;
		if (window->KeyHold(GLFW_KEY_W) && whiteball.center.x <= width / 2 - whiteball.r) {
			whiteball.center += glm::vec3(movementSpeed * deltaTime, 0, 0);
		}

		// && whiteball.center.z >= length / 3
		if (window->KeyHold(GLFW_KEY_A)) {
			whiteball.center -= glm::vec3(0, 0, movementSpeed * deltaTime);

		}

		if (window->KeyHold(GLFW_KEY_S) && whiteball.center.x >= -width / 2 + whiteball.r) {
			whiteball.center -= glm::vec3(movementSpeed * deltaTime, 0, 0);
		}

		if (window->KeyHold(GLFW_KEY_D) && whiteball.center.z <= length / 2 - whiteball.r) {
			whiteball.center += glm::vec3(0, 0, movementSpeed * deltaTime);

		}
	}
}

void Tema2::OnKeyPress(int key, int mods)
{
	// add key press event

	/*if (key == GLFW_KEY_O)
	{
		camera->Set(glm::vec3(0, 6 * height, 0), glm::vec3(0, 0, 1), glm::vec3(1, 0, 0));
		projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, 0.1f, 200.0f);
	}

	if (key == GLFW_KEY_P)
	{
		float x = whiteball.center.x;
		float z = whiteball.center.z;
		float newAngle = atan2(x, z);

		tacdistance = 0;

		float auxZ = 6 * ballRadius * cos(newAngle);
		float auxX = 6 * ballRadius * sin(newAngle);
		tacangle = newAngle;
		camera->Set(glm::vec3(whiteball.center.x + auxX, height + 1, whiteball.center.z + auxZ), glm::vec3(0, height + 1, 0), glm::vec3(0, 1, 0));
	}*/

	if (key == GLFW_KEY_SPACE) {
		if(cameraIsUp == false) {
			SetUp();
		}
		else {
			SetDown();
		}
	}
}


void Tema2::SetUp() {
	cameraIsUp = true;
	camera->Set(glm::vec3(0, 6 * height, 0), glm::vec3(0, 0, 1), glm::vec3(1, 0, 0));
	projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, 0.1f, 200.0f);
}

void Tema2::SetDown() {
	cameraIsUp = false;

	float x = whiteball.center.x;
	float z = whiteball.center.z;
	float newAngle = atan2(x, z);

	tacdistance = 0;

	float auxZ = 6 * ballRadius * cos(newAngle);
	float auxX = 6 * ballRadius * sin(newAngle);
	tacangle = newAngle;
	camera->Set(glm::vec3(whiteball.center.x + auxX, height + 1, whiteball.center.z + auxZ), glm::vec3(0, height + 1, 0), glm::vec3(0, 1, 0));
}

void Tema2::OnKeyRelease(int key, int mods)
{
	// add key release event

}

void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event


	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		float sensivityOX = 0.001f;
		float sensivityOY = 0.001f;

		if (window->GetSpecialKeyState() == 0) {
			renderCameraTarget = true;
			// TODO : rotate the camera in Third-person mode around OX and OY using deltaX and deltaY
			// use the sensitivity variables for setting up the rotation speed
			//camera->RotateThirdPerson_OX(sensivityOX * deltaY);
			camera->RotateThirdPerson_OY(sensivityOY * deltaX, whiteball.center, ballRadius);
			tacangle += sensivityOY * deltaX / 2;
			projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, 0.1f, 200.0f);

		}

		if (window->GetSpecialKeyState() && GLFW_MOD_CONTROL) {

			renderCameraTarget = false;
			// TODO : rotate the camera in First-person mode around OX and OY using deltaX and deltaY
			// use the sensitivity variables for setting up the rotation speed
			camera->RotateFirstPerson_OX(sensivityOX * deltaY);
			camera->RotateFirstPerson_OY(sensivityOY * deltaX);
		}

	}
}

void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
	if (button == 1)
	{
		renderTac = false;
		whiteball.tx = tacdistance * sin(tacangle) * whiteball.speed;

		whiteball.tz = tacdistance * cos(tacangle) * whiteball.speed;

		firstBall = 'x';
		notempty = false;
		whiteball.decX = whiteball.tx * 0.1f;
		whiteball.decZ = whiteball.tz * 0.1f;

		whiteball.inMovement = true;
		SetUp();
		tacdistance = 0;
	}
}

void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema2::OnWindowResize(int width, int height)
{
}
