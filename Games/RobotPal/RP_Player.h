/*
 *  RP_Player.h
 *  RobotPal
 *
 *  Created by Trevor Powell on 24/07/08.
 *  Copyright 2008 PanicKitten Softworks. All rights reserved.
 *
 */

#ifndef RP_PLAYER_H
#define RP_PLAYER_H

class rpRobot;

#include "RP_Map.h"

class rpPlayer : public vsSprite
{
	rpMapCoord		m_coord;

	vsDisplayList *	m_torso;
	vsDisplayList *	m_leg;

	vsMaterial *	m_white;

	float			m_legCycle;

	rpRobot *		m_robot;

	vsVector2D		m_startPos;
	vsVector2D		m_endPos;
	float			m_moveTimer;
	float			m_moveDuration;
	bool			m_moving;

	void			TryToMoveTo(const rpMapCoord &coord);

public:

					rpPlayer();
	virtual			~rpPlayer();

	void			SetRobot( rpRobot *robot ) { m_robot = robot; }
	rpMapCoord		GetMapCoord() { return m_coord; }
	void			SetMapCoord( const rpMapCoord &c );

	void			MoveTo( const vsVector2D &where );
	bool			IsMoving() { return m_moving; }

	virtual void	Update( float timeStep );
	virtual void	DynamicDraw( vsRenderQueue *queue );
};

#endif // RP_PLAYER_H

