/*
 *  RP_Fader.cpp
 *  RobotPal
 *
 *  Created by Trevor Powell on 27/07/08.
 *  Copyright 2008 PanicKitten Softworks. All rights reserved.
 *
 */

#include "RP_Fader.h"

rpFader::rpFader():
	m_material( new vsMaterial("opaque") )
{
	m_fading = false;
	m_alpha = 1.0f;
}

rpFader::~rpFader()
{
	vsDelete( m_material );
}

void
rpFader::FadeTo( float alpha )
{
	m_startAlpha = m_alpha;
	m_endAlpha = alpha;
	m_timer = 0.f;
	m_fadeDuration = 1.0f;
	m_fading = true;
	m_fadeType = vsRandom::GetInt(4);
}

void
rpFader::Update( float timeStep )
{
	if ( m_fading )
	{
		m_timer += timeStep;

		if ( m_timer > m_fadeDuration )
		{
			m_alpha = m_endAlpha;
			m_fading = false;
		}
		else
		{
			m_alpha = vsInterpolate( m_timer / m_fadeDuration, m_startAlpha, m_endAlpha );
		}
	}
}

void
rpFader::DynamicDraw( vsRenderQueue *queue )
{
	vsDisplayList *list = queue->MakeTemporaryBatchList(m_material, queue->GetMatrix(), 10 * 1024);
	vsVector2D tl = vsSystem::GetScreen()->GetScene(2)->GetTopLeftCorner();
	vsVector2D br = vsSystem::GetScreen()->GetScene(2)->GetBottomRightCorner();

	vsVector2D v[4];

	switch ( m_fadeType )
	{
		case 0:
		{
			float x = vsInterpolate( m_alpha, tl.x, br.x );
			v[0] = tl;
			v[1].Set(x, tl.y);
			v[2].Set(x, br.y);
			v[3].Set(tl.x,br.y);
			break;
		}
		case 1:
		{
			float x = vsInterpolate( m_alpha, br.x, tl.x );
			v[0].Set(x, tl.y);
			v[1].Set(br.x, tl.y);
			v[2].Set(br.x, br.y);
			v[3].Set(x,br.y);
			break;
		}
		case 2:
		{
			float y = vsInterpolate( m_alpha, tl.y, br.y );
			v[0] = tl;
			v[1].Set(br.x, tl.y);
			v[2].Set(br.x, y);
			v[3].Set(tl.x, y);
			break;
		}
		case 3:
		{
			float y = vsInterpolate( m_alpha, br.y, tl.y );
			v[0].Set(tl.x, br.y);
			v[1].Set(br.x, br.y);
			v[2].Set(br.x, y);
			v[3].Set(tl.x, y);
			break;
		}
	}

	int indices[4] = { 0, 1, 2, 3 };

	list->SetColor( vsColor(0.f,0.f,0.f,1.f) );
	list->VertexArray(v,4);
	list->TriangleFan(indices,4);
}


