#include "TheGame.h"
#include <iostream>
#include <string>
#include <Windows.h>
#include <ctime>
#include "stb_image.h"

// sets up display, method call to start gameloop, instantiates mesh plane and audio device
TheGame::TheGame()
{
	gameState = GameState::PLAY;
	DisplayWindow* displayWindow = new DisplayWindow();
	run();
    Mesh* plane();
	Sound audio();

	Shader* NewShader();
	Shader* OldShader();
}

TheGame::~TheGame()
{
}

// Runs methods to instantiate objects and files then starts game loop
void TheGame::run()
{
	Initialise();
	GameLoop();
}

void TheGame::Initialise()
{
	//sets up display
	displayWindow.InitialiseDisplay();
/*
	cout << "Hits to engines: " << HitsTaken << " / " << HitLimit << endl;*/
	// loads audio files
	jetSound = audio.AudioFileLoad("..\\res\\Sounds\\Wind.wav");
	hitSound = audio.AudioFileLoad("..\\res\\Sounds\\BirdHit.wav");
	chirpSound = audio.AudioFileLoad("..\\res\\Sounds\\Chirp.wav");
	// loads 3D models
	plane.ModelFileLoad("..\\res\\Objects\\NewPlane.obj");
	bird1.ModelFileLoad("..\\res\\Objects\\bird.obj");
	bird2.ModelFileLoad("..\\res\\Objects\\bird.obj");
	bird3.ModelFileLoad("..\\res\\Objects\\bird.obj");
	skySphere.ModelFileLoad("..\\res\\Objects\\SkyCubeTest.obj");
	// sets camera attributes
	cam.InitialiseCamera(glm::vec3(0, 3, -5), 70.0f, (float)displayWindow.GetWidth()/ displayWindow.GetHeight(), 0.01f, 100.0f);
	// starting positons for game
	SetInitialPositions();

	PlaneShader.LoadThreeShaders("..\\res\\Shaders\\shaderRimToon");
	OldShader.InitialiseShader("..\\res\\Shaders\\shader");
	//DissShader.InitialiseShader("..\\res\\Shaders\\DissolveShader");
	Skybox.InitialiseShader("..\\res\\Shaders\\Cubemap");
	VisNormShader.LoadThreeShaders("..\\res\\Shaders\\VisNormShader");

	skyTexture.LoadTextureFile("..\\res\\Textures\\Sky.jpg");
	planeTexture.LoadTextureFile("..\\res\\Textures\\Metal.jpg");
	birdTexture.LoadTextureFile("..\\res\\Textures\\fur.jpg");

	//skyboxFaces[0].LoadTextureFile("..\\res\\Textures\\Sky.jpg");
	//skyboxFaces[1].LoadTextureFile("..\\res\\Textures\\Sky.jpg");
	//skyboxFaces[2].LoadTextureFile("..\\res\\Textures\\Sky.jpg");
	//skyboxFaces[3].LoadTextureFile("..\\res\\Textures\\Sky.jpg");
	//skyboxFaces[4].LoadTextureFile("..\\res\\Textures\\Sky.jpg");
	//skyboxFaces[5].LoadTextureFile("..\\res\\Textures\\Sky.jpg");

	//skyboxFaces.size();

	cubemapTex = SetSkyboxTex();
	SetSkyboxVertices();
}

void TheGame::PlaySoundFiles(unsigned int soundFile, glm::vec3 origin)
{
	// plays audio if the sound isnt already being played
	ALint state;
	alGetSourcei(soundFile, AL_SOURCE_STATE, &state);
	if (AL_PLAYING != state)
	{
		audio.Play(soundFile, origin);
	}
}

