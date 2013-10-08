/*
 *  RP_ModeInGame.cpp
 *  RobotPal
 *
 *  Created by Trevor Powell on 22/07/08.
 *  Copyright 2008 PanicKitten Softworks. All rights reserved.
 *
 */

#include "RP_ModeInGame.h"

#include "RP_Game.h"
#include "RP_Map.h"
#include "RP_Player.h"

rpModeInGame::rpModeInGame( rpGame *game ):
	m_game(game)
{
}

void
rpModeInGame::Init()
{
	Parent::Init();

	m_game->FadeIn();

	m_currentLevel = 0;
	m_levelCount = 0;
	m_movingToNextLevel = false;

	while(1)
	{
		if ( !vsFile::Exists( vsFormatString("Map%d.lvl", m_levelCount) ) )
		{
			break;
		}
		m_levelCount++;
	};

	m_map = new rpMap;
	m_map->RegisterOnScene(0);
	m_map->Load(m_currentLevel);

}

void
rpModeInGame::Deinit()
{
	vsDelete(m_map);

	Parent::Deinit();
}

void
rpModeInGame::Update( float timeStep )
{
	Parent::Update(timeStep);

	if ( m_movingToNextLevel )
	{
		if ( !m_game->IsFading() )
		{
			m_movingToNextLevel = false;
			if ( m_currentLevel >= m_levelCount )
			{
				m_game->FadeIn();
				rpGame::Instance()->SetMode(rpGame::Mode_Credits);
			}
			else
			{
				vsDelete( m_map );
				m_map = new rpMap;
				m_map->RegisterOnScene(0);
				m_map->Load(m_currentLevel);
				m_game->FadeIn();
			}
		}
	}
	else
	{
		if ( m_map->IsSolved() )
		{
			m_game->FadeOut();
			m_movingToNextLevel = true;
			m_currentLevel++;
		}

		if ( rpGame::Instance()->GetInput()->WasPressed(CID_B) )
		{
			vsDelete( m_map );
			m_map = new rpMap;
			m_map->RegisterOnScene(0);
			m_map->Load(m_currentLevel);
		}
	}
}
