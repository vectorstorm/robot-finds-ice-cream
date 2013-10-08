/*
 *  RP_Robot.cpp
 *  RobotPal
 *
 *  Created by Trevor Powell on 24/07/08.
 *  Copyright 2008 PanicKitten Softworks. All rights reserved.
 *
 */

#include "RP_Robot.h"

#include "RP_Map.h"
#include "RP_Player.h"

rpRobot::rpRobot(rpPlayer *player):
vsSprite( vsDisplayList::Load("Robot") )
{
	m_player = player;
	m_moving = false;
}

rpRobot::~rpRobot()
{
}

void
rpRobot::SetMapCoord( const rpMapCoord &coord )
{
	m_coord = coord;
//	SetPosition( rpMap::Instance()->GetCellPosition(m_coord) );
}

void
rpRobot::Update( float timeStep )
{
	if ( m_moving )
	{
		m_moveTimer += timeStep;
		
		if ( m_moveTimer > m_moveDuration )
		{
			m_moving = false;
			SetPosition( m_endPos );
		}
		else
		{
			SetPosition( vsInterpolate( m_moveTimer / m_moveDuration, m_startPos, m_endPos ) );
		}
	}
}


void
rpRobot::MoveTo( const vsVector2D &where )
{
	m_moving = true;
	
	m_startPos = GetPosition();
	m_endPos = where;
	m_moveDuration = 0.1f;
	m_moveTimer = 0.f;
}


/*
bool
rpRobot::CanMove(int dx, int dy)
{
	if ( rpMap::Instance()->Cell(m_x+dx, m_y+dy) != 0 )
	{
		return true;
	}
	
	return false;
}*/
/*
void
rpRobot::Move(int dx, int dy)
{
	SetMapPos(m_x+dx, m_y+dy);
}

bool
rpRobot::LeashReachesMapPos(int x, int y)
{
	int dx = m_x-x;
	int dy = m_y-y;
	
	if ( vsFabs(dx) > 1 || vsFabs(dy) > 1 )
		return false;
	
	return true;
}
*/