void TheGame::SetInitialPositions() {
	// engines left and right positions of plane
	leftEngine = glm::vec3(planePos.x + engineOffsetx - (PlaneRotZ * 0.05), planePos.y + engineOffsety - (planeRotX * 2.5) + (PlaneRotZ * 2.5), planePos.z + engineOffsetz + (planeRotX * 0.05));
	rightEngine = glm::vec3(planePos.x - engineOffsetx - (PlaneRotZ * 0.05), planePos.y + engineOffsety - (planeRotX * 2.5) - (PlaneRotZ * 2.5), planePos.z + engineOffsetz + (planeRotX * 0.05));
	// sets target value on first run after this will change target after an event i.e. hit engine, missed
	bird1target = leftEngine;
	bird2target = rightEngine;	
	bird3target = leftEngine;
}

void TheGame::GameLoop()
{
	// checks if still not in Exit state
	while (gameState != GameState::EXIT)
	{
		// continually plays audio of a jet engine
		//PlaySoundFiles(jetSound, glm::vec3(0.0f, 0.0f, 0.0f));
		// checks if engine has been hit too many times
		if (HitsTaken >= HitLimit) {
			cout << "Failed - Press any Key to Continue..." << endl;
			string check;
			cin >> check;
			gameState = GameState::EXIT;
		}
		// sets seed for random
		srand(time(0));
		if (gameState == GameState::PLAY)
		{
			// takes in keyboard 
			Keyboard();
			VisNormShader.Bind();
			//Render(false);
			Render(true);


			// checks if collision occurred with large sphere around plane
			if (Collided(*planeMovements.GetPos(), plane.getBoundingSphereRadius(), bird1.getBoundingSpherePos(), bird1.getBoundingSphereRadius())) {
				CheckBird1Collision();
			}

			if (Collided(*planeMovements.GetPos(), plane.getBoundingSphereRadius(), bird2.getBoundingSpherePos(), bird2.getBoundingSphereRadius())) {
				CheckBird2Collision();
			}

			if (Collided(*planeMovements.GetPos(), plane.getBoundingSphereRadius(), bird3.getBoundingSpherePos(), bird3.getBoundingSphereRadius())) {
				CheckBird3Collision();
			}

			CheckBirdsOutRange();
			
		}
	}
}

#pragma region BirdChecks

//checks if any birds are behind the camera
void TheGame::CheckBirdsOutRange() {
	glm::vec3 bird1CurrentPos = *bird1Movements.GetPos();
	if (bird1CurrentPos.z < -5) {
		// randomly generates the x and y positions
		bird1OrigPos = glm::vec3(rand() % 60 - 30, rand() % 60 - 30, 50);
		// sets back position back to start point
		bird1Incre = 0;
		bird1Movements.SetPos(bird1OrigPos);
		// finds closest engine part and makes that he target
		if (Distance(bird1OrigPos, rightEngine) < Distance(bird1OrigPos, leftEngine)) {
			bird1target = rightEngine;
		}
		else {
			bird1target = leftEngine;
		}
	}
	// repeated
	glm::vec3 bird2CurrentPos = *bird2Movements.GetPos();
	if (bird2CurrentPos.z < -5) {
		// the z changes as this gives different rate of movement 
		bird2OrigPos = glm::vec3(rand() % 60 - 30, rand() % 60 - 30, 100);
		bird2Incre = 0;
		bird2Movements.SetPos(bird2OrigPos);
		if (Distance(bird2OrigPos, rightEngine) < Distance(bird2OrigPos, leftEngine)) {
			bird2target = rightEngine;
		}
		else {
			bird2target = leftEngine;
		}
	}


	glm::vec3 bird3CurrentPos = *bird3Movements.GetPos();
	if (bird3CurrentPos.z < -5) {
		// random position is opposite of the second bird
		bird3OrigPos = glm::vec3(-bird2OrigPos.x, -bird2OrigPos.y, 100);
		bird3Incre = 0;
		bird3Movements.SetPos(bird3OrigPos);
		if (Distance(bird3OrigPos, rightEngine) < Distance(bird3OrigPos, leftEngine)) {
			bird3target = rightEngine;
		}
		else {
			bird3target = leftEngine;
		}
	}
}

