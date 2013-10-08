/*
 *  RP_Map.h
 *  RobotPal
 *
 *  Created by Trevor Powell on 24/07/08.
 *  Copyright 2008 PanicKitten Softworks. All rights reserved.
 *
 */

#ifndef RP_MAP_H
#define RP_MAP_H

class rpVendingMachine;
class rpPlayer;
class rpRobot;

#define MAX_MACHINES (10)

class rpMapCoord
{
public:
	int	x;
	int	y;

	rpMapCoord() {x =  y = 0;}
	rpMapCoord(int x_in, int y_in) {x = x_in; y = y_in;}

	bool operator==(const rpMapCoord &o) {return (x==o.x && y==o.y);}
	bool operator!=(const rpMapCoord &o) {return (x!=o.x || y!=o.y);}

	rpMapCoord  operator+( const rpMapCoord &b ) const {return rpMapCoord(x+b.x, y+b.y);}
	rpMapCoord  operator-( const rpMapCoord &b ) const {return rpMapCoord(x-b.x, y-b.y);}
	rpMapCoord  operator*( float b ) const {return rpMapCoord(x*b,y*b);}
	rpMapCoord  operator*=( float b ) {x*=b; y*=b; return *this;}
	rpMapCoord  operator+=( const rpMapCoord &b ) {*this = *this+b; return *this; }
	rpMapCoord  operator-=( const rpMapCoord &b ) {*this = *this-b; return *this; }
};

class rpMap : public vsSprite, public vsSingleton<rpMap>
{
	rpPlayer *	m_player;
	rpRobot *	m_robot;

	vsSprite *	m_title;
	vsSprite *	m_desc;
	vsSprite *	m_descTwo;

	rpVendingMachine *	m_machine[MAX_MACHINES];
	rpMapCoord			m_machineCoord[MAX_MACHINES];
	int					m_machineCount;

	vsMaterial * m_white;
	vsMaterial * m_opaque;

	int		*m_cell;
	bool	*m_cellVisitedByRobot;

	int		m_width;
	int		m_height;

	float	m_thickness;

	bool	m_solved;

	void	CheckForInput(float timeStep);
	void	TryToMovePlayerTo( const rpMapCoord &cell );
	rpMapCoord	FindRobotDestinationForShove( const rpMapCoord &direction );

public:

	rpMap();
	virtual ~rpMap();

	void	Load(int mapNumber);
	void	Save(int mapNumber);

	virtual void	Update(float timeStep);
	virtual void	DynamicDraw( vsRenderQueue *queue );

	bool	IsSolved() { return m_solved; }

	int		GetWidth() { return m_width; }
	int		GetHeight() { return m_height; }

	vsVector2D	GetCellPosition(int x, int y);
	vsVector2D	GetVertexPosition(int x, int y);

	vsVector2D	GetCellPosition(const rpMapCoord &c) { return GetCellPosition(c.x,c.y); }
	vsVector2D	GetVertexPosition(const rpMapCoord &c) { return GetVertexPosition(c.x,c.y); }

	bool &	CellVisited(const rpMapCoord &c) { return CellVisited(c.x,c.y); }
	bool &	CellVisited(int x, int y);
	int &	Cell(const rpMapCoord &c) { return Cell(c.x,c.y); }
	int &	Cell(int x, int y);
};


#endif // RP_MAP_H

