/*
 *  RP_ModeInGame.h
 *  RobotPal
 *
 *  Created by Trevor Powell on 22/07/08.
 *  Copyright 2008 PanicKitten Softworks. All rights reserved.
 *
 */

#ifndef RP_MODEINGAME_H
#define RP_MODEINGAME_H

class rpGame;
class rpMap;
class rpPlayer;

class rpModeInGame : public coreGameMode
{
	typedef coreGameMode Parent;
	
	rpGame *			m_game;
	//rpPlayer *			m_player;
	
	rpMap *				m_map;
	
	bool				m_movingToNextLevel;
	
	int					m_currentLevel;
	int					m_levelCount;

public:
	
	rpModeInGame(rpGame *game);
	
	virtual void		Update(float timeStep);
	
	virtual void		Init();
	virtual void		Deinit();
	
};

#endif // RP_MODEINGAME_H

