/*
 *  RP_Map.cpp
 *  RobotPal
 *
 *  Created by Trevor Powell on 24/07/08.
 *  Copyright 2008 PanicKitten Softworks. All rights reserved.
 *
 */

#include "RP_Map.h"

#include "RP_Game.h"
#include "RP_Player.h"
#include "RP_Robot.h"
#include "RP_VendingMachine.h"


const float cellWidth = 10.f;
const float cellHeight = 10.f;
const float cellRowOffset = 4.f;
const float cellColumnOffset = -0.2f;


rpMap::rpMap():
	m_white( new vsMaterial("White") ),
	m_opaque( new vsMaterial("Opaque") )
{
	m_cell = NULL;
	m_cellVisitedByRobot = NULL;
	m_width = m_height = 0;

	m_thickness = 4.f;

	m_player = NULL;
	m_robot = NULL;
	m_title = NULL;
	m_desc = NULL;
	m_descTwo = NULL;
	m_solved = false;

	for ( int i = 0; i < MAX_MACHINES; i++ )
	{
		m_machine[i] = NULL;
	}
	m_machineCount = 0;
}

rpMap::~rpMap()
{
	vsDeleteArray(m_cell);
	vsDeleteArray(m_cellVisitedByRobot);
	vsDelete(m_player);
	vsDelete(m_robot);
	vsDelete(m_title);
	vsDelete(m_desc);
	vsDelete(m_descTwo);
	vsDelete(m_white);
	vsDelete(m_opaque);
}

