/*
 *  RP_Game.cpp
 *  Memory
 *
 *  Created by Trevor Powell on 22/07/08.
 *  Copyright 2008 PanicKitten Softworks. All rights reserved.
 *
 */

#include "RP_Game.h"

#include "RP_ModeCredits.h"
#include "RP_ModeInGame.h"

#include "RP_Fader.h"

REGISTER_MAINGAME("Robot Finds Ice Cream", rpGame);

rpGame::rpGame()
{
	m_sceneCount = 4;	// we need four layers;  one for the game, one for the HUD, one for the Options screen, one for our cursor.
	m_showedIntro = false;
}

rpGame::~rpGame()
{
}

void
rpGame::Init()
{
	Parent::Init();

	//vsSystem::Instance()->HideCursor();

	m_enteringFromIntro = false;

	m_fader = new rpFader;
	m_fader->RegisterOnScene(2);

	m_mode[Mode_InGame] = new rpModeInGame(this);
	m_mode[Mode_Credits] = new rpModeCredits(this);

	m_music[Music_Main] = new vsMusic("FabCushion.ogg");
	m_currentMusic = -1;

	m_registerSound = new vsSoundSample("Register.ogg");

	PlayMusic(Music_Main);

#if 0
	if ( !m_showedIntro )
		SetMode( Mode_Intro );
	else
		SetMode( Mode_TitleScreen );
#else
	SetMode( Mode_InGame );
#endif

	m_showedIntro = true;
}

void
rpGame::Deinit()
{
	SetGameMode(NULL);

	vsSystem::GetScreen()->GetScene(0)->SetCamera2D(NULL);

	for ( int i = 0; i < MODE_MAX; i++ )
		vsDelete( m_mode[i] );

	for ( int i = 0; i < MUSIC_MAX; i++ )
		vsDelete( m_music[i] );

	vsDelete( m_fader );
	vsDelete( m_registerSound );

	Parent::Deinit();
}

void
rpGame::Update(float timeStep)
{
	Parent::Update(timeStep);

}

void
rpGame::SetMode( rpGame::GameMode mode )
{
	SetGameMode( m_mode[mode] );
}

void
rpGame::PlayMusic( Music type )
{
	if ( m_currentMusic != type )
	{
		m_currentMusic = type;
		m_music[type]->Start();
	}
}

void
rpGame::GoToPreferences()
{
	coreGame *prefs = coreGameRegistry::FindGame("Preferences");

	core::SetGame(prefs);
}

void
rpGame::FadeIn()
{
	m_fader->FadeIn();
}

void
rpGame::FadeOut()
{
	m_fader->FadeOut();
}

bool
rpGame::IsFading()
{
	return m_fader->IsFading();
}

void
rpGame::PlayRegisterSound()
{
	m_registerSound->Play();
}