void TheGame::CheckBird1Collision() {
	// checks if collided with one of the engines 
		if (Collided(leftEngine, 0.35, bird1.getBoundingSpherePos(), bird1.getBoundingSphereRadius()) || Collided(rightEngine, 0.35, bird1.getBoundingSpherePos(), bird1.getBoundingSphereRadius())) {
			// adds to hit amount and clears console and displays new value
			HitsTaken++;
			//system("cls");
			//cout << "Hits to engines: " << HitsTaken << " / " << HitLimit << endl;
			// plays sound
			//PlaySoundFiles(hitSound, *planeMovements.GetPos());
			// random position
			bird1OrigPos = glm::vec3(rand() % 60 - 30, rand() % 60 - 30, 50);
			// back to starting positition
			bird1Incre = 0;
			bird1Movements.SetPos(bird1OrigPos);
			// find target engine
			if (Distance(bird1OrigPos, rightEngine) < Distance(bird1OrigPos, leftEngine)) {
				bird1target = rightEngine;
			}
			else {
				bird1target = leftEngine;
			}
		}
		// if collided with main body of plane then randomise and reset position
		if (Collided(*planeMovements.GetPos() + glm::vec3(0, 1, 2), 1, bird1.getBoundingSpherePos(), bird1.getBoundingSphereRadius()) ||
			Collided(*planeMovements.GetPos() + glm::vec3(0, 1, 0), 1, bird1.getBoundingSpherePos(), bird1.getBoundingSphereRadius()) ||
			Collided(*planeMovements.GetPos() + glm::vec3(0, 1, -2), 1, bird1.getBoundingSpherePos(), bird1.getBoundingSphereRadius())) {
			//PlaySoundFiles(chirpSound, *planeMovements.GetPos());
			bird1OrigPos = glm::vec3(rand() % 60 - 30, rand() % 60 - 30, 50);
			bird1Incre = 0;
			bird1Movements.SetPos(bird1OrigPos);
			if (Distance(bird1OrigPos, rightEngine) < Distance(bird1OrigPos, leftEngine)) {
				bird1target = rightEngine;
			}
			else {
				bird1target = leftEngine;
			}
		}
}
// repeats for 2 other birds

void TheGame::CheckBird2Collision() {
	if (Collided(leftEngine, 0.35, bird2.getBoundingSpherePos(), bird2.getBoundingSphereRadius()) || Collided(rightEngine, 0.35, bird2.getBoundingSpherePos(), bird2.getBoundingSphereRadius())) {
		HitsTaken++;
		//system("cls");
		//cout << "Hits to engines: " << HitsTaken << " / " << HitLimit << endl;
		//PlaySoundFiles(hitSound, *planeMovements.GetPos());
		bird2OrigPos = glm::vec3(rand() % 60 - 30, rand() % 60 - 30, 100);
		bird2Incre = 0;
		bird2Movements.SetPos(bird2OrigPos);
		if (Distance(bird2OrigPos, rightEngine) < Distance(bird2OrigPos, leftEngine)) {
			bird2target = rightEngine;
		}
		else {
			bird2target = leftEngine;
		}
		if (Collided(*planeMovements.GetPos() + glm::vec3(0, 1, 2), 1, bird2.getBoundingSpherePos(), bird2.getBoundingSphereRadius()) ||
			Collided(*planeMovements.GetPos() + glm::vec3(0, 1, 0), 1, bird2.getBoundingSpherePos(), bird2.getBoundingSphereRadius()) ||
			Collided(*planeMovements.GetPos() + glm::vec3(0, 1, -2), 1, bird2.getBoundingSpherePos(), bird2.getBoundingSphereRadius())) {
			//PlaySoundFiles(chirpSound, *planeMovements.GetPos());
			bird2OrigPos = glm::vec3(rand() % 60 - 30, rand() % 60 - 30, 50);
			bird2Incre = 0;
			bird2Movements.SetPos(bird2OrigPos);
			if (Distance(bird2OrigPos, rightEngine) < Distance(bird2OrigPos, leftEngine)) {
				bird2target = rightEngine;
			}
			else {
				bird2target = leftEngine;
			}
		}
	}
}

