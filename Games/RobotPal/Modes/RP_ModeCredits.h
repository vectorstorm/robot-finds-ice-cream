/*
 *  MYS_ModeCredits.h
 *  VectorStorm
 *
 *  Created by Trevor Powell on 9/01/08.
 *  Copyright 2008 PanicKitten Softworks. All rights reserved.
 *
 */

#ifndef MYS_MODECREDITS_H
#define MYS_MODECREDITS_H

class rpGame;
class vsSprite;
class vsFile;

#define MAX_CREDIT_LINES (25)

class rpModeCredits : public coreGameMode
{
	rpGame *	m_game;
	vsFile *	m_credits;

	vsSprite *	m_line[MAX_CREDIT_LINES];
	int			m_lineId[MAX_CREDIT_LINES];
	bool		m_lineUsed[MAX_CREDIT_LINES];
	int			m_lineCount;
	bool		m_creditsDone;

	bool		m_fadingOut;
	bool		m_inittedFade;
	float		m_fadingTimer;


	float		m_creditsPos;

	void		LoadLine( int i );	// load a line into slot 'i'.

public:

	rpModeCredits( rpGame *game );
	virtual ~rpModeCredits();

	virtual void Init();
	virtual void Deinit();

	virtual void Update(float timeStep);
};


#endif // MYS_MODECREDITS_H