void
rpMap::Load(int mapId)
{
	vsDeleteArray(m_cell);

	vsString titleStr("Title");
	vsString descStr("Desc");
	vsString descTwoStr("DescTwo");
	vsString dimsStr("Dims");
	vsString lvlStr("Lvl");
	vsString playerStr("Player");
	vsString robotStr("Robot");
	vsString vendingStr("Vending");

	int curLine = 0;
	bool setDims = false;

	vsString fileName = vsFormatString("Map%d.lvl",mapId);
	vsFile file(fileName, vsFile::MODE_Read);

	vsRecord r;

	while( file.Record(&r) )
	{
		vsString l = r.GetLabel().AsString();
		if ( l == titleStr )
		{
			vsString str = r.GetToken(0).AsString();
			vsVector2D pos = vsSystem::GetScreen()->GetScene(1)->GetTopLeftCorner();
			pos += vsVector2D::One * 30.f;
			m_title = new vsSprite( vsBuiltInFont::CreateString( str, 20.f, 25.f ) );
			m_title->SetColor( vsColor::White );
			m_title->RegisterOnScene(1);
			m_title->SetPosition( pos );
		}
		if ( l == descStr )
		{
			vsAssert( m_desc == NULL, "Two Descs??" );
			vsString str = r.GetToken(0).AsString();
			vsVector2D pos = vsSystem::GetScreen()->GetScene(1)->GetTopLeftCorner();
			pos += vsVector2D::One * 60.f;
			m_desc = new vsSprite( vsBuiltInFont::CreateString( str, 15.f, 20.f ) );
			m_desc->SetColor( vsColor::White );
			m_desc->RegisterOnScene(1);
			m_desc->SetPosition( pos );
		}
		if ( l == descTwoStr )
		{
			vsAssert( m_descTwo == NULL, "Two Desc2s??" );
			vsString str = r.GetToken(0).AsString();
			vsVector2D pos = vsSystem::GetScreen()->GetScene(1)->GetTopLeftCorner();
			pos += vsVector2D::One * 90.f;
			m_descTwo = new vsSprite( vsBuiltInFont::CreateString( str, 15.f, 20.f ) );
			m_descTwo->SetColor( vsColor::White );
			m_descTwo->RegisterOnScene(1);
			m_descTwo->SetPosition( pos );
		}
		else if ( l == dimsStr )
		{
			vsAssert(!setDims, "Two Dims directives in the level file??");

			m_width = r.GetToken(0).AsInteger();
			m_height = r.GetToken(1).AsInteger();

			vsAssert(m_width > 0 && m_height > 0, "Illegal level dimensions??");

			m_cell = new int[m_width * m_height];
			m_cellVisitedByRobot = new bool[m_width * m_height];
			for ( int i = 0; i < m_width * m_height; i++ )
			{
				m_cellVisitedByRobot[i] = false;
			}

			setDims = true;
		}
		else if ( l == lvlStr )
		{
			vsAssert(setDims, "Lvl directive before Dims directive??");
			vsAssert(r.GetTokenCount() == m_width, "Wrong number of parameters to Lvl");
			vsAssert(curLine < m_height, "Too many Lvl lines??");

			for ( int i = 0; i < m_width; i++ )
			{
				Cell(i,curLine) = r.GetToken(i).AsInteger();
			}

			curLine++;
		}
		else if ( l == playerStr )
		{
			vsAssert(setDims, "Player directive before Dims directive??");
			vsAssert(r.GetTokenCount() == 2, "Wrong number of parameters to Player");
			vsAssert(!m_player, "More than one player??");

			int x = r.GetToken(0).AsInteger();
			int y = r.GetToken(1).AsInteger();

			rpMapCoord c(x,y);

			m_player = new rpPlayer();
			m_player->SetColor(vsColor::Blue);
			AddChild(m_player);
			m_player->SetMapCoord(c);
			m_player->SetPosition( GetCellPosition(c) );
		}
		else if ( l == robotStr )
		{
			vsAssert(setDims, "Robot directive before Dims directive??");
			vsAssert(r.GetTokenCount() == 2, "Wrong number of parameters to Robot");
			vsAssert(m_player, "Robot directive before Player directive??");
			vsAssert(!m_robot, "More than one robot??");

			int x = r.GetToken(0).AsInteger();
			int y = r.GetToken(1).AsInteger();

			rpMapCoord c(x,y);

			m_robot = new rpRobot(m_player);
			m_player->SetRobot(m_robot);
			m_robot->SetColor(vsColor::Red);
			AddChild(m_robot);
			m_robot->SetMapCoord(c);
			m_robot->SetPosition( GetCellPosition(c) );
		}
		else if ( l == vendingStr )
		{
			vsAssert(m_machineCount < MAX_MACHINES, "Too many vending machines!");

			int x = r.GetToken(0).AsInteger();
			int y = r.GetToken(1).AsInteger();

			rpMapCoord c(x,y);

			m_machine[m_machineCount] = new rpVendingMachine;
			m_machine[m_machineCount]->SetPosition( GetCellPosition(c) );
			m_machineCoord[m_machineCount] = c;
			AddChild(m_machine[m_machineCount]);

			m_machineCount++;
		}
	}

	vsAssert(curLine == m_height, "Not enough Lvl lines for declared dimensions?");

	vsAssert( m_width > 0.f, "Error!  Width of zero??" );
	vsScene *l = vsSystem::GetScreen()->GetScene(0);
	l->GetCamera()->SetFOV( m_width * 15.f );

}

void
rpMap::Update( float timeStep )
{
	if ( !m_player->IsMoving() && (!m_robot || !m_robot->IsMoving()) )
	{
		CheckForInput( timeStep );
	}

	{
		m_solved = true;
		for ( int i = 0; i < m_machineCount; i++ )
		{
			if ( !m_machine[i]->IsVisited() )
			{
				m_solved = false;
				break;
			}
		}
	}

	vsSprite::Update( timeStep );
}

void
rpMap::CheckForInput( float timeStep )
{
	vsInput *i = rpGame::Instance()->GetInput();

	rpMapCoord playerPos = m_player->GetMapCoord();
	rpMapCoord c = playerPos;

	if ( i->WasPressed(CID_Left) )
	{
		c.x--;
	}
	if ( i->WasPressed(CID_Right) )
	{
		c.x++;
	}
	if ( i->WasPressed(CID_Up) )
	{
		c.y--;
	}
	if ( i->WasPressed(CID_Down) )
	{
		c.y++;
	}

	if ( c != playerPos )
	{
		// check map thingie

		if ( Cell(c) != 0 )
		{
			TryToMovePlayerTo(c);
		}
	}
}