void TheGame::CheckBird3Collision() {
	if (Collided(leftEngine, 0.35, bird3.getBoundingSpherePos(), bird3.getBoundingSphereRadius()) || Collided(rightEngine, 0.35, bird3.getBoundingSpherePos(), bird3.getBoundingSphereRadius())) {
		HitsTaken++;
		//system("cls");
		//cout << "Hits to engines: " << HitsTaken << " / " << HitLimit << endl;
		//PlaySoundFiles(hitSound, *planeMovements.GetPos());
		bird3OrigPos = glm::vec3(-bird2OrigPos.x, -bird2OrigPos.y, 100);
		bird3Incre = 0;
		bird3Movements.SetPos(bird3OrigPos);
		if (Distance(bird3OrigPos, rightEngine) < Distance(bird3OrigPos, leftEngine)) {
			bird3target = rightEngine;
		}
		else {
			bird3target = leftEngine;
		}
		if (Collided(*planeMovements.GetPos() + glm::vec3(0, 1, 2), 1, bird3.getBoundingSpherePos(), bird3.getBoundingSphereRadius()) ||
			Collided(*planeMovements.GetPos() + glm::vec3(0, 1, 0), 1, bird3.getBoundingSpherePos(), bird3.getBoundingSphereRadius()) ||
			Collided(*planeMovements.GetPos() + glm::vec3(0, 1, -2), 1, bird3.getBoundingSpherePos(), bird3.getBoundingSphereRadius())) {
			//PlaySoundFiles(chirpSound, *planeMovements.GetPos());aaaaa
			bird3OrigPos = glm::vec3(rand() % 60 - 30, rand() % 60 - 30, 50);
			bird3Incre = 0;
			bird3Movements.SetPos(bird3OrigPos);
			if (Distance(bird3OrigPos, rightEngine) < Distance(bird3OrigPos, leftEngine)) {
				bird3target = rightEngine;
			}
			else {
				bird3target = leftEngine;
			}
		}
	}

}

#pragma endregion

// checks inputs
void TheGame::Keyboard()
{
	SDL_Event keyboardEvent;
	
	while (SDL_PollEvent(&keyboardEvent))
	{
		// keystates checks what keys are not available
		const Uint8 *ks = SDL_GetKeyboardState(NULL);

		if (ks[SDL_SCANCODE_W]) {
			// w pressed then increment rotation in x axis if within 0.25
			if (planeRotX<0.25)
				planeRotX += 0.005f;
		}

		if (ks[SDL_SCANCODE_S]) {
			// s pressed then increment rotation in x axis if within 0.25
			if (planeRotX>-0.25)
				planeRotX -= 0.005f;
		}

		if (ks[SDL_SCANCODE_D]) {
			// d pressed then increment rotation in z axis if within 0.25
			if (PlaneRotZ<0.25)
				PlaneRotZ += 0.005f;
		}

		if (ks[SDL_SCANCODE_A]) {
			// a pressed then increment rotation in z axis if within 0.25
			if (PlaneRotZ>-0.25)
				PlaneRotZ -= 0.005f;
		}

		if (ks[SDL_SCANCODE_ESCAPE]) {
			// esc pressed then quit the game loop
			gameState = GameState::EXIT;
		}
	}
}

#pragma region GeneralMethods(i.e dist and collision check)

// finds distance between two points 
float TheGame::Distance(glm::vec3 a, glm::vec3 b) {
	return  glm::sqrt((b.x - a.x)*(b.x - a.x) + (b.y - a.y)*(b.y - a.y) + (b.z - a.z)*(b.z - a.z));
}

bool TheGame::Collided(glm::vec3 aP, float aR, glm::vec3 bP, float bR)
{
	// get distance
	float distance = Distance(aP, bP);
	// if distance is less than radius of two spheres then collision 
	if (distance < (aR + bR))
	{
		return true;
	}
	else
	{
		return false;
	}
}

