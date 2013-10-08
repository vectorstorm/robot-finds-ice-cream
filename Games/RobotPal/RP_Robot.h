/*
 *  RP_Robot.h
 *  RobotPal
 *
 *  Created by Trevor Powell on 24/07/08.
 *  Copyright 2008 PanicKitten Softworks. All rights reserved.
 *
 */

#ifndef RP_ROBOT_H
#define RP_ROBOT_H

class rpPlayer;

#include "RP_Map.h"

class rpRobot : public vsSprite
{
	rpPlayer *		m_player;
	rpMapCoord		m_coord;
	
	vsVector2D		m_startPos;
	vsVector2D		m_endPos;
	float			m_moveTimer;
	float			m_moveDuration;
	bool			m_moving;
	
	
public:
	rpRobot(rpPlayer *player);
	virtual ~rpRobot();
	
	rpMapCoord		GetMapCoord() { return m_coord; }
	void			SetMapCoord( const rpMapCoord &c );

//	void			SetMapPos(int x, int y);
//	bool			IsAtMapPos(int x, int y) { return (m_x == x && m_y == y); }
//	bool			LeashReachesMapPos(int x, int y);
	
	void			MoveTo( const vsVector2D &where );
	bool			IsMoving() { return m_moving; }
	
	
	virtual void	Update( float timeStep );
};

#endif // RP_ROBOT_H
