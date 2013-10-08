/*
 *  RP_VendingMachine.cpp
 *  RobotPal
 *
 *  Created by Trevor Powell on 27/07/08.
 *  Copyright 2008 PanicKitten Softworks. All rights reserved.
 *
 */

#include "RP_VendingMachine.h"

#include "RP_Game.h"

rpVendingMachine::rpVendingMachine():
	vsSprite( NULL )
{
	m_normalList = vsDisplayList::Load("VendingMachine");
	m_visitedList = vsDisplayList::Load("VisitedMachine");
	m_visited = false;
	
	SetDisplayList(m_normalList);
}

rpVendingMachine::~rpVendingMachine()
{
	SetDisplayList(NULL);
	
	vsDelete(m_normalList);
	vsDelete(m_visitedList);
}

void
rpVendingMachine::SetVisited()
{
	if ( !m_visited )
	{
		rpGame::Instance()->PlayRegisterSound();
	}
	m_visited = true;
	SetDisplayList(m_visitedList);
}