#pragma endregion



void TheGame::SetToonLighting() {
	PlaneShader.setVec3("lightDir", glm::vec3(0.5, 0.5, 0.0));
	
}

void TheGame::SetRimToonLighting() {
	//PlaneShader.InitialiseShader("..\\res\\Shaders\\shaderRimToon");
	float explosionFactor;
	switch (HitsTaken) {
	case 0:
		explosionFactor = -1.6;
		break;
	case 1:
		explosionFactor = -1.45;
		break;
	case 2:
		explosionFactor = -1.30;
		break;
	case 3:
		explosionFactor = -1.15;
		break;
	case 4:
		explosionFactor = -1.0;
		break;
	case 5:
		explosionFactor = -0.85;
		break;
	}
	PlaneShader.Bind();
	PlaneShader.setVec3("lightDir", glm::vec3(1.5, 2.5, 3.5));
	PlaneShader.setVec3("InputColor", glm::vec3(0.5, 0.5, 0.5));
	PlaneShader.setMat4("u_vm", cam.GetView());
	PlaneShader.setMat4("u_pm", cam.GetProjection());
	PlaneShader.setFloat("time", explosionFactor);
}

//void TheGame::SetDissolveShader() {
//	DissShader.setMat4("matrix_viewProjection", cam.GetViewProjection());
//	DissShader.setMat4("matrix_model", glm::mat4(1.0f));
//	DissShader.SetTexture()
//}

void TheGame::SetVisNormShader() {
	VisNormShader.Bind();
	VisNormShader.setMat4("m", planeMovements.GetModel());
	VisNormShader.setMat4("v", cam.GetView());
	VisNormShader.setMat4("p", cam.GetProjection());
}

unsigned int TheGame::SetSkyboxTex() {

	unsigned int ID;
	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, ID);

	int width, height, nrChannels;
	
	for (GLuint i = 0; i < 6; i++)
	{
		unsigned char *data = stbi_load(skyTexture.GetFileName().c_str(), &width, &height, &nrChannels, 0);
		if (data) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else {
			std::cout << "Cubemap texture failed to load at path: " << skyTexture.GetFileName() << std::endl;
			stbi_image_free(data);
		}
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return ID;
}

void TheGame::SetSkyboxVertices() {

	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *) 0);

}

void TheGame::SetReflectionVertices() {

	glGenVertexArrays(1, &reflVAO);
	glGenBuffers(1, &reflVBO);
	glBindVertexArray(reflVAO);
	glBindBuffer(GL_ARRAY_BUFFER, reflVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
}

void TheGame::SetReflection() {
	PlaneShader.InitialiseShader("..\\res\\Shaders\\reflection");
	PlaneShader.Bind();
	//PlaneShader.setMat4("vp", cam.GetViewProjection());
	//PlaneShader.setMat4("m", /*planeMovements.GetModel()*/glm::mat4(1.0f));
	PlaneShader.setVec3("camPos", cam.GetPos());

	glBindVertexArray(reflVAO);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTex);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void TheGame::SetSkybox() {
	glDepthMask(GL_FALSE);
	Skybox.Bind();
	Skybox.setMat4("proj_view", cam.GetViewProjection());

	glBindVertexArray(skyboxVAO);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTex);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDepthMask(GL_TRUE);
}

