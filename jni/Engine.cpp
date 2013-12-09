#include <Engine.h>
#include <stdlib.h>
#include <FileReader.h>
#include <cmath>
#include <Windows.h>
#include <time.h>

#include <Quad.h>
//#include <Windows.h>
#include <iostream>

#undef isnan

Engine::Engine()
{
	Scale = blackBarH = blackBarV = 0;

	x = y			= 0;
	position1		= 0;
	positionPhase	= 0;
	rotationangle	= 0;

	//Make link texture
	texture =	new Texture();
	texture->loadTexture("Link2.tga");

	//Make octorock texture
	texture_octo = new Texture();
	texture_octo->loadTexture("Octo.tga");

	//Make ganon texture
	texture_ganon = new Texture();
	texture_ganon->loadTexture("Ganon.tga");

	shader =	new Shader();
	camera =	new Camera();
	keyboard =	new Keyboard();

	static float angle = 0.0f;
	angle += 0.5f;

	//Draw texture 1
	Quad1 =	new Quad(128,128,0,100,100,0);
	Quad1->setShader(shader);
	Quad1->setTexture(texture_octo->texture);

	//Draw texture 2
	Quad2 = new Quad(150,150,0,100,100,0);
	Quad2->setShader(shader);
	Quad2->setTexture(texture->texture);

	//Draw texture 3
	Quad3 = new Quad(300,300,0,0,0,0);
	Quad3->setShader(shader);
	Quad3->setTexture(texture_ganon->texture);

	sine		= 0;
	position1	= 0;
	link_rotate = 0;
}

Engine::~Engine()
{
	//Delete always pointers! If there is "new Thing()" -> delete it.
	delete	texture;
	delete	texture_octo;
	delete	texture_ganon;
	delete	Quad1;
	delete	Quad2;
	delete	Quad3;

}

void Engine::Update()
{
	rotationangle += 1;

	//Octo
	Quad1->setPosition(sin(rotationangle/15)*100+600, cos(rotationangle/15)*100+400,0);
	Quad1->rotate(rotationangle,0,0,1);
	//Link
	//Quad2->setPosition(rotationangle,0,0);
	//Ganon
	Quad3->setPosition(800,250,0);
	Quad3->resize(abs(sin(rotationangle/50))*120,abs(sin(rotationangle/50)*120),0);
	Quad3->rotate(rotationangle,0,0,-1);

	if(GetAsyncKeyState(VK_UP))
	{
		Quad2->move(0,5);
	}
	if(GetAsyncKeyState(VK_DOWN))
	{
		Quad2->move(0,-5);
	}
	if(GetAsyncKeyState(VK_LEFT))
	{
		Quad2->move(-5,0);
	}
	if(GetAsyncKeyState(VK_RIGHT))
	{
		Quad2->move(5,0);
	}
	if(GetAsyncKeyState(VK_SPACE))
	{
		link_rotate += 50;
		Quad2->rotate(link_rotate,0,0,1);
	}
	else
	{
		link_rotate = 0;
		Quad2->rotate(link_rotate,0,0,1);
	}

	Quad2->setColor(glm::vec4(0,1,1,1));
}

	/**
	*  This code fixes the aspect ration.
	*/
void Engine::fixAspectRatio(float desiredWidth, float desiredHeight, float width, float height)
{
	//////////////////Don't change anything, until its really really necessary!/////////////////////////

	//Calculate desired Aspect Ratio
	float dAR = desiredWidth/desiredHeight;

	//Calculate real Aspect Ratio
	float rAR = width/height;
	float w,h;
	//check Aspect Ratio's
	if(dAR == rAR)
	{
		//Same aspect, no letterboxing needed!
		Scale = width/desiredWidth;
	}
	else if(dAR<rAR)
	{
		//Horizontal letterboxing needed!
		Scale = height/desiredHeight;
		blackBarH = (width-Scale*desiredWidth)/2;
	}
	else
	{
		//Vertical letterboxing needed!
		Scale = width/desiredWidth;
		blackBarV = (height-(Scale*desiredHeight))/2;
	}
	w = desiredWidth*Scale;
	h = desiredHeight*Scale;

	glViewport((int)blackBarH, (int)blackBarV, (int)w, (int)h); // Sets up the OpenGL viewport
	
	checkGlError("ennen setuniformia");
	shader->setUniformMatrix("matProjection",camera->Projection);
	checkGlError("setuniformi projection");
	shader->setUniformMatrix("View",camera->View);
	checkGlError("setuniformi view");

	//GLfloat projection[16]  = 
	//{
	//	1.0f/desiredWidth, 0,	0,	-0.5f
	//	,0,	1.0f/desiredHeight,	0,	0.5f
	//	,0,	0,	1,		0
	//	,0,	0,	0,		1
	//};
	//Quad::Projection = (float*)calloc(16,sizeof(float));
	//Quad::Projection[0] = projection[0];
	//Quad::Projection[1] = projection[1];
	//Quad::Projection[2] = projection[2];
	//Quad::Projection[3] = projection[3];
	//Quad::Projection[4] = projection[4];
	//Quad::Projection[5] = projection[5];
	//Quad::Projection[6] = projection[6];
	//Quad::Projection[7] = projection[7];
	//Quad::Projection[8] = projection[8];
	//Quad::Projection[9] = projection[9];
	//Quad::Projection[10] = projection[10];
	//Quad::Projection[11] = projection[11];
	//Quad::Projection[12] = projection[12];
	//Quad::Projection[13] = projection[13];
	//Quad::Projection[14] = projection[14];
	//Quad::Projection[15] = projection[15];

}


void Engine::Draw()
{
	checkGlError("Engine draw");
	//Draw
	glClearColor(0.3f,0.5f,0.3f,1);
	#ifndef ANDROID_FRAMEWORK
	glClearDepth(1);
	#else
	glClearDepthf(1);
	#endif
	//Important GL draw clear thing
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	//Quad draws goes here. Remember the draw order.
	Quad1->setColor(glm::vec4(1,0,1,1));
	Quad1->Draw(0.8f);
	Quad3->setColor(glm::vec4(0,1,1,1));
	Quad3->Draw(0.9f);
	Quad2->setColor(glm::vec4(1,1,0,1));
	Quad2->Draw(0.7f);
}

