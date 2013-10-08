/*
 *  RP_VendingMachine.h
 *  RobotPal
 *
 *  Created by Trevor Powell on 27/07/08.
 *  Copyright 2008 PanicKitten Softworks. All rights reserved.
 *
 */

#ifndef RP_VENDINGMACHINE_H
#define RP_VENDINGMACHINE_H

class rpVendingMachine : public vsSprite
{
	vsDisplayList *		m_normalList;
	vsDisplayList *		m_visitedList;
	
	bool				m_visited;
public:
	
	rpVendingMachine();
	virtual ~rpVendingMachine();
	
	void		SetVisited();
	bool		IsVisited() { return m_visited; }
};

#endif // RP_VENDINGMACHINE_H