void TheGame::ObjectMGR(bool loadShaders) {

	//    skybox method
	if (loadShaders) {
		SetSkybox();


		OldShader.Bind();
		// sets active texture
		skyTexture.Bind(0);
		// updates the view projection
		OldShader.UpdateShader(glm::vec3(0, 0, 0), cam);
		// draw sphere 
		//skySphere.RenderModel();

		//SetShader(OldShader);
	}
	


	//         plane method
	// sets position, rotation and scale for plane
	planeMovements.SetPos(glm::vec3(PlaneRotZ *-5, planeRotX *-5, 10));
	planeMovements.SetRot(glm::vec3(planeRotX, 0, PlaneRotZ));
	planeMovements.SetScale(glm::vec3(0.15, 0.15, 0.15));

	//SetVisNormShader();
	//plane.RenderModel();
	//SetVisNormShader();
	//plane.RenderModel();
	//SetReflection();
	if (loadShaders) {
		SetRimToonLighting();

		PlaneShader.UpdateShader(planeMovements, cam);
	}
	planeTexture.Bind(0);
	plane.RenderModel();
	// the sphere used for collision is set
	plane.SetBoundingSphere(*planeMovements.GetPos(), 7.0f);
	// left engine and right engine position found by calculating rotation and each engine offset
	planePos = *planeMovements.GetPos();
	leftEngine = glm::vec3(planePos.x + engineOffsetx - (PlaneRotZ * 0.05),
		planePos.y + engineOffsety - (planeRotX * 2.5) + (PlaneRotZ * 2.5),
		planePos.z + engineOffsetz + (planeRotX * 0.05));
	rightEngine = glm::vec3(planePos.x - engineOffsetx - (PlaneRotZ * 0.05),
		planePos.y + engineOffsety - (planeRotX * 2.5) - (PlaneRotZ * 2.5),
		planePos.z + engineOffsetz + (planeRotX * 0.05));

	//     bird methods
	if (loadShaders) {
		OldShader.Bind();
		OldShader.UpdateShader(glm::vec3(0, 0, 0), cam);
	}

	{
		// gets the direction to travel
		glm::vec3 bird1Dir = bird1OrigPos - bird1target;
		// sets position equal to starting point plus the direction tomes by progrssion of increment 
		bird1Movements.SetPos(bird1OrigPos + (bird1Dir * bird1Incre));
		bird1Movements.SetRot(glm::vec3(0.0, 0.0, 0.0));
		bird1Movements.SetScale(glm::vec3(0.25, 0.25, 0.25));

		PlaneShader.UpdateShader(bird1Movements, cam);
		birdTexture.Bind(0);
		bird1.RenderModel();
		bird1.SetBoundingSphere(*bird1Movements.GetPos(), 0.5f);
		// plusses over time and transforms bird along path
		bird1Incre = bird1Incre - 0.015f;
	}
	// repeated for other 2 bird models
	{
		glm::vec3 bird2Dir = bird2OrigPos - bird2target;
		bird2Movements.SetPos(bird2OrigPos + (bird2Dir * bird2Incre));
		bird2Movements.SetRot(glm::vec3(0.0, 0.0, 0.0));
		bird2Movements.SetScale(glm::vec3(0.5, 0.5, 0.5));
		PlaneShader.UpdateShader(bird2Movements, cam);
		birdTexture.Bind(0);
		bird2.RenderModel();
		bird2.SetBoundingSphere(*bird2Movements.GetPos(), 0.25f);
		bird2Incre = bird2Incre - 0.01f;
	}

	{
		glm::vec3 bird3Dir = bird3OrigPos - bird3target;
		bird3Movements.SetPos(bird3OrigPos + (bird3Dir * bird3Incre));
		bird3Movements.SetRot(glm::vec3(0.0, 0.0, 0.0));
		bird3Movements.SetScale(glm::vec3(0.5, 0.5, 0.5));

		PlaneShader.UpdateShader(bird3Movements, cam);
		birdTexture.Bind(0);
		bird3.RenderModel();
		bird3.SetBoundingSphere(*bird3Movements.GetPos(), 0.25f);
		bird3Incre = bird3Incre - 0.01f;
	}
}

void TheGame::Render(bool loadShaders)
{
	// sets display background to grey
	displayWindow.ClearDisplay(1.0f, 0.5f, 0.5f, 1.0f);
	// links shader to memory

	ObjectMGR(loadShaders);

	glEnableClientState(GL_COLOR_ARRAY); 
	glEnd();
	// swap over to other background window
	displayWindow.SwapBuff();
} 