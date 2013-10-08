/*
 *  RP_Player.cpp
 *  RobotPal
 *
 *  Created by Trevor Powell on 24/07/08.
 *  Copyright 2008 PanicKitten Softworks. All rights reserved.
 *
 */

#include "RP_Player.h"

#include "RP_Game.h"
#include "RP_Map.h"
#include "RP_Robot.h"

rpPlayer::rpPlayer():
	vsSprite( NULL ),
	m_white( new vsMaterial("White") )
{
	m_moving = false;
	m_torso = vsDisplayList::Load("Player");
	m_leg = vsDisplayList::Load("PlayerLeg");

	SetScale( vsVector2D::One * 0.1f );

	m_legCycle = 0.f;
}

rpPlayer::~rpPlayer()
{
	vsDelete( m_torso );
	vsDelete( m_leg );
	vsDelete( m_white );
}

void
rpPlayer::SetMapCoord(const rpMapCoord &coord)
{
	m_coord = coord;
	//SetPosition( rpMap::Instance()->GetCellPosition(coord) );
}

void
rpPlayer::Update( float timeStep )
{
	if ( m_moving )
	{
		m_moveTimer += timeStep;

		m_legCycle += timeStep * 10.f;
		if ( m_legCycle > 1.f )
			m_legCycle -= 1.f;

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
	else
	{
		m_legCycle = 0.5f;
	}
}

void
rpPlayer::DynamicDraw( vsRenderQueue *queue )
{
	vsDisplayList *list = queue->MakeTemporaryBatchList( m_white, queue->GetMatrix(), 10 * 1024 );
	list->Append( *m_torso );

	float ang = DEGREES(45.0f);
	vsAngle angle = vsCos(m_legCycle * PI) * ang;
	vsTransform2D l;
	l.SetTranslation(5.f,0.f);
	l.SetAngle(angle);

	list->PushTransform(l);
	list->Append( *m_leg );
	list->PopTransform();

	l.SetAngle( -angle );

	list->PushTransform(l);
	list->Append( *m_leg );
	list->PopTransform();
}

void
rpPlayer::MoveTo( const vsVector2D &where )
{
	m_moving = true;

	m_startPos = GetPosition();
	m_endPos = where;
	m_moveDuration = 0.2f;
	m_moveTimer = 0.f;
}