void
rpMap::TryToMovePlayerTo( const rpMapCoord &dc )
{
	bool canMove = true;

	rpMapCoord destinationC = dc;
	rpMapCoord playerC = m_player->GetMapCoord();
	rpMapCoord deltaC = destinationC - playerC;

	if ( m_robot )	// if there's a robot, we need to check whether this is a legal move for the robot.
	{
		rpMapCoord robotC = m_robot->GetMapCoord();
		rpMapCoord robotToDestinationC = destinationC - robotC;

		if ( robotC == destinationC )	// we're trying to move into the robot, so push the robot!
		{
			rpMapCoord robotDestinationC = FindRobotDestinationForShove(deltaC);

			if ( robotDestinationC == robotC )
			{
				canMove = false;
			}
			else
			{
				m_robot->MoveTo( GetCellPosition( robotDestinationC ) );
				m_robot->SetMapCoord( robotDestinationC );
				destinationC = robotDestinationC - deltaC;

				for ( int i = 0; i < m_machineCount; i++ )
				{
					if ( robotDestinationC == m_machineCoord[i] )
					{
						m_machine[i]->SetVisited();
					}
				}
			}
		}
		else if ( vsFabs(robotToDestinationC.x) <= 1 && vsFabs(robotToDestinationC.y) <= 1 )
		{
			// do nothing;  my leash reaches to here, so don't need to move the robot at all
		}
		else if ( robotC == playerC - deltaC )	// am I pulling the robot behind me?
		{
			if ( CellVisited( robotC + deltaC ) )
			{
				canMove = false;
			}
			else
			{
				CellVisited( robotC ) = true;
				m_robot->SetMapCoord(robotC + deltaC);
				m_robot->MoveTo( GetCellPosition( robotC + deltaC ) );

				for ( int i = 0; i < m_machineCount; i++ )
				{
					if ( robotC + deltaC == m_machineCoord[i] )
					{
						m_machine[i]->SetVisited();
					}
				}
			}
		}
		else
		{
			canMove = false;
		}
	}

	if ( canMove )
	{
		m_player->SetMapCoord(destinationC);
		m_player->MoveTo( GetCellPosition( destinationC ) );

		if ( !m_robot )	// no robot?  Must be a tutorial level!  Let the player hit vending machines!
		{
			for ( int i = 0; i < m_machineCount; i++ )
			{
				if ( destinationC == m_machineCoord[i] )
				{
					m_machine[i]->SetVisited();
				}
			}
		}
	}
}

rpMapCoord
rpMap::FindRobotDestinationForShove( const rpMapCoord &direction )
{
	rpMapCoord robotPos = m_robot->GetMapCoord();
	rpMapCoord shovePos = robotPos;
	rpMapCoord prevPos = robotPos;

	while( Cell(shovePos) != 0 && !CellVisited( shovePos ) )
	{
		if ( prevPos != robotPos )
		{
			CellVisited( prevPos ) = true;
		}
		prevPos = robotPos;
		robotPos = shovePos;
		shovePos += direction;
	}
	if ( prevPos != robotPos )
	{
		CellVisited( prevPos ) = true;
	}

	return robotPos;
}

