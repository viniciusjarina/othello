
#include <troll.h>

#include "OthelloApp.h"
#include "GameScene.h"

using Troll::System;

using Troll::Screen;

using Troll::MouseInput;
using Troll::KeyInput;

int OthelloApp::Run()
{
	if(!Init())
		return 1;

	bool quit = false;

	GameScene scene;

	while(!quit) // was ESC key pressed?
	{
		Screen::StartFrame();
		
		MouseInput::Update();
		KeyInput::Update();

		quit = scene.Logic();
			
		if(!Screen::SkipFrame()) // Frame can be rendered
		{
			scene.Draw();
		}
		Screen::Flip();		// Flip screen
	}


	Cleanup();
	return 0;
}

bool OthelloApp::Init()
{
	if(!System::Init())			// Inialize (input, sound, files, etc)
		return false;
	
	if(!System::SetupScreen(320,320))	// Setup and create screen with defaul size
		return false;
	
	System::SetScreenTitle("Othello");

	return true;
}

void OthelloApp::Cleanup()
{
	System::Cleanup();
}

OthelloApp::OthelloApp()
{
	
}