void
rpMap::DynamicDraw( vsRenderQueue *queue )
{
	vsDisplayList *list = queue->MakeTemporaryBatchList( m_white, queue->GetMatrix(), 10 * 1024 );
	vsDisplayList *opaqueList = queue->MakeTemporaryBatchList( m_opaque, queue->GetMatrix(), 10 * 1024 );

	float hch = 0.5f * cellHeight;
	float hcw = 0.5f * cellWidth;

	float hoffw = 0.5f * cellRowOffset;
	float hoffh = 0.5f * cellColumnOffset;

	float thicknessy = 0.8f;
	float thicknessx = -.0f;

	vsVector2D t(thicknessx, thicknessy);
	t *= m_thickness;

	vsVector2D v[8] = {
		vsVector2D(-hcw - hoffw, -hch - hoffh),
		vsVector2D(hcw - hoffw, -hch + hoffh),
		vsVector2D(hcw + hoffw, hch + hoffh),
		vsVector2D(-hcw + hoffw, hch - hoffh),

		vsVector2D((-hcw - hoffw) + t.x, (-hch - hoffh) + t.y),
		vsVector2D((hcw - hoffw) + t.x, (-hch + hoffh) + t.y),
		vsVector2D((hcw + hoffw) + t.x, (hch + hoffh) + t.y),
		vsVector2D((-hcw + hoffw) + t.x, (hch - hoffh) + t.y)
	};
	int index[5] = {0,1,2,3,0};
	int indexTwo[5] = {0,4,7,6,2};
	int indexThree[2] = {3,7};

	int	frontStrip[6] = {0, 4, 3, 7, 2, 6};

	list->VertexArray(v,8);
	opaqueList->VertexArray(v,8);
	vsTransform2D trans;

	vsColor surfaceColor(0.0f,0.0f,0.1f,1.0f);
	vsColor sideColor(0.1f,0.0f,0.1f,1.0f);

	vsColor rememberedColor(0.1f,0.f,0.f,1.f);

	for ( int y = 0; y < m_height; y++ )		// draw top to bottom
	{
		for ( int x = m_width-1; x >= 0; x-- )	// draw right to left
		{
			if ( Cell(x,y) == 1 )
			{
				trans.SetTranslation( GetCellPosition(x,y) );
				/*v[0] = GetVertexPosition(x,y);
				v[1] = GetVertexPosition(x+1,y);
				v[2] = GetVertexPosition(x+1,y+1);
				v[3] = GetVertexPosition(x,y+1);*/
				list->PushTransform(trans);
				opaqueList->PushTransform(trans);

				if ( CellVisited(x,y) )
				{
					list->SetColor(rememberedColor);
					opaqueList->SetColor(rememberedColor);
				}
				else
				{
					list->SetColor(surfaceColor);
					opaqueList->SetColor(surfaceColor);
				}

				opaqueList->TriangleFan(index,4);
				opaqueList->SetColor(sideColor);
				opaqueList->TriangleStrip(frontStrip,6);

				list->SetColor(vsColor(0.5f,0.5f,0.5f,0.3f));
				list->LineStrip(index,5);
				list->LineStrip(indexTwo,5);
				list->LineStrip(indexThree,2);

				opaqueList->PopTransform();
				list->PopTransform();
			}
		}
	}
}

vsVector2D
rpMap::GetCellPosition(int x, int y)
{
	const float baseX = (-0.5f * m_width * cellWidth) + (cellWidth * 0.5f);
	const float baseY = (-0.5f * m_height * cellHeight) + (cellHeight * 0.5f);

	return vsVector2D( baseX + x * cellWidth + y * cellRowOffset, baseY + y * cellHeight + x * cellColumnOffset );
}

vsVector2D
rpMap::GetVertexPosition(int x, int y)
{
	const float baseX = -0.5f * m_width * cellWidth;
	const float baseY = -0.5f * m_height * cellHeight;

	return vsVector2D( baseX + x * cellWidth, baseY + y * cellHeight );
}

static int s_offMap = 0;
static bool s_offVisited = false;

int &
rpMap::Cell(int x, int y)
{
	// if we're off the edge of the map, return our static offmap variable.
	if ( x < 0 || x >= m_width || y < 0 || y >= m_height )
	{
		s_offMap = 0;
		return s_offMap;
	}

	return m_cell[(y * m_width) + x];
}

bool &
rpMap::CellVisited(int x, int y)
{
	// if we're off the edge of the map, return our static offmap variable.
	if ( x < 0 || x >= m_width || y < 0 || y >= m_height )
	{
		s_offVisited = false;
		return s_offVisited;
	}

	return m_cellVisitedByRobot[(y * m_width) + x];
}